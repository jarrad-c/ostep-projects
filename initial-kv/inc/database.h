#ifndef DATABASE_H
#define DATABASE_H

typedef struct database {
	char **table;
	int size;
} database_t;

database_t *load_database(char *);
int save_database(database_t *, char *);

void free_db(database_t *);

int put(database_t *, int, char *);
int get(database_t *, int, char **);
int del(database_t *, int);
int clear(database_t *);
int print_all(database_t *);


#endif
