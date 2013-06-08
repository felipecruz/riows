#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifndef H_HASH
#define H_HASH
typedef struct hash_table hash;
typedef struct hash_elem_s hash_elem_t;

struct hash_elem_s {
    int key;
    void *value;
    size_t value_len;
    hash_elem_t *next;
};

struct hash_table {
    hash_elem_t **elements;
    size_t elements_length;
    size_t hash_len;
    size_t capacity;
};

#define match(key1, key2, size) (0 == memcmp (key1, key2, size))

hash *hash_init (size_t);
void hash_destroy (hash **hash);
void hash_put (hash *table, int key, void *data, size_t length);
hash_elem_t *hash_get (hash *table, int key);
void hash_del (hash *table, int key);
#endif
