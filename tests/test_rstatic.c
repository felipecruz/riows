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
    char input6[] = "fontello.ttf?61924141";

    ENSURE (0 == strcmp ("png", extension (input1)));
    ENSURE (0 == strcmp ("json", extension (input2)));
    ENSURE (0 == strcmp ("woff?632182", extension (input3)));
    ENSURE (0 == strcmp ("js", extension (input4)));
    ENSURE (0 == strcmp ("woff?632.182", extension (input5)));
    ENSURE (0 == strcmp ("ttf?61924141", extension (input6)));
}

void test_extract_query_string (void)
{
    char input1[] = "/data/?type=json";
    char input2[] = "/info.json";
    char input3[] = "/image.woff?632182";
    char input4[] = "/image.ttf?632.182";

    char *out1 = extract_query_string (input1);
    char *out2 = extract_query_string (input2);
    char *out3 = extract_query_string (input3);
    char *out4 = extract_query_string (input4);

    ENSURE (0 == strcmp ("type=json", out1));
    ENSURE (NULL == out2);
    ENSURE (0 == strcmp ("632182", out3));
    ENSURE (0 == strcmp ("632.182", out4));

    ENSURE (0 == strcmp ("/data/", input1));
    ENSURE (0 == strcmp ("/info.json", input2));
    ENSURE (0 == strcmp ("/image.woff", input3));
    ENSURE (0 == strcmp ("/image.ttf", input4));

    free (out1); free (out2); free (out3); free (out4);
}
