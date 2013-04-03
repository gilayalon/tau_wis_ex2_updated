#ifndef FIREWALL_H_
#define FIREWALL_H_

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "rule.h"
#include "table.h"
#include "helpers.h"

#define FW_CMDS_SIZE 	6
extern const char *FW_CMDS[];

typedef enum {
	CMD_CLIENT_HELLO = 0,
	CMD_SERVER_HELLO,
	CMD_CLIENT_SUPER,
	CMD_SERVER_MARKET,
	CMD_CLIENT_BYE,
	CMD_SERVER_BYE,
	CMD_BAD
} cmd_t;

#define VERDICT_ACCEPT 'A'
#define VERDICT_DROP   'D'

typedef enum {
	REASON_DISALLOWED_ID = 0,
	REASON_OUT_OF_CONNECTION,
	REASON_OUT_OF_STATE,
	REASON_BAD_SEQUENCE_NUM,
	REASON_BAD_COMMAND,
	REASON_PACKET_OK
} reason_t;

extern const char *FW_REASONS[];

list_t *rules;
table_t *connections;

int fw_init(char *filename);
int fw_check_id(__u8 client, __u8 server);
int fw_check_cmd(char *val);
int fw_check_con(__u8 client, __u8 server, __u8 seq, cmd_t cmd);
int fw_inspect(char *packet);
int fw_shutdown();

#endif
