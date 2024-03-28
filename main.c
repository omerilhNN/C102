#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PACKET_SIZE 16
#define MAX_PACKETS 16

int main() {
	FILE* file;
	char* filename = "test.txt";
	char buffer[PACKET_SIZE + 1]; // +1 for null terminator
	char** packets = NULL;
	int wordCtr = 0, sentenceCtr = 0, digitCtr = 0, numberCtr = 0;

	if (fopen_s(&file, filename, "r")) {
		printf("Error opening file\n");
		exit(1);
	}
	else {
		fseek(file, 0, SEEK_END);
		size_t fileSize = ftell(file);
		rewind(file);


		packets = (char**)malloc(MAX_PACKETS * sizeof(char*));
		if (packets == NULL) {
			printf("Memory allocation failed for packets\n");
			exit(1);
		}

		int packet_count = 0;
		size_t bytes_read; // dosya ile ilgili veri boyutu iþlemleri için size_t

		// Dosya tamponu oluþtur
		if (setvbuf(file, NULL, _IOFBF, PACKET_SIZE) != 0) {
			printf("Buffering failed\n");
			exit(1);
		}

		while ((bytes_read = fread(buffer, sizeof(char),  PACKET_SIZE, file)) != 0) {
			buffer[bytes_read] = '\0'; // Null terminate buffer
			packets[packet_count] = (char*)malloc((bytes_read + 1) * sizeof(char)); // +1 for null terminator
			if (packets[packet_count] == NULL) {
				printf("Memory allocation failed for packet\n");
				exit(1);
			}

			if (bytes_read >= PACKET_SIZE) {
				memcpy(packets[packet_count], buffer, bytes_read);
				//packets[packet_count][bytes_read] = '\0'; // Null terminate buffer
				packet_count++;
			}
			
			if (packet_count >= MAX_PACKETS || ftell(file) == fileSize)
				break;
		}


		for (int i = 0; i < packet_count; i++) {
			printf("Package %d,%s\n", i,packets[i]);
		}

		// Free dynamically allocated memory
		for (int i = 0; i < packet_count; i++) {
			free(packets[i]);
		}
		free(packets);

		fclose(file);
	}

	return 0;
}
