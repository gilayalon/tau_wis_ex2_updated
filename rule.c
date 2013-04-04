#include "rule.h"
#include <stdio.h>

__u8 unpack(__u64 p, int k) {
	unsigned  mask = 0xFF;
	__u64 n = k * CHAR_BIT;
	__u8 result;

	mask <<= n;
	result = ((p & mask)>>n);
	return result;
}

item_t *lst_new_rule(unsigned char *bytes) {
	rule_t *data;
	item_t *result;

	data = (rule_t *)malloc(sizeof(rule_t));

	data->client_min_id = bytes[0];
	data->client_max_id = bytes[1];
	data->server_min_id = bytes[2];
	data->server_max_id = bytes[3];
	data->verdict = bytes[4];
	data->padding[0] = bytes[5];
	data->padding[1] = bytes[6];
	data->padding[2] = bytes[7];

	result = lst_create(data);
	return result;
}
