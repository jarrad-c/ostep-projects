#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool match_present(char *line, char *searchTerm) {
	int lineLength = strlen(line);
	int termLength = strlen(searchTerm);

	for (int j = 0; j < (lineLength); j++) {	
		if (strncmp(&line[j], searchTerm, termLength) == 0) {
			return true;
		}
	}
	return false;
}

void find_lines(FILE *fp, char *searchTerm) {
	char *line = NULL;
	size_t linecap = 0;
	while (getline(&line, &linecap, fp) > 0) {
		if (match_present(line, searchTerm)) {
			printf("%s", line);
		}
	}
}


int main(int argc, char **argv) {
	if (argc < 2) {
		printf("wgrep: searchterm [file ...]\n");
		exit(1);
	}
	char *searchTerm = argv[1];
	
	if (argc == 2) {
		find_lines(stdin, searchTerm);
		exit(0);
	}

	for (int i = 2; i < argc; i++) {
		FILE *fp = fopen(argv[i], "r");
		if (fp == NULL) {
			printf("wgrep: cannot open file\n");
			exit(1);
		}
		
		find_lines(fp, searchTerm);
		fclose(fp);
	}
	exit(0);
}

