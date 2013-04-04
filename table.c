#include "table.h"

table_t *tbl_init() {
	int i;
	table_t *ct = (table_t *)malloc(sizeof(table_t));

	ct->max_size = HASH_SIZE;
	ct->curr_size = 0;
	ct->map = (list_t **)malloc(HASH_SIZE * sizeof(list_t *));
	for (i = 0; i < HASH_SIZE; i++)
		ct->map[i] = lst_init();
	return ct;
}

connection_t *tbl_get(table_t *ct, __u8 client, __u8 server) {
	int index = tbl_hash(client, server);
	list_t *list = ct->map[index];

	return lst_find_con(list, client, server);
}

int tbl_put(table_t *ct, connection_t *c) {
	int index = tbl_hash(c->client, c->server);

	if (ct->curr_size <= MAX_CONNECTIONS) {
		lst_add(ct->map[index], lst_create(c));
		ct->curr_size++;

		return 0;
	}

	return 1;
}

void tbl_clear(table_t *ct) {
	int i;

	for (i = 0; i < ct->max_size; i++) {
		if (ct->map[i] != NULL) {
			lst_clear(ct->map[i]->head);
			free(ct->map[i]);
		}
	}

	free(ct->map);
}

int tbl_hash(__u8 client, __u8 server) {
	int c, i;
	unsigned int hash = 5831;
	char key[8];

	sprintf(key, "s%dd%d", client, server);
	for (i = 0; i < 8; i++)
	{
		c = key[i];
		hash = ((hash << 5) + hash) + c;
	}

	return (hash % HASH_SIZE);
}
