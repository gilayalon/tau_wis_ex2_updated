#include "firewall.h"
#include <stdio.h>

#define MAXLENGTH 1024

int main(int argc, char **argv) {
	int go = 1, i = 0;
	char c = 0;

	fw_init(argv[1]);

	while (go == 1) {

		char command[MAXLENGTH] = { 0 };
		i = 0;
		while ((c = getchar()) != '\n') {
			command[i] = c;
			i++;
		}
		if (strcmp(command, "exit") == 0) { /* to end the program neatly */
			go = 0;
		} else {
			fw_inspect(command);
			printf("From %s To %s Verdict %s Reason %s\n",output[0], output[1], output[2], FW_REASONS[reason]);
		}
	}
	fw_shutdown();
	return 0;
}
