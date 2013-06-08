#include <stdlib.h>
#include "thc.h"
#include "trie.h"

void test_trie_node_init (void)
{
    trie_node_t *trie;
    trie = trie_node_init ();
    ENSURE(NULL != trie);
    ENSURE(0 == trie->value);
    ENSURE(NULL != trie->children);

    for (int i = 0; i < ALPHABET_SIZE; i++)
        ENSURE (trie->children[i] == NULL);
}

void test_trie_find_value (void)
{
    trie_node_t **children = malloc (sizeof (trie_node_t) * ALPHABET_SIZE);
    const char *input = "abcdefghijklmnopqrstuvxwyz";

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        children[i] = malloc (sizeof (trie_node_t));
        children[i]->value = *(input++);
    }

    ENSURE('t' == _find_value (children, 't')->value);
    ENSURE('b' == _find_value (children, 'b')->value);
    ENSURE('z' == _find_value (children, 'z')->value);
    ENSURE('a' == _find_value (children, 'a')->value);
    ENSURE(NULL == _find_value (children, ')'));
    ENSURE(NULL == _find_value (children, ']'));
    ENSURE(NULL == _find_value (children, ' '));
}

void test_trie_add_word (void)
{
    trie_node_t *trie;
    trie = trie_node_init ();

    ENSURE(1 == trie_add_word (trie, "d"));
    ENSURE('d' == _find_value (trie->children, 'd')->value);
    ENSURE(1 == trie_add_word (trie, "z"));
    ENSURE('z' == _find_value (trie->children, 'z')->value);

    /* FIXME - leaking */
    trie = trie_node_init ();

    ENSURE(1 == trie_add_word (trie, "da"));

    /* make sure 'a' value insn't present in trie root children */
    ENSURE(NULL == _find_value (trie->children, 'a'));
}

void test_trie_has_word (void)
{
    trie_node_t *trie;
    trie = trie_node_init ();

    trie_add_word (trie, "d");
    trie_add_word (trie, "da");
    trie_add_word (trie, "asdf");
    trie_add_word (trie, "lkhkl");

    ENSURE(1 == trie_has_node (trie, "d"));
    ENSURE(1 == trie_has_node (trie, "da"));
    ENSURE(1 == trie_has_node (trie, "asdf"));
    ENSURE(1 == trie_has_node (trie, "lkhkl"));
    ENSURE(0 == trie_has_node (trie, "e"));
    ENSURE(0 == trie_has_node (trie, "dap"));
    ENSURE(0 == trie_has_node (trie, "asdfa"));
}
