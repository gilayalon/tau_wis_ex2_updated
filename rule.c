#include "rule.h"

__u8 *unpack(__u64 p, int k) {
	unsigned  mask = 0xFF;
	__u64 n = k * CHAR_BIT;

    mask <<= n;
	return &( ( p & mask ) >> n );
}

rule_t *lst_new_rule(__u64 segment) {
	rule_t *result;

	result = (rule_t *)malloc(sizeof(rule_t));

	result->client_min_id = *unpack(segment, 0);
	result->client_max_id = *unpack(segment, 1);
	result->server_min_id = *unpack(segment, 2);
	result->server_max_id = *unpack(segment, 3);
	result->verdict = *unpack(segment, 4);
	result->padding = unpack(segment, 5);

	return result;
}
