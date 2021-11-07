#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

// initialize buf with spots for 100 string pointers
int buf_len = 100;

int read_all_lines(FILE *, char **);
void write_lines_in_reverse(FILE *, char **, int);
bool same_file(FILE *, FILE *);

int main(int argc, char *argv[]) {
	FILE *in_fp;
	FILE *out_fp;

	if (argc > 3) {
		fprintf(stderr, "usage: reverse <input> <output>\n");
		exit(1);
	}
	// get std fps
	if (argc <= 2) {
		out_fp = stdout;
	}
	else {
		out_fp = fopen(argv[2], "w");
		if (out_fp == NULL) {
			fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
			exit(1);
		}
	}

	// get input fp
	if (argc == 1) {
		in_fp = stdin;
	}
	else {
		in_fp = fopen(argv[1], "r");
		if (in_fp == NULL) {
			fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
			exit(1);
		}
	}

	if (same_file(in_fp, out_fp)) {
		fprintf(stderr, "reverse: input and output file must differ\n");
		exit(1);
	}


	char **buffer = (char **) malloc(buf_len * sizeof(char *));
	if (buffer == NULL) {
		fprintf(stderr, "malloc failed");
		exit(1);
	}
	int lines = read_all_lines(in_fp, buffer);
	fclose(in_fp);
	write_lines_in_reverse(out_fp, buffer, lines);
	fclose(out_fp);
	exit(0);
}

int read_all_lines(FILE *fp, char **buffer) {
	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;
	int i = 0;
	while ((linelen = getline(&line, &linecap, fp)) > 0) {
		if (i >= buf_len) {
			buf_len += 100;
			buffer = (char **) realloc(buffer, buf_len * sizeof(char *));
			if (buffer == NULL) {
				fprintf(stderr, "malloc failed");
				exit(1);
			}
		}
		char *line_copy = (char *) malloc(linelen);
		strlcpy(line_copy, line, linelen);
		buffer[i] = line_copy;
		i++;	
	}
	return i;
}

void write_lines_in_reverse(FILE *fp, char **buffer, int lines) {
	for (int i = (lines - 1); i >= 0; i--) {
		fprintf(fp, "%s\n", buffer[i]);
	}
}

bool same_file(FILE *f1, FILE *f2) {
	int fd1 = fileno(f1);
	int fd2 = fileno(f2);
	
	struct stat stat1, stat2;
	if (fstat(fd1, &stat1) == -1) exit(1);
	if (fstat(fd2, &stat2) == -1) exit(1);
	
	return ((stat1.st_dev == stat2.st_dev) && (stat1.st_ino == stat2.st_ino));
}
