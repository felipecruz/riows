#include "trie.h"

trie_node_t* trie_node_init ()
{
    trie_node_t* trie;
    trie = malloc (sizeof (trie_node_t));
    if (trie == NULL)
        return NULL;
    trie->children = malloc (sizeof (trie_node_t*) * ALPHABET_SIZE);
    if (trie->children == NULL) {
        free (trie);
        return NULL;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        trie->children[i] = NULL;
    }
    trie->value = 0;
    return trie;
}

trie_node_t* _find_value (trie_node_t **children, const char value)
{
    if (children == NULL || !VALID_(value))
        return NULL;

    if (children[NORMALIZE_(value)] != NULL)
        return children[NORMALIZE_(value)];

    return NULL;
}

int trie_add_word (trie_node_t *trie, const char* word)
{
    while (*word) {
        trie_node_t *found = _find_value (trie->children, *(word));
        if (found) {
            trie = found;
        } else {
            trie->children[NORMALIZE_(*(word))] = trie_node_init ();
            trie->children[NORMALIZE_(*(word))]->value = *(word);
            trie = trie->children[NORMALIZE_(*word)];
        }
        *(word++);
    }
    return 1;
}

int trie_has_node (trie_node_t *trie, const char* word)
{
    while (*word) {
        trie_node_t *found = _find_value (trie->children, *(word));
        if (found) {
            trie = found;
        } else {
            return 0;
        }
        *(word++);
    }
    return 1;
}
