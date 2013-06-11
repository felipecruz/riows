#include <rstatic.h>
#include "thc.h"
#include "test_rstatic.h"


void test_extension (void)
{
    char input1[] = "image.png";
    char input2[] = "info.json";
    char input3[] = "image.woff?632182";
    char input4[] = "jquery.2.0.1.min.js";
    char input5[] = "image.woff?632.182";

    ENSURE (0 == strcmp ("png", extension (input1)));
    ENSURE (0 == strcmp ("json", extension (input2)));
    ENSURE (0 == strcmp ("woff?632182", extension (input3)));
    ENSURE (0 == strcmp ("js", extension (input4)));
    ENSURE (0 == strcmp ("woff?632.182", extension (input5)));
}

void test_extract_query_string (void)
{
    char input1[] = "/data/?type=json";
    char input2[] = "/info.json";
    char input3[] = "/image.woff?632182";
    char input4[] = "/image.woff?632.182";

    ENSURE (0 == strcmp ("type=json", extract_query_string (input1)));
    ENSURE (NULL == extract_query_string (input2));
    ENSURE (0 == strcmp ("632182", extract_query_string (input3)));
    ENSURE (0 == strcmp ("632.182", extract_query_string (input4)));
}
