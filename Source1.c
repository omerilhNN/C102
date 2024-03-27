#include <stdio.h>

#define MAX_BUFFER_SIZE 256
#define PACKET_SIZE 64

int main() {
	FILE* file;
	char* filename = "test.txt";
	char** packets = NULL;
	char buffer[PACKET_SIZE];
	int wordCtr = 0, sentenceCtr = 0, digitCtr = 0, numberCtr = 0;

	if (fopen_s(&file, filename, "r") != 0) {
		printf("Error opening file\n");
		exit(1);
	}
	setvbuf(file, buffer, _IOFBF, PACKET_SIZE);

	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	fseek(file, 0, SEEK_SET);

	int rep = MAX_BUFFER_SIZE / PACKET_SIZE;
	packets = (char**)malloc(sizeof(char*) * MAX_BUFFER_SIZE);

	for (int i = 0; i < rep; i++) {
		packets[i] = (char*)malloc(sizeof(char) * PACKET_SIZE);
		fread(packets[i], sizeof(char), PACKET_SIZE, file);
	}
	// Her bir paket için istatistikleri hesapla
	for (int i = 0; i < rep; i++) {
		for (int j = 0; j < PACKET_SIZE; j++) {
			if (isalnum(packets[i][j])) {
				if (isdigit(packets[i][j])) {
					digitCtr++;
					if (isdigit(packets[i][j + 1])) {
						while (isdigit(packets[i][j + 1])) {
							j++;
						}
						numberCtr++;
					}
				}
				else if (isalpha(packets[i][j])) {
					wordCtr++;
				}
			}
			else if (packets[i][j] == '.' || packets[i][j] == '?' || packets[i][j] == '!') {
				sentenceCtr++;
			}
		}
	}



	printf("Word Count: %d\n", wordCtr);
	printf("Sentence Count: %d\n", sentenceCtr);
	printf("Digit Count: %d\n", digitCtr);
	printf("Number Count: %d\n", numberCtr);


}