#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/database.h"

typedef enum operation {GET, PUT, DELETE, CLEAR, ALL} operation_t;

int get_cmd(char *, operation_t *, int *, char **);
int execute_cmd(operation_t, int, char *);
int extract_keyval(char *, int *, char **);

int main(int argc, char **argv) {
	for (int i=1; i < argc; i++) {
		operation_t op;
		int key;
		char *value;
		if (get_cmd(argv[i], &op, &key, &value) == -1) {
			fputs("bad command", stderr);
			continue;
		}
		printf("Got commmand %d with key %d value %s", op, key, value);

	}
	exit(0);
}

int get_cmd(char *command, operation_t *op, int *key, char **value) {
	char *op_c = strsep(&command, ",");
	if (strlen(op_c) != 1) {
		return -1;
	}
	switch (op_c[0]) {
		case 'd':
			if (extract_keyval(command, key, value) == -1) 
				return -1;
			*op = DELETE;
			return 0;
		case 'p':
			if (extract_keyval(command, key, value) == -1)
				return -1;
			*op = PUT;
			return 0;
		case 'g':
			if (extract_keyval(command, key, value) == -1)
				return -1;
			*op = GET;
			return 0;
		case 'c':
			*op  = CLEAR;
			return 0;
		case 'a':
			*op = ALL;
			return 0;
		default:
			return -1;
	}
}

int extract_keyval(char *command, int *key, char **value) {
	char *key_s = strsep(&command, ",");
	if (key_s == NULL || *key_s == '\0') 
		return -1;
	if (command == NULL || *command == '\0')
		return -1;
	*key = atoi(key_s);
	*value = command;
	return 0;
}

int execute_cmd(operation_t op, int key, char *value) {
	switch (op) {
		case PUT:
			if (put(key, value) == -1) {
				fprintf(stderr, "Failed to put %s at %d", value, key);
				exit(1);
			}
		case default:
			fprintf(stderr, "Command %d not yet supported", op);
			exit(1);
	}
}
