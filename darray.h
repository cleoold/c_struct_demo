#pragma once

typedef void (*Array_D_cb)(void*); 

/* normal dynamic array */
typedef struct Array_D {
    void **content;
    int length;
    struct Array_D *(*pushback)(struct Array_D*, void*);
    void (*free)(struct Array_D*, Array_D_cb);
} Array_D;

Array_D *Array_D_new();
