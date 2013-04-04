#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "list.h"
#include "firewall.h"

typedef struct {
	__u8 client, server, seq;
	state_t state;
} connection_t;


connection_t *lst_new_con(__u8 client, __u8 server, __u8 seq);
connection_t *lst_find_con(list_t *list, __u8 client, __u8 server);

#endif
