#include "list.h"

list *list_init (void)
{
    list *l;

    l = malloc (sizeof (list));
    l->head = NULL;
    l->tail = NULL;
    l->length = 0;

    return l;
}

void list_destroy (list **l)
{
    list *lp = *l;
    list_node_t *current = node_pop (lp);

    while (current) {
        node_destroy (current);
        current = node_pop (lp);
    }

    lp->head = NULL;
    lp->tail = NULL;
    lp->length = 0;

    free (lp);
    *(l) = NULL;
}

void node_destroy (list_node_t* n)
{
    free (n->data);
    free (n);
}

/* node_pop returns the head and remove it from the list */
list_node_t *node_pop (list *l)
{
    list_node_t *poped = l->head;

    /* if the old head is NULL, the list is empty return NULL */
    if (poped == NULL)
        return NULL;

    /* if the poped element is the tail, the list is empty  */
    /* and tail should be NULL */
    if (l->tail == poped)
        l->tail = NULL;

    /* new head points to old head next */
    l->head = poped->next;
    
    /* if head is null, the list has no elements */
    /* if it's not null, set the new head prev to null */
    if (l->head != NULL)
        l->head->prev = NULL;

    l->length--;

    return poped;
}

list_node_t *list_push_node (list* l, void* data)
{
    list_node_t *n = malloc (sizeof (list_node_t));
    
    n->data = data;
    n->next = l->head;
    n->prev = NULL;

    if (l->head != NULL)
        l->head->prev = n;

    if (l->tail == NULL)
        l->tail = n;

    l->head = n;
    l->length++;

    return n;
}
