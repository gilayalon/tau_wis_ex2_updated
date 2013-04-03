#include "connection.h"

connection_t *lst_new_con(__u8 client, __u8 server, __u8 seq) {
	connection_t *c = (connection_t *)malloc(sizeof(connection_t));

	c->client = client;
	c->server = server;
	c->seq = seq;
	c->state = STATE_INIT;

	return c;
}

connection_t *lst_find_con(list_t *list, __u8 client, __u8 server) {
	item_t *head;
	connection_t *data;

	if (!lst_is_empty(list)) {
		head = list->head->next;
		data = (connection_t *)head->data;

		for (; head != list->tail; head = head->next) {
			if (data->client == client && data->server == server) return data;
		}
	}

	return NULL;
}
