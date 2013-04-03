#include "table.h"

table_t *tbl_init() {
	table_t *ct = (table_t *)malloc(sizeof(table_t));

	ct->max_size = HASH_SIZE;
	ct->curr_size = 0;
	ct->map = (list_t **)malloc(HASH_SIZE * sizeof(list_t *));

	return ct;
}

connection_t *tbl_get(table_t *ct, __u8 client, __u8 server) {
	int index = tbl_hash(tbl_gen_key(client, server));
	list_t *list = ct->map[index];

	return lst_find_con(list, client, server);
}

int tbl_put(table_t *ct, connection_t *c) {
	int index = tbl_hash(tbl_gen_key(c->client, c->server));

	if (ct->curr_size <= MAX_CONNECTIONS) {
		if (ct->map[index] == NULL) ct->map[index] = lst_init();
		lst_add(ct->map[index], lst_create(c));
		ct->curr_size++;

		return 0;
	}

	return 1;
}

void tbl_clear(table_t *ct) {
	int i;

	for (i = 0; i < ct->max_size; i++) {
		if (ct->map[i] != NULL)
			lst_clear(ct->map[i]->head);
	}

	free(ct->map);
}

char *tbl_gen_key(__u8 client, __u8 server) {
	char key[8];

	sprintf(key, "s%dd%d", client, server);
	return key;
}

int tbl_hash(char *key) {
	int c;
	unsigned int hash = 5831;

	while ((c = *key++))
	{
		hash = ((hash << 5) + hash) + c;
	}

	return (hash % HASH_SIZE);
}
