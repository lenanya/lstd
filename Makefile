all: test

test: test.c lstd.h
	cc test.c -o test -ggdb -O0