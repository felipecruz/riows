#include "thc.h"
#include "test_rioev.h"

int main (int argc, const char *argv[])
{
    int _rioev = 0;
#ifdef ALL
    _rioev = 1;
#endif

#ifdef trioev
    _rioev = 1;
#endif

    if (_rioev) {
        thc_addtest (test_rioev_init); 
        thc_addtest (test_rioev_add_del_mod); 
        thc_addtest (test_rioev_poll); 
    } 

    return thc_run (THC_VERBOSE);
}
