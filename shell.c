#include "shell.h"
#include "firewall.h"
#include <stdio.h>

#define MAXLENGTH 1024

int main(int argc, char **argv) {
	int go = 1, i = 0;
	char c = 0;

	fw_init(argv[1]);

	while (go == 1) {
		char command[MAXLENGTH] = { 0 };

		while ((c = getchar()) != '\n') {
			command[i] = c;
			i++;
		}
		fw_inspect(command);
	}
	return 0;
}
