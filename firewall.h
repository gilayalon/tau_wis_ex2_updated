#ifndef _TAU_FW_H_
#define _TAU_FW_H_

#include <linux/types.h>

#define MAX_CONN 120


extern const char *CMDS[];

typedef enum {
	CMD_CLIENT_HELLO = 0,
	CMD_SERVER_HELLO,
	CMD_CLIENT_SUPER,
	CMD_SERVER_MARKET,
	CMD_CLIENT_BYE,
	CMD_SERVER_BYE,
	NUM_OF_CMDS
} cmd_t;	


typedef struct {
	__u8 client_min_id, client_max_id;
	__u8 server_min_id, server_max_id;
	__u8 verdict;
	__u8 padding[3];
} rule_t;


#define VERDICT_ACCEPT 'A'
#define VERDICT_DROP   'D'


extern const char *REASONS[];

typedef enum {
	REASON_DISALLOWED_ID = 0, 
	REASON_OUT_OF_CONNECTION,
	REASON_OUT_OF_STATE,     
	REASON_BAD_SEQUENCE_NUM, 
	REASON_BAD_COMMAND,
	REASON_PACKET_OK,
	NUM_OF_REASONS
} reason_t;


typedef enum {
	STATE_PRE = 0,
	STATE_INIT,
	STATE_IDLE,
	STATE_SUPER,
	STATE_CLIENT_BYE,
	STATE_SERVER_BYE,
	STATE_END,
	STATE_BAD,
	NUM_OF_STATES
} state_t;


typedef struct {
	__u8 client, server, seq, state;
} conv_t;


typedef enum {
	SIDE_CLIENT,
	SIDE_SERVER
} side_t;

#endif
