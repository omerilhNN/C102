#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        int length = ftell(file);
        rewind(file);

        packets = (char**)malloc(MAX_PACKETS * sizeof(char*));
        if (packets == NULL) {
            printf("Memory allocation failed for packets\n");
            exit(1);
        }

        int packet_count = 0;
        size_t bytes_read;


        while (!feof(file) && packet_count < MAX_PACKETS) {
            bytes_read = fread(buffer, sizeof(char), PACKET_SIZE, file);
            buffer[bytes_read] = '\0'; // Null terminate buffer
            packets[packet_count] = (char*)malloc((bytes_read + 1) * sizeof(char)); // +1 for null terminator

            if (packets[packet_count] == NULL) {
                printf("Memory allocation failed for packet\n");
                exit(1);
            }

            if (strcpy_s(packets[packet_count], bytes_read + 1, buffer) != 0) {
                printf("String copy failed\n");
                exit(1);
            }
            if (bytes_read >= PACKET_SIZE) {
                memcpy(packets[packet_count], buffer, bytes_read);
                packets[packet_count][bytes_read] = '\0'; // Null terminate buffer
                packet_count++;
            }

            if (packet_count >= MAX_PACKETS || bytes_read < PACKET_SIZE || ftell(file) == length)
                break;
        }


        for (int i = 0; i < packet_count; i++) {
            char* context = NULL;
            char* temp_package = strdup(packets[i]);//strtok_s adresteki asıl değeri değiştiriyor diye strdup ile kopyasını oluştur
            char* token = strtok_s(temp_package, ".!?", &context);

            while (token != NULL) {
                if ((*token != '\0' && ((*token == '\n' || isspace(*token))) && isupper(*(token + 1))) || (i == 0 && ( isupper(*token) || isupper(*(token+1))))) {
                    sentenceCtr++;
                }
                token = strtok_s(NULL, ".!?", &context); // Bir sonraki cümleyi al
            }

            free(temp_package);
            free(token);

            int packet_length = strlen(packets[i]);

            for (int j = 0; j < packet_length && packets[i][j] != '\0'; j++) {
                if (isspace(packets[i][j]) || ispunct(packets[i][j]) || packets[i][j] == '\n') {
                    if (j > 0 && !isspace(packets[i][j - 1]) && !ispunct(packets[i][j - 1]) && !isdigit(packets[i][j-1])) {
                        wordCtr++; // Kelime sayýsýný artýr
                    }
                    if (j> 1 && isdigit(packets[i][j - 1]) && !isdigit(packets[i][j - 2]))
                        digitCtr++;
                    if (j > 1 && isdigit(packets[i][j - 1]) && isdigit(packets[i][j - 2]))
                        numberCtr++;     
                }
                //if (j == length - 1) {
                //    //son karakter eðer harf ise
                //    if (!isspace(packets[i][j]) && !ispunct(packets[i][j])) {
                //        wordCtr++;
                //    }
                //}
            }
            if (!isspace(packets[i][packet_length - 1]) && !ispunct(packets[i][packet_length - 1])) {
                wordCtr++;
            }
        }
    }
        printf("Sentence count: %d\n", sentenceCtr);
        printf("Word count: %d\n", wordCtr);
        printf("Digit count: %d\n", digitCtr);
        printf("Number count: %d\n", numberCtr);


        free(packets);
        fclose(file);
        return 0;
}

    

