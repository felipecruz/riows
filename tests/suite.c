#include "thc.h"
#include "test_rioev.h"
#include "test_rstatic.h"

int main (int argc, const char *argv[])
{
    int _rioev = 0;
    int _rstatic = 0;
#ifdef ALL
    _rioev = 1;
    _rstatic = 1;
#endif

#ifdef trioev
    _rioev = 1;
#endif

#ifdef trstatic
    _rstatic = 1;
#endif

    if (_rioev) {
        thc_addtest (test_rioev_init_destroy);
        thc_addtest (test_rioev_add_del_mod);
        thc_addtest (test_rioev_poll);
    }
    if (_rstatic) {
        thc_addtest (test_extension);
        thc_addtest (test_extract_query_string);
    }

    return thc_run (THC_VERBOSE);
}
