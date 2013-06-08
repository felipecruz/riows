#include <stdlib.h>

#ifndef H_TRIE
#define H_TRIE
typedef struct trie_node_s trie_node_t;

struct trie_node_s {
    char value;
    trie_node_t **children;
};

#define ALPHABET_SIZE 26
#define NORMALIZE_(l) (l - 97)
#define VALID_(l) (l > 96 && l < 123)

trie_node_t* trie_node_init (void);
trie_node_t* _find_value (trie_node_t **children, const char value);
int trie_add_word (trie_node_t *trie, const char* word);
int trie_has_node (trie_node_t *trie, const char* word);
#endif
