#ifndef RULE_H_
#define RULE_H_

#include "list.h"
#include <limits.h>

#define SEGMENT_SIZE	8

typedef struct {
	__u8 client_min_id, client_max_id;
	__u8 server_min_id, server_max_id;
	__u8 verdict;
	__u8 padding[3];
} rule_t;

__u8 unpack(__u64 p, int k);
item_t *lst_new_rule(unsigned char *bytes);

#endif
