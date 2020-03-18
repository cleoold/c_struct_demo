#include <stdio.h>
#include <stdlib.h>
#include "darray.h"

typedef struct Test { int x; } Test;

int main(void) {
    Test *a = malloc(sizeof(Test)),
         *b = malloc(sizeof(Test)),
         *c = malloc(sizeof(Test));
    a->x = 1, b->x = 2, c->x = 300;

    Array_D *arr = Array_D_new();
    arr->pushback(arr, a);
    arr->pushback(arr, b);
    arr->pushback(arr, c);

    for (int i = 0; i < arr->length; i++)
        printf("%d\n", ((Test*)(arr->content[i]))->x);

    arr->free(arr, (Array_D_cb)free);
    arr = NULL;
}
