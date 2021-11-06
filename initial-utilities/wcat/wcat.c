#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 300

int main(int argc, char **argv) {
	if (argc < 2) {
		exit(0); 
	}
	
	for (int i = 1; i < argc; i++) {
		FILE *fp = fopen(argv[i], "r");
		if (fp == NULL) {
			// one way to print err:
			// fprintf(stderr, "Cannot open file `%s`\n", argv[1]);
			// char *err = strerror(errno);
			// fputs(err, stderr);
			//
			// another:
			// perror("Cannot open file");
			//
			// instructions want this specific error message:
			printf("wcat: cannot open file\n");
			exit(1);
		}
		
		char buffer[MAX_SIZE];
		while (fgets(buffer, MAX_SIZE, fp)) {
			printf("%s", buffer);
		}
		
		if(fclose(fp)) {
			perror("Could not close file");
			exit(1); 
		}

	}
	exit(0);
}

