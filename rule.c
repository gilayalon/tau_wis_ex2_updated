#include "rule.h"

__u8 unpack(__u64 p, int k) {
	unsigned  mask = 0xFF;
	__u64 n = k * CHAR_BIT;
	__u8 result;

    mask <<= n;
    result = ((p & mask)>>n);
	return result;
}

item_t *lst_new_rule(__u64 segment) {
	rule_t *data;
	item_t *result;

	data = (rule_t *)malloc(sizeof(rule_t));

	data->client_min_id = unpack(segment, 0);
	data->client_max_id = unpack(segment, 1);
	data->server_min_id = unpack(segment, 2);
	data->server_max_id = unpack(segment, 3);
	data->verdict = unpack(segment, 4);
	data->padding[1] = unpack(segment, 5);
	data->padding[2] = unpack(segment, 6);
	data->padding[3] = unpack(segment, 7);

	result = lst_create(data);
	return result;
}
