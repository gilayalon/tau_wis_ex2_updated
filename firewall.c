#include "firewall.h"

int fw_init(char *filename) {
	int i, j;
	int fdin;
	char *src;
	__u64 segment;
	struct stat statbuf;
	char bytes[SEGMENT_SIZE];

	rules = lst_init();
	connections = tbl_init();

	fdin = open(filename, O_RDONLY);
	fstat(fdin, &statbuf);

	src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0);
	for (i = 0; i < statbuf.st_size; i+=SEGMENT_SIZE) {
		memcpy(bytes, src, SEGMENT_SIZE);

		for (j = 0; j < SEGMENT_SIZE; j++) {
			segment |= (int)bytes[j] << (j * SEGMENT_SIZE);
		}

		lst_add(rules, lst_new_rule(segment));
	}

	close(fdin);

	return 0;
}

int fw_check_id(__u8 client, __u8 server) {
	item_t *head;
	rule_t *data;

	if (client < 0 || client > 255 || server < 0 || server > 255)
		return 1;

	if (!lst_is_empty(rules)) {
		head = rules->head->next;
		data = (rule_t *)head->data;

		for ( ; head != rules->tail; head = head->next) {
			if (client >= data->client_min_id && client <= data->client_max_id
					&& server >= data->server_min_id && server <= data->server_max_id) {
				if ((char)data->verdict == 'A') {
					return 0;
				} else {
					return 1;
				}
			}
		}
	}

	return 1;
}

int fw_check_cmd(char *cmd) {
	int i;

	for (i = 0; i < FW_CMDS_SIZE; i++) {
		if (strcmp(cmd, FW_CMDS[i]) == 0)
			return i;
	}

	return CMD_BAD;
}

int fw_check_con(__u8 client, __u8 server, __u8 seq, cmd_t cmd) {
	connection_t *c = tbl_get(connections, client, server);
	connection_t *_c = tbl_get(connections, server, client);

	if (c == NULL) {
		if (cmd == CMD_CLIENT_HELLO) {
			if (_c != NULL && c->state == STATE_END) {
				return REASON_OUT_OF_CONNECTION;
			} else {
				if (seq == -1 || seq < 0 || seq > 255) {
					return REASON_BAD_SEQUENCE_NUM;
				} else {
					if (tbl_put(connections, lst_new_con(client, server, seq))) {
						return REASON_OUT_OF_CONNECTION;
					} else {
						return REASON_PACKET_OK;
					}
				}
			}
		} else {
			return REASON_OUT_OF_CONNECTION;
		}
	} else {
		if ((c->state == STATE_INIT && cmd != CMD_SERVER_HELLO) ||
				(c->state == STATE_IDLE && cmd != CMD_CLIENT_SUPER) ||
				(c->state == STATE_SUPER && cmd != CMD_SERVER_MARKET) ||
				(c->state == STATE_CLIENT_BYE && cmd != CMD_SERVER_BYE) ||
				(c->state == STATE_SERVER_BYE && cmd != CMD_CLIENT_BYE) ||
				(c->state == STATE_END || c->state == STATE_BAD)) {
			return REASON_OUT_OF_STATE;
		}

		if (seq != ((c->seq + 1) % 256)) {
			return REASON_BAD_SEQUENCE_NUM;
		} else {
			c->seq = (c->seq + 1) % 256;
			c->state++;

			return REASON_PACKET_OK;
		}
	}
}

int fw_inspect(char *packet) {
	int i;
	char chunks[8];

	cmd_t cmd;
	__u8 client, server, seq;

	chunks[0] = trim(strtok(packet, " \t\v\f\r"));
	for (i = 1; i < 8; i++) {
		chunks[i] = trim(strtok(NULL, " \t\v\f\r"));
	}

	client = strToInt(chunks[1]);
	server = strToInt(chunks[3]);

	if (fw_check_id(client, server) || client == server){
		return REASON_DISALLOWED_ID;
	} else {
		cmd = fw_check_cmd(chunks[7]);
		if (cmd == CMD_BAD) {
			return REASON_BAD_COMMAND;
		} else {
			seq = strToInt(chunks[5]);
			return fw_check_con(client, server, seq, cmd);
		}
	}
}
