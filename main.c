#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PACKET_SIZE 16
#define MAX_PACKETS 16

int main() {
	FILE* file;
	char* filename = "test.txt";
	char buffer[PACKET_SIZE] = { 0 };
	char** packets = NULL;
	int wordCtr = 0, sentenceCtr = 0, digitCtr = 0, numberCtr = 0;

	if (fopen_s(&file, filename, "r")) {
		printf("Error opening file\n");
		exit(1);
	}
	else {
		fseek(file, 0, SEEK_END);
		int length = ftell(file);
		fseek(file, 0, SEEK_SET);

		if (setvbuf(file, buffer, _IOFBF, PACKET_SIZE) != 0) {
			printf("Tamponlama ayarlama basarisiz\n");
			exit(1);
		}
		else {
			packets = (char**)malloc(MAX_PACKETS * sizeof(char*));
			int packet_count = 0;

			if (packets == NULL) {
				printf("Bellek tahsisi basarisiz\n");
				exit(1);
			}
			int buffer_index = 0;

			size_t bytes_read;
			while ((bytes_read = fread(buffer + buffer_index, 1, PACKET_SIZE - buffer_index, file)) > 0) {
				buffer_index += bytes_read;
				if (buffer_index >= PACKET_SIZE) {
					packets[packet_count] = (char*)malloc(PACKET_SIZE);
					memcpy(packets[packet_count], buffer, PACKET_SIZE);
					packet_count++;

					buffer_index = 0;

					if (packet_count >= MAX_PACKETS)
						break;
				}
			}
			for (int i = 0; i < packet_count; ++i) {
				printf("package %d", i);

				free(packets[i]);
			}
			free(packets);
		}
	}

	return 0;
}
