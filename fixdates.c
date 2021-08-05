#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>


char *swap_date(char *date);


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect number of arguments.\nusage: fixdates csvfile\n");
        return EXIT_FAILURE;
    }

    FILE *ptr;
    char buffer[512];
    char *ptr_to_date;
    int counter = 0;
    int column = 0;
    bool headers_read = false;

    if ((ptr = fopen(argv[1], "r")) == NULL) {
        perror("Cannot open input file.\n");
        return EXIT_FAILURE;
    }

    while ((fgets(buffer, sizeof(buffer), ptr) != NULL)) {
        if (!headers_read) {
            headers_read = true;
            continue;
        }

        ptr_to_date = strtok(buffer, ",");
        column++;
        while ((ptr_to_date = strtok(NULL, ",")) != NULL) {
            column++;
            if (column == 3) {
                swap_date(ptr_to_date);
                column = 0;
                counter++;
                break;
            }
            if (counter == 50) {
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}


char *swap_date(char *date) {
    char buffer[32] = {0};
    char *month = strtok(date, "/");
    char *day = strtok(NULL, "/");
    char *year = strtok(NULL, "/");

    strncpy(buffer, day, strlen(day) + 1);
    strcat(buffer, "/");
    strcat(buffer, month);
    strcat(buffer, "/");
    strcat(buffer, year);

    printf("day: %s\n", buffer);
}
