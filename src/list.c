#include <stdint.h>

#include "list.h"
#include "utils.h"
#include "logger.h"

void init_list(list *l)
{
	l->size = 0;
	l->head = NULL;
}

void list_add(list *l, node *n)
{
	// The most recent node is added just before the head and then becomes the new head
	n->next = l->head;
	if (l->head == NULL)
	{
		n->prev = l->head;
	}
	else
	{
		n->prev = l->head->prev;
	}
	l->head = n;
	l->size++;
}

void list_remove(list *l, node *n)
{
	if (l->size == 0)
	{
		logger(WARNING, "Cant remove node from empty list");
		return;
	}

	n->next->prev = n->prev;
	n->prev->next = n->next;

	if (n == l->head)
	{
		l->head = l->head->next;
	}

	if (l->size == 1)
	{
		l->head = NULL;
	}

	l->size--;
}