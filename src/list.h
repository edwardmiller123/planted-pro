#ifndef LIST_H
#define LIST_H

#include <stdint.h>

typedef struct node {
	uint32_t data;
	struct node * next;
	struct node * prev;
} node;

typedef struct list {
	uint32_t size;
	node * head;
} list;

void init_list(list *l);

// Adds a new node to the list.
void list_add(list *l, node *n);

void list_remove(list *l, node *n);

#endif

