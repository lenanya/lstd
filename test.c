#define LSTD_NOPREFIX
#define LSTD_DSA
//#define LSTD_DEBUG
#include "lstd.h"

int main(int argc, char **argv) {
   
    char *org = "test,one,two";

    dynamic_string_array test = string_split(org, ',');

    dsa_print_data(&test);

    dsa_free(&test);

    return 0;
}