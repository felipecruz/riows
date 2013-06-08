#include "hash.h"

/*
 * 3 small hash functions
 * http://www.cse.yorku.ca/~oz/hash.html
 *
 */

/* sdbm hash function */

unsigned long hash_val (uint8_t *str)
{
    unsigned long _hash = 0;
    int c;

    while (c = *str++)
        _hash = c + (_hash << 6) + (_hash << 16) - _hash;

    return _hash;
}

hash *hash_init (size_t length)
{
    hash *table = malloc (sizeof (hash));
    if (table == NULL)
        return NULL;

    table->elements_length = 0;
    table->hash_len = 0;
    table->capacity = length;

    table->elements = malloc (sizeof (hash_elem_t*) * length);
    if (table->elements == NULL) {
        free (table);
        return NULL;
    }

    for (int i = 0; i < table->capacity; i++) {
        table->elements[i] = NULL;
    }

    return table;
}

void hash_destroy (hash **table)
{
    if ((*table) == NULL)
        return;

    for (int i = 0; i < (*table)->capacity; i++) {
        hash_elem_t *prev;
        hash_elem_t *el = (*table)->elements[i];
        while (el) {
            prev = el;
            el = el->next;
            free (prev->value);
            free (prev);
        }
    }

    free ((*table)->elements);
    free (*table);
    *table = NULL;
}

void hash_put (hash *table, uint8_t *key, void *data, size_t length)
{
    if (table == NULL || key == NULL || data == NULL || length == 0)
        return;

    hash_elem_t *el;
    unsigned long _key = hash_val (key);
    size_t index = _key % table->capacity;

    if (table->elements[index] == NULL) {
        el = malloc (sizeof (hash_elem_t));
        el->key = key;
        el->value = data;
        el->value_len = length;
        el->next = NULL;
        table->elements[index] = el;
    } else { /* handle colision */
        hash_elem_t *prev;
        hash_elem_t *linked = table->elements[index];
        while (linked) {
            if (match(linked->key, key, strlen (key))) { /* element found */
                free (linked->value);
                linked->value = data;
                linked->value_len = length;
                return;
            }
            prev = linked;
            linked = linked->next;
        }
        /* add at the end of collision list */
        el = malloc (sizeof (hash_elem_t));
        el->key = key;
        el->value = data;
        el->value_len = length;
        el->next = NULL;
        prev->next = el;
    }
}

hash_elem_t *hash_get (hash *table, uint8_t *key)
{
    if (table == NULL || key == NULL)
        return NULL;

    unsigned long _key = hash_val (key);
    size_t index = _key % table->capacity;
    hash_elem_t *el = table->elements[index];

    if (el != NULL && match(el->key, key, strlen (key)))
        return el;

    while (el) {
        if (match(el->key, key, strlen (key)))
            return el;
        el = el->next;
    }

    return NULL;
}

void hash_del (hash *table, uint8_t *key)
{
    if (table == NULL || key == NULL)
        return;

    unsigned long _key = hash_val (key);
    size_t index = _key % table->capacity;
    hash_elem_t *el = table->elements[index];
    hash_elem_t *prev;

    if (el == NULL)
        return;

    if (el != NULL && match(el->key, key, strlen (key))) {
        table->elements[index] = el->next;
        free (el->value);
        free (el);
        return;
    }

    while (el) {
        if (match(el->key, key, strlen (key))) {
            prev->next = el->next;
            free (el->value);
            free (el);
        }
        prev = el;
        el = el->next;
    }
}
