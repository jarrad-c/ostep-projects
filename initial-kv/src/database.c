#include "database.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static const int INIT_DB_SIZE = 100;

static int parse_entry(char *line, int *key, char **value) {
	char *key_s = strsep(&line, ",");
	if (key_s == NULL || *key_s == '\0')
		return -1;
	*key = atoi(key_s);
	*value = line;
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

database_t *load_database(char *path) {
	database_t *db = alloc_db();

	int fd = open(path, O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR);
	if (fd == -1) {
		perror("error loading database file");
		free_db(db);
		return NULL;
	}

	FILE *fp = fdopen(fd, "r");
	if (fp == NULL) {
		perror("error loading database file");
		free_db(db);
		return NULL;	
	}
	
	char *line = NULL;
	size_t linecap = 0;
	ssize_t len;
	while ((len = getline(&line, &linecap, fp)) > 0) {
		if (line[len - 1] == '\n') {
			line[len - 1] = '\0';
		}
		if (*line == '\0')
			// empty line	
			continue;
		int key;
		char *value;
		if (parse_entry(line, &key, &value) != 0) {
			fprintf(stderr, "Error parsing line '%s'\n", line);
			continue;
		}
		put(db, key, value);
	}
	fclose(fp);
	return db;
}

int save_database(database_t *db, char *db_path) {
	FILE *fp = fopen(db_path, "w");
	if (fp == NULL) {
		perror("error opening database file");
		return -1;
	}

	for (int key=0; key < db->size; ++key) {
		char *value = db->table[key];
		if (value != NULL) {
			fprintf(fp, "%d,%s\n", key, value); 
		}
	}
	fclose(fp);
	return 0;
}

int put(database_t *db, int key, char *val) {
	// printf("Putting '%s' at val %d\n", val, key);
	if (key >= db->size) {
		if (realloc_db(db, key + 1) != 0) {
			return -1;
		}
	}

	int len = strlen(val);
	char *valCopy = (char *) malloc(len * sizeof(char));
	db->table[key] = strcpy(valCopy, val);
	return 0;
}

int get(database_t *db, int key, char **val) {
	// printf("Getting value for key %d\n", key);
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
	// printf("Deleting value for key %d\n",  key);
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
	for (int key = 0; key < db->size; ++key) {
		del(db, key);
	}
	return 0;
}

int print_all(database_t *db) {
	for (int key = 0; key < db->size; ++key) {
		if (db->table[key] != NULL) {
			printf("%d,%s\n", key, db->table[key]);
		}
	}
	return 0;
}


