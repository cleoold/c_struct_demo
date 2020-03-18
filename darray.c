#include <stdlib.h>
#include "darray.h"

static void Array_D_free();
static Array_D *Array_D_pushback();

Array_D *Array_D_new() {
    Array_D *res;

    if ((res = malloc(sizeof(Array_D))) == NULL)
        return NULL;
    res->content = NULL;
    res->length = 0;
    res->pushback = Array_D_pushback;
    res->free = Array_D_free;
    return res;
}

static void Array_D_free(Array_D *arry, Array_D_cb callback) {
    register int i;
    for (i = 0; i < arry->length; ++i)
        callback(arry->content[i]);
    free(arry->content);
    free(arry);
}

static Array_D *Array_D_pushback(Array_D *arry, void *item) {
    if ((arry->content = realloc(
        arry->content, sizeof item * (arry->length+1))) == NULL)
        return NULL;
    arry->content[arry->length] = item;
    ++arry->length;
    return arry;
}
