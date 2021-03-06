#include "list.h"

list_t *lst_init() {
	list_t *list = (list_t *)malloc(sizeof(list_t));

	list->head = lst_create(NULL);
	list->tail = lst_create(NULL);

	list->head->next = list->tail;
	list->tail->prev = list->head;

	return list;
}

item_t *lst_create(void *data) {
	item_t *item = (item_t *)malloc(sizeof(item_t));

	item->data = data;
	item->next = NULL;
	item->prev = NULL;

	return item;
}

void lst_add(list_t *list, item_t *item) {
	item->next = list->head->next;
	item->prev = list->head;
	list->head->next->prev = item;
	list->head->next = item;
}

void lst_clear(item_t *head) {
	if (head != NULL) {
		lst_clear(head->next);
		free(head->data);
		free(head);
	}
}

int lst_is_empty(list_t *list) {
	int result = 0;
	if (list->head->next == NULL)
		result = 1;
	return result;
}
