#include <string.h>
#include "thc.h"
#include "list.h"

void test_list_init_destroy (void)
{
    list* l;
    l = list_init ();
    ENSURE (l != NULL);
    list_destroy (&l);
    ENSURE (l == NULL);
}

void test_list_push_node (void)
{
    list *l;
    list_node_t *n, *n1, *n2;

    l = list_init ();

    char *data = strdup ("Felipe");
    n = list_push_node (l, (void*) data);
    ENSURE(l->head ==  n);
    ENSURE(l->length == 1);
    ENSURE(l->tail == n);

    char *data2 = strdup ("João");
    n1 = list_push_node (l, (void*) data2);
    ENSURE(l->head ==  n1);
    ENSURE(l->length == 2);
    ENSURE(l->tail == n);

    char *data3 = strdup ("Tester");
    n2 = list_push_node (l, (void*) data3);
    ENSURE(l->head ==  n2);
    ENSURE(l->length == 3);
    ENSURE(l->tail == n);

    list_destroy (&l);
    ENSURE(l == NULL);
}

void test_list_node_pop (void)
{
    list *l;
    list_node_t *n, *n1, *n2;

    l = list_init ();

    char *data = strdup ("Felipe");
    n = list_push_node (l, (void*) data);

    char *data2 = strdup ("João");
    n1 = list_push_node (l, (void*) data2);

    char *data3 = strdup ("Tester");
    n2 = list_push_node (l, (void*) data3);

    n2 = node_pop (l);
    ENSURE(memcmp ("Tester", n2->data, 6) == 0);
    node_destroy (n2);

    n1 = node_pop (l);
    ENSURE(memcmp ("João", n1->data, 5) == 0);
    node_destroy (n1);

    n = node_pop (l);
    ENSURE(memcmp ("Felipe", n->data, 6) == 0);
    node_destroy(n);

    n = node_pop (l);
    ENSURE(n == NULL);

    n = list_push_node (l, (void*) strdup ("Tester"));
    n1 = list_push_node (l, (void*) strdup ("João"));
    n2 = list_push_node (l, (void*) strdup ("Felipe"));

    while ((n = node_pop (l))) {
        ENSURE(n != NULL);
        node_destroy (n);
    }

    ENSURE(n == NULL);

    list_destroy (&l);
    ENSURE(l == NULL);
}

void test_list_traverse_reverse_macro (void)
{
    int count = 0;
    list *l = list_init ();

    char *data = strdup ("Felipe");
    list_node_t* n = list_push_node (l, (void*) data);

    char *data2 = strdup ("João");
    list_node_t* n1 = list_push_node (l, (void*) data2);

    char *data3 = strdup ("Tester");
    list_node_t* n2 = list_push_node (l, (void*) data3);

    LIST_TRAVERSE(l, el)
    {
        if (count == 0)
            ENSURE(memcmp ("Felipe", el->data, 6) == 0);
        count++;
    }

    ENSURE (3 == count);

    count = 0;
    LIST_REVERSE(l, el)
    {
        if (count == 0)
            ENSURE(memcmp ("Tester", el->data, 6) == 0);
        count++;
    }

    ENSURE (3 == count);
    list_destroy (&l);
}
