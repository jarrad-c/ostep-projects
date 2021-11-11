#include "inc/database.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const int INIT_DB_SIZE = 100;

static int parse_entry(char *line, int *key, char **value) {
	char *key_s = strsep(value, ",");
	*key = atoi(key_s);
	return 0;
}

static database_t *alloc_db(void) {
	database_t *db = (database_t *) malloc(sizeof(database_t));
	if (db == NULL) {
		perror("Failed to alloc database_t");
		return NULL;
	}
	db->size = INIT_DB_SIZE;
	db->table = (char **) calloc(INIT_DB_SIZE, sizeof(char *));
	if (db->table == NULL) {
		perror("Failed to alloc db table");
		free(db);
		return NULL;
	}
	return db;
}

static int realloc_db(database_t *db, int new_size) {
	char **new_table = (char **) realloc(db-> table, new_size * sizeof(char **));
	if (new_table == NULL) {
		perror("Failed to realloc db table");
		return -1;
	}
	db->table = new_table;
	db->size = new_size;
	return 0;
}

void free_db(database_t *db) {
	free(db->table);
	free(db);
}

database_t *load_database(char *db_path) {
	database_t *db = alloc_db();

	FILE *fp = fopen(db_path, "r");
	if (fp == NULL) {
		perror("error loading database file");
		return NULL;	
	}
	
	char *line = NULL;
	size_t linecap = 0;
	while (getline(&line, &linecap, fp) > 0) {
		int key;
		char *value;
		if (parse_entry(line, &key, &value) != 0) {
			fprintf(stderr, "Error parsing line '%s'", line);
			continue;
		}
		put(db, key, value);
	}
	return db;
}

int save_database(database_t *db, char *db_path) {
	return -1;
}

int put(database_t *db, int key, char *val) {
	printf("Putting '%s' at val %d\n", val, key);
	if (key >= db->size) {
		if (realloc_db(db, key + 1) != 0) {
			return -1;
		}
	}
	db->table[key] = val;
	return 0;
}

int get(database_t *db, int key, char **val) {
	printf("Getting value for key %d\n", key);
	if (key >= db->size) {
		return -1;
	}
	if (db->table[key] == NULL) {
		return -1;
	}
	*val = db->table[key];
	return 0;
}

int del(database_t *db, int key) {
	printf("Deleting value for key %d\n",  key);
	if (key >= db->size) {
		return -1;
	}
	if (db->table[key] == NULL) {
		return -1;
	}
	free(db->table[key]);
	db->table[key] = NULL;
	return 0;
}

int clear(database_t *db) {
	printf("Clearing database\n");
	// TODO
	return -1;
}

int all(database_t *db, char ***buffer) {
	printf("Getting all keyvals\n");
	// TODO
	return -1;
}


