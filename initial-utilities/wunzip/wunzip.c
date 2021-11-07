#include <stdio.h>
#include <stdlib.h>

int read_char_count(FILE *, int *, char *);
void write_char_count(int, char);

int main(int argc, char **argv) {
	if (argc < 2) {
		puts("wunzip: file1 [file2 ...]");
		exit(1);
	}
	for (int i = 1; i < argc; i++) {
		FILE *fp = fopen(argv[i], "rb");
		if (fp == NULL) {
			puts("wunzip: Cannot open file");
			exit(1);
		}
		while (!feof(fp)) {
			int count;
			char character;
			int bytes_read = read_char_count(fp, &count, &character);
			if (bytes_read == 0) {
				break;
			}
			// printf("Read char %c with count %d\n", character, count);
			write_char_count(count, character);
		}
	}
}

int read_char_count(FILE *fp, int *count, char *character) {
	int bytes_read;
	bytes_read = fread(count, sizeof(int32_t), 1, fp);
	if (bytes_read == 0) {
		return 0;
	}
	bytes_read += fread(character, sizeof(char), 1, fp);
	return bytes_read;
}

void write_char_count(int count, char character) {
	for (int i = 0; i < count; i++) {
		fwrite(&character, sizeof(char), 1, stdout);
	}
}
