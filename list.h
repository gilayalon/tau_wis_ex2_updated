#ifndef LIST_H_
#define LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/types.h>

typedef struct {
	void *data;
	item_t *prev;
	item_t *next;
} item_t;

typedef struct {
	item_t *head;
	item_t *tail;
} list_t;

list_t *lst_init();
item_t *lst_create(void *data);
void lst_add(list_t *list, item_t *item);
void lst_clear(item_t *head);
int lst_is_empty(list_t *list);

#endif

