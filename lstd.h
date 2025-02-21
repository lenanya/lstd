#ifndef LSTD
#define LSTD
#include <stdio.h>
#include <stddef.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>
#include <time.h>

#ifdef LSTD_DSA
#define LSTD_DSA_INITIAL_CAPACITY 256

typedef struct lstd_dynamic_string_array {
    size_t count;
    size_t capacity;
    char **data;
} lstd_dynamic_string_array;

// forward declarations
void lstd_dsa_free(lstd_dynamic_string_array *da);
void lstd_dsa_append(lstd_dynamic_string_array *da, char *str);
void lstd_dsa_append_many(lstd_dynamic_string_array *da, size_t amount, ...);
void lstd_dsa_remove_at(lstd_dynamic_string_array *da, size_t n);
void lstd_dsa_pop(lstd_dynamic_string_array *da);
void lstd_dsa_insert_at(lstd_dynamic_string_array *da, char *str, size_t n);
char *lstd_shift(char **xs);


void lstd_dsa_append(lstd_dynamic_string_array *da, char *str) {
    if (da->count < 1) da->data = (char **)malloc(LSTD_DSA_INITIAL_CAPACITY * sizeof(char*));
    if (da->count++ >= da->capacity) {
        da->data = (char **)realloc(da->data, (da->count + LSTD_DSA_INITIAL_CAPACITY) * sizeof(char*));
        da->capacity += LSTD_DSA_INITIAL_CAPACITY;
    }
    #ifdef LSTD_DEBUG
    printf("dsa_append: %s\n", str);
    #endif
    da->data[da->count-1] = str;
}

void lstd_dsa_append_many(lstd_dynamic_string_array *da, size_t amount, ...) {
    va_list args;
    va_start(args, amount);
    for (size_t i = 0; i < amount; ++i) {
        #ifdef LSTD_DEBUG
        printf("dsa_append_many: %lu\n", i);
        #endif
        lstd_dsa_append(da, va_arg(args, char *));
    }
    va_end(args);
}

void lstd_dsa_remove_at(lstd_dynamic_string_array *da, size_t n) {
    assert(n < da->count);
    lstd_dynamic_string_array new_da = {0};
    new_da.data = (char **)malloc((da->count-1) * sizeof(char*));
    for (size_t i = 0; i < da->count; ++i) {
        if (i == n) continue;
        lstd_dsa_append(&new_da, da->data[i]);
    }
    da = (lstd_dynamic_string_array*)memcpy(da, &new_da, sizeof(new_da));
    da->count--;
    lstd_dsa_free(&new_da);
}

void lstd_dsa_pop(lstd_dynamic_string_array *da) {
    assert(da->count > 0);
    da->count--;
}

void lstd_dsa_insert_at(lstd_dynamic_string_array *da, char *str, size_t n) {
    assert(n < da->count);
    lstd_dynamic_string_array new_da = {0};
    new_da.data = (char **)malloc((da->count+1) * sizeof(char*));
    for (size_t i = 0; i < da->count; ++i) {
        if (i == n) lstd_dsa_append(&new_da, str);
        lstd_dsa_append(&new_da, da->data[i]);
    }
    da = (lstd_dynamic_string_array*)memcpy(da, &new_da, sizeof(new_da));
    lstd_dsa_free(&new_da);
}

void lstd_dsa_free(lstd_dynamic_string_array *da) {
    if (da->data) free(da->data);
}

void lstd_dsa_print_data(lstd_dynamic_string_array *da) {
    if (da->count < 1) printf("\n");
    else {
        for (size_t i = 0; i < da->count; ++i) {
            printf("%s\n", da->data[i]);
        }
    }
}

// make macros to strip prefix if wanted

#ifdef LSTD_NOPREFIX
#define dynamic_string_array lstd_dynamic_string_array
#define dsa_append lstd_dsa_append
#define dsa_append_many lstd_dsa_append_many
#define dsa_pop lstd_dsa_pop 
#define dsa_insert_at lstd_dsa_insert_at
#define dsa_remove_at lstd_dsa_remove_at
#define dsa_free lstd_dsa_free
#define dsa_print_data lstd_dsa_print_data
#endif // LSTD_NOPREFIX
#endif // LSTD_DSA


// these arent done, they SUCK
#ifdef LSTD_MAP
#define LSTD_MAP_INITIAL_CAPACITY 256

typedef struct lstd_map {
    size_t count;
    size_t capacity;
    char **keys;
    void **items;
} lstd_map;

static int get_key_index(lstd_map *map, char *key) {
    if (!map || map->count < 1) return -1;
    for (size_t i = 0; i < map->count; ++i) {
        if (!map->keys[i]) continue;
        #ifdef LSTD_DEBUG
        printf("%s : %s\n", map->keys[i], key);
        #endif
        if (strcmp(map->keys[i], key) == 0) return i;
    }
    return -1;
}

static void new_key_item_pair(lstd_map *map, char *key, void *item) {
    if (map->count < 1) {
        map->keys = (char **)malloc(LSTD_MAP_INITIAL_CAPACITY * sizeof(char**));
        map->items = (void **)malloc(LSTD_MAP_INITIAL_CAPACITY * sizeof(void**));
    }
    if (map->count++ >= map->capacity) {
        map->keys = (char **)realloc(map->keys, (map->count + LSTD_MAP_INITIAL_CAPACITY) * sizeof(char**));
        map->capacity += LSTD_MAP_INITIAL_CAPACITY;
        map->items = (void **)realloc(map->items, (map->count + LSTD_MAP_INITIAL_CAPACITY) * sizeof(void**));
    }
    map->keys[map->count-1] = key;
    map->items[map->count-1] = item;
}

void lstd_map_set(lstd_map *map, char *key, void *item) {
    #ifdef LSTD_DEBUG
    printf("In map_set\n");
    #endif
    int key_index = get_key_index(map, key);
    if (key_index < 0) {
        new_key_item_pair(map, key, item);
    } else {
        map->items[key_index] = item;
    }
}

void *lstd_map_get(lstd_map *map, char *key) {
    #ifdef LSTD_DEBUG
    printf("In map_get\n");
    #endif
    int key_index = get_key_index(map, key);
    if (key_index < 0) return NULL;
    else return map->items[key_index];
}

void lstd_map_free(lstd_map *map) {
    if (map->keys) free(map->keys);
    if (map->items) free(map->items);
}

#ifdef LSTD_NOPREFIX
#define map lstd_map
#define map_set lstd_map_set
#define map_get lstd_map_get
#define map_free lstd_map_free
#endif // LSTD_NOPREFIX
#endif // LSTD_MAP

// fucky, doesnt really work
char *lstd_shift(char **xs) {
    if (!xs[*xs[0]]) return NULL;
    char *x = xs[*xs[0]];
    return x;
}

static size_t first_index_of_needle(char* str, char needle) {
    size_t i = 0;
    while (str[i] != 0 && str[i] != needle) ++i;
    return i;     
}

lstd_dynamic_string_array lstd_string_split(char *str, char needle) {
    lstd_dynamic_string_array ret = {0};
    char* str_cpy = str;
    size_t i = 0;
    size_t index;
    while (i < strlen(str)) {
        index = first_index_of_needle(str_cpy, needle);
        char* substr = malloc(index);
        strncpy(substr, str_cpy, index);
        lstd_dsa_append(&ret, substr);
        size_t len = strlen(substr) + 1;
        str_cpy += len;
        i += len;
    }
    return ret;
}

#ifdef LSTD_NOPREFIX
#define shift lstd_shift
#define string_split lstd_string_split
#endif // LSTD_NOPREFIX

#endif // LSTD