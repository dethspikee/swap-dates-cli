#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>


void show_help(void);
char *swap_date(char *date);


int main(int argc, char *argv[]) {
    if (argc > 30) {
        fprintf(stderr, "Incorrect number of arguments.\nusage: fixdates csvfile\n");
        return EXIT_FAILURE;
    }

    int opt;
    int c_flag = false;

    while ((opt = getopt(argc, argv, "c:h")) != -1) {
        switch (opt) {
            case 'c':
                printf("column: %d\n", atoi(optarg));
                c_flag = true;
                break;
            case 'h':
                show_help();
                return EXIT_SUCCESS;
        }
    }

    if (!c_flag) {
        fprintf(stderr, "Missing filename option -c\n");
        return EXIT_FAILURE;
    } else if (argv[optind] == NULL) {
        fprintf(stderr, "Must provide csv filename.\n");
        return EXIT_FAILURE;
    }

    FILE *ptr;
    char buffer[512];
    char *ptr_to_date;
    int counter = 0;
    int column = 0;
    bool headers_read = false;

    if ((ptr = fopen(argv[optind], "r")) == NULL) {
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
            if (counter == 2000)
                break;
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

void show_help(void) {
    printf("usage:\tfixdate -c 3 -dd / file.csv\n\n");
    printf("\tCreate new csv file based on the input csv file with swapped dates."
            "\n\tExample: dates in the mm/dd/yyyy format will be changed to dd/mm/yyyy\n\tand vice versa.\n\n");
    printf("options:\t\tdescription:\n");
    printf("-c NUMBER\t\tColumn number where dates are located in the input csv.\n");
    printf("-dd CHAR\t\tDate delimiter. In the date 07/09/1993 -dd will be \"/\".\n");
}
