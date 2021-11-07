#include <stdio.h>
#include <stdlib.h>

void write_rle_char(char, int);

int main(int argc, char **argv) {
	if (argc < 2) {
		puts("wzip: file1 [file2 ...]");
		exit(1);
	}
	char running_char;
	char next_char;
	int32_t running_count = 0;

	
	for (int i = 1; i < argc; i++) {
		FILE *fp = fopen(argv[i], "r");
		if (fp == NULL) {
			puts("wzip: Cannot open file");
			exit(1);
		}

		fread(&running_char, sizeof(char), 1, fp);
		while (!feof(fp)) {
			running_count++;
			fread(&next_char, sizeof(char), 1, fp);
			if (next_char != running_char) {
				write_rle_char(running_char, running_count);
				running_count = 0;
				running_char = next_char;
			}
		}

	}
	write_rle_char(running_char, running_count);
	exit(0);
}

void write_rle_char(char character, int32_t count) {
	fwrite(&count, sizeof(int32_t), 1, stdout);	
	fwrite(&character, sizeof(char), 1, stdout);
}

