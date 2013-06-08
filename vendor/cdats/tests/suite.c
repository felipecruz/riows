#include "thc.h"
#include "test_list.h"
#include "test_hash.h"
#include "test_trie.h"

int main (int argc, const char *argv[])
{
    int _tlist = 0;
    int _thash = 0;
    int _ttrie = 0;
#ifdef ALL
    _tlist = 1;
    _thash = 1;
    _ttrie = 1;
#endif

#ifdef tlist
    _tlist = 1;
#endif

#ifdef thash
    _thash = 1;
#endif

#ifdef ttrie
    _ttrie = 1;
#endif

    if (_tlist) {
        /* list tests */
        thc_addtest (test_list_init_destroy); 
        thc_addtest (test_list_push_node); 
        thc_addtest (test_list_node_pop); 
        thc_addtest (test_list_traverse_reverse_macro); 
    } 

    if (_thash) {
        /* hash tests */
        thc_addtest (test_hash_init_destroy); 
        thc_addtest (test_hash_put_get);
        thc_addtest (test_hash_put_collision);
        thc_addtest (test_hash_del);
    }
    
    if (_ttrie) {
        thc_addtest (test_trie_node_init);
        thc_addtest (test_trie_find_value);
        thc_addtest (test_trie_add_word);
        thc_addtest (test_trie_has_word);
    }

    return thc_run (THC_VERBOSE);
}
