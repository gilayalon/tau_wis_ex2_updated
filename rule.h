#ifndef RULE_H_
#define RULE_H_

#include "list.h"
#include "firewall.h"
#include <limits.h>

#define SEGMENT_SIZE	8

__u8 unpack(__u64 p, int k);
item_t *lst_new_rule(unsigned char *bytes);

#endif
