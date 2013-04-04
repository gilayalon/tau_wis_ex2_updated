#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "list.h"

typedef enum {
	STATE_INIT = 0,
			STATE_IDLE,
			STATE_SUPER,
			STATE_CLIENT_BYE,
			STATE_SERVER_BYE,
			STATE_END,
			STATE_BAD
} state_t;

typedef struct {
	__u8 client, server, seq;
	state_t state;
} connection_t;


connection_t *lst_new_con(__u8 client, __u8 server, __u8 seq);
connection_t *lst_find_con(list_t *list, __u8 client, __u8 server);

#endif
