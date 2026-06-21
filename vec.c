#include "vec.h"

void vec_init(Vec *v) {
    v->data     = NULL;
    v->size     = 0;
    v->capacity = 0;
}

int vec_push(Vec *v, void *item) {
    if (v->size == v->capacity) {
        size_t new_cap = v->capacity == 0 ? 4 : v->capacity * 2;
        void **tmp = realloc(v->data, new_cap * sizeof(void *));
        if (!tmp) return -1;
        v->data     = tmp;
        v->capacity = new_cap;
    }
    v->data[v->size++] = item;
    return 0;
}

void *vec_get(Vec *v, size_t i)  { return v->data[i]; }
size_t vec_len(Vec *v)           { return v->size; }

void vec_free(Vec *v) {
    free(v->data);
    vec_init(v);
}

void vec_remove(Vec *v, size_t i) {
    free(v->data[i]);
    memmove(&v->data[i], &v->data[i+1], (v->size - i - 1) * sizeof(void *));
    v->size--;
}
