#include <stdio.h>
#include "inc/commands.h"

int put(int key, char *val) {
	printf("Putting '%s' at val %d\n", val, key);
	// TODO
	return -1;
}

int get(int key, char **val) {
	printf("Getting value for key %d\n", key);
	// TODO
	return -1;
}

int del(int key) {
	printf("Deleting value for key %d\n",  key);
	// TODO
	return -1;
}

int clear(void) {
	printf("Clearing database\n");
	// TODO
	return -1;
}

int all(char ***buffer) {
	printf("Getting all keyvals\n");
	// TODO
	return -1;
}


