#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef H_LIST
#define H_LIST
typedef struct list_node_s list_node_t;

struct list_node_s {
    void *data;
    size_t *length;
    struct list_node_s *next;
    struct list_node_s *prev;
};

typedef struct {
    list_node_t *head;
    list_node_t *tail;
    int length;
} list;

#define LIST_REVERSE(list, element)                                             \
    for (list_node_t *element = list->head; element; element = element->next)   \

#define LIST_TRAVERSE(list, element)                                            \
    for (list_node_t *element = list->tail; element; element = element->prev)   \

list *list_init (void);
void list_destroy (list **l);
void node_destroy (list_node_t* n);
list_node_t *node_pop (list *l);
list_node_t *list_push_node (list* l, void* data);
#endif
