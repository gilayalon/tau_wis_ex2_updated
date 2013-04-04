#include "firewall.h"

int fw_init(char *filename) {
	int i;
	int fdin;
	char *src;
	struct stat statbuf;
	unsigned char bytes[SEGMENT_SIZE];

	rules = lst_init();
	i = lst_is_empty(rules);
	connections = tbl_init();

	fdin = open(filename, O_RDONLY);
	fstat(fdin, &statbuf);
	src = mmap(0, statbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0);

	for (i = 0; i < statbuf.st_size; i+=SEGMENT_SIZE) {
		memcpy(bytes, &src[i], SEGMENT_SIZE);
		if (bytes[4] != 0)
			lst_add(rules, lst_new_rule(bytes));
	}

	close(fdin);

	return 0;
}

int fw_check_id(int client, int server) {
	item_t *head;
	rule_t *data;

	if (client < 0 || client > 255 || server < 0 || server > 255 || client == server)
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

int fw_check_con(int client, int server, int seq, cmd_t cmd) {
	connection_t *c = tbl_get(connections, client, server);
	connection_t *_c = tbl_get(connections, server, client);
	output[2] = "D";

	if ((c == NULL && _c == NULL && cmd != CMD_CLIENT_HELLO) || /* אין קשר ולא מתחילים בהלו */
			(c != NULL && (cmd == CMD_SERVER_HELLO || cmd == CMD_SERVER_MARKET || cmd == CMD_SERVER_BYE)) || /* יש קשר בו אני הקליינט ועכשיו יש פקטה בה אני הסרבר */
			(_c != NULL && (cmd == CMD_CLIENT_HELLO || cmd == CMD_CLIENT_SUPER || cmd == CMD_CLIENT_BYE))) { /* יש קשר בו אני הסרבר ועכשיו יש פקטה בה אני הקליינט */
		return REASON_OUT_OF_CONNECTION; /* הפקטה לא חלק מקשר קיים */

	} else if (c == NULL && cmd == CMD_CLIENT_HELLO) { /* לא היה קשר ביניהם ועכשיו מתחילים */
		if (seq < 0 || seq > 255) {
			return REASON_BAD_SEQUENCE_NUM;
		} else {
			if (tbl_put(connections, lst_new_con(client, server, seq))) { /* יש כבר 120 שיחות בטבלה */
				printf("here2\n");
				return REASON_OUT_OF_CONNECTION;
			} else {
				output[2] = "A";
				return REASON_PACKET_OK;
			}
		}
	} else {
		if (c != NULL) {
			if ((c->state == STATE_IDLE && cmd != CMD_CLIENT_SUPER && cmd != CMD_CLIENT_BYE) ||
					(c->state == STATE_SERVER_BYE && cmd != CMD_CLIENT_BYE) ||
					(c->state == STATE_END || c->state == STATE_BAD)) {
				return REASON_OUT_OF_STATE;
			}
			if (seq != ((c->seq + 1) % 256)) {
				return REASON_BAD_SEQUENCE_NUM;
			}
			c->seq = (c->seq + 1) % 256;
			if (c->state == STATE_IDLE && cmd == CMD_CLIENT_BYE)
				c->state += 2;
			c->state += 1;
			output[2] = "A";
			return REASON_PACKET_OK;
		} else if (_c != NULL) {
			if ((_c->state == STATE_INIT && cmd != CMD_SERVER_HELLO) ||
					(_c->state == STATE_IDLE && cmd != CMD_SERVER_BYE) ||
					(_c->state == STATE_SUPER && cmd != CMD_SERVER_MARKET) ||
					(_c->state == STATE_CLIENT_BYE && cmd != CMD_SERVER_BYE) ||
					(_c->state == STATE_END || _c->state == STATE_BAD)) {
				printf("_c->state = %d, cmd = %d\n", _c->state, cmd);
				return REASON_OUT_OF_STATE;
			}
			if (seq != ((_c->seq + 1) % 256)) {
				return REASON_BAD_SEQUENCE_NUM;
			}
			_c->seq = (_c->seq + 1) % 256;
			if (_c->state == STATE_IDLE && cmd == CMD_SERVER_BYE)
				_c->state += 3;
			_c->state += 1;
			output[2] = "A";
			return REASON_PACKET_OK;
		}

	}
	return REASON_PACKET_OK;
}

void fw_inspect(char *packet) {
	int i;
	char *chunks[8];

	cmd_t cmd;
	__u8 client, server, seq;

	chunks[0] = (trim(strtok(packet, " \t\v\f\r")));
	for (i = 1; i < 8; i++) {
		chunks[i] = (trim(strtok(NULL, " \t\v\f\r")));
	}

	output[0] = chunks[1];
	output[1] = chunks[3];

	client = strToInt(chunks[1]);
	server = strToInt(chunks[3]);

	if (fw_check_id(client, server)){
		output[2] = "D";
		reason = REASON_DISALLOWED_ID;
	} else {
		cmd = fw_check_cmd(chunks[7]);
		if (cmd == CMD_BAD) {
			output[2] = "D";
			reason = REASON_BAD_COMMAND;
		} else {
			seq = strToInt(chunks[5]);
			reason = fw_check_con(client, server, seq, cmd);
		}
	}
}

int fw_shutdown() {
	lst_clear(rules->head);
	free(rules);
	tbl_clear(connections);
	free(connections);
	return 0;
}
