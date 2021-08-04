#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect number of arguments.\nusage: fixdates csvfile\n");
        return EXIT_FAILURE;
    }

    char buffer[512];
    char *ptr_to_str;
    FILE *ptr;
    int counter = 0;

    if ((ptr = fopen(argv[1], "r")) == NULL) {
        perror("Cannot open input file.\n");
        return EXIT_FAILURE;
    }

    while ((fgets(buffer, sizeof(buffer), ptr) != NULL)) {
        ptr_to_str = strtok(buffer, ",");
        counter++;
        while ((ptr_to_str = strtok(NULL, ",")) != NULL) {
            counter++;
            if (counter == 3) {
                printf("%s\n", ptr_to_str);
                counter = 0;
                break;
            }
        }
    }

    return EXIT_SUCCESS;
}
