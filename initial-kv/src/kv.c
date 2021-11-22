#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

typedef enum operation {GET, PUT, DELETE, CLEAR, ALL} operation_t;

int get_cmd(char *, operation_t *, int *, char **);
int execute_cmd(database_t *, operation_t, int, char *);
int extract_keyval(char *, int *, char **);

int main(int argc, char **argv) {
	database_t *db = load_database("database.txt");
	for (int i=1; i < argc; i++) {
		operation_t op;
		int key;
		char *value;
		if (get_cmd(argv[i], &op, &key, &value) == -1) {
			fputs("bad command", stderr);
			continue;
		}
		// printf("Got commmand %d with key %d value %s\n", op, key, value);
		if (execute_cmd(db, op, key, value) == -1) {
			free_db(db);
			exit(1);
		}
	}
	if (save_database(db, "database.txt") == -1) {
		fputs("Error saving database", stderr);
		free_db(db);
		exit(1);
	}
	free_db(db);
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
	*key = atoi(key_s);
	*value = command;
	return 0;
}

int execute_cmd(database_t *db, operation_t op, int key, char *value) {
	switch (op) {
		case PUT:
			if (put(db, key, value) == -1) {
				fprintf(stderr, "Failed to put %s at %d", value, key);
				return -1;
			}
			break;
		case GET:
			char *value;
			if (get(db, key, &value) == -1) {
				printf("%d not found\n", key);
				return 0;
			}
			printf("%d,%s\n", key, value);
			break;
		case DELETE:
			if (del(db, key) == -1) {
				fprintf(stderr, "Failed to delete value for key %d\n", key);
				return -1;
			}
			break;
		default:
			fprintf(stderr, "Command %d not yet supported", op);
			return -1;
	}
	return 0;
}
