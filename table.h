#ifndef TABLE_H_
#define TABLE_H_

#include "connection.h"

#define MAX_CONNECTIONS		120
#define HASH_SIZE			47

typedef struct {
	int max_size;
	int curr_size;
	list_t **map;
} table_t;

table_t *tbl_init();
connection_t *tbl_get(table_t *ct, __u8 client, __u8 server);
int tbl_put(table_t *ct, connection_t *c);
void tbl_clear(table_t *ct);
char *tbl_gen_key(__u8 client, __u8 server);
int tbl_hash(char *key);

#endif
