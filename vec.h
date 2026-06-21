#ifndef VEC_H
#define VEC_H

#include <stdlib.h>
#include <string.h>

typedef struct {
    void   **data;
    size_t   size;
    size_t   capacity;
} Vec;

void vec_init(Vec *v);
int vec_push(Vec *v, void *item);
void *vec_get(Vec *v, size_t i);
size_t vec_len(Vec *v);
void vec_free(Vec *v);
void vec_remove(Vec *v, size_t i);

#endif
