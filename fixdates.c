#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>


void show_help(void); 
void validate_field(char *date, char delimiter); 
void write_csv(char *field, FILE *fp_write, bool add_delimiter);
char *swap_date(char *date);


int main(int argc, char *argv[]) {
    int c_flag = false;
    int d_flag = false;
    int dob_column = 0;
    int opt;
    char delimiter;

    while ((opt = getopt(argc, argv, "c:d:h")) != -1) {
        switch (opt) {
            case 'c':
                c_flag = true;
                dob_column = atoi(optarg);
                break;
            case 'd':
                d_flag = true;
                delimiter = *optarg;
                break;
            case 'h':
                show_help();
                return EXIT_SUCCESS;
        }
    }

    if (!c_flag) {
        fprintf(stderr, "Missing column option -c\n");
        return EXIT_FAILURE;
    } else if (!d_flag) {
        fprintf(stderr, "Missing delimiter option -d\n");
        return EXIT_FAILURE;
    } else if (argv[optind] == NULL) {
        fprintf(stderr, "Must provide csv filename.\n");
        return EXIT_FAILURE;
    }

    FILE *fp_read, *fp_write;
    char *ptr_to_date, *new_date;
    char buffer[512];
    int counter = 0;
    int column = 0;
    bool headers_read = false;

    if ((fp_read = fopen(argv[optind], "r")) == NULL) {
        perror("Cannot open input file.\n");
        return EXIT_FAILURE;
    }

    if ((fp_write = fopen("testing.csv", "w")) == NULL) {
        fprintf(stderr, "Error");
        exit(EXIT_FAILURE);
    }

    while ((fgets(buffer, sizeof(buffer), fp_read) != NULL)) {
        if (!headers_read) {
            headers_read = true;
            write_csv(buffer, fp_write, false);
            //exit(0);
            continue;
        }

        ptr_to_date = strtok(buffer, ",");
        column++;
        while (ptr_to_date != NULL) {
            if (column == dob_column) {
                validate_field(ptr_to_date, delimiter);
                new_date = swap_date(ptr_to_date);
                write_csv(new_date, fp_write, true);
                free(new_date);
                counter++;
            } else {
                write_csv(ptr_to_date, fp_write, true);
                column++;
            }

            //if (counter == 10)
                //exit(0);

            ptr_to_date = strtok(NULL, ",");
        }
        column = 0;
        write_csv("\n", fp_write, false);
    }

    return EXIT_SUCCESS;
}


void write_csv(char *field, FILE *fp_write, bool add_delimiter) {
    if (add_delimiter) {
        fprintf(fp_write, "%s,", field);
    } else {
        fprintf(fp_write, "%s", field);
    }
}

char *swap_date(char *date) {
    char *test = malloc(100);
    char buffer[32] = {0};
    char *month = strtok(date, "/");
    char *day = strtok(NULL, "/");
    char *year = strtok(NULL, "/");

    strncpy(test, day, strlen(day) + 1);
    strcat(test, "/");
    strcat(test, month);
    strcat(test, "/");
    strcat(test, year);

    return test;
}


void validate_field(char *date, char delimiter) {
    while(*date) {
        if (*date == delimiter) {
            return;
        }
        date++;
    }
    printf("Invalid file type. Make sure -c NUMBER matches column number in the input csv file\n"
            "and -d CHAR matches input date delimiter.\n");
    exit(EXIT_FAILURE);
}


void show_help(void) {
    printf("usage:\tfixdate -c 3 -dd / file.csv\n\n");
    printf("\tCreate new csv file based on the input csv file with swapped dates."
            "\n\tExample: dates in the mm/dd/yyyy format will be changed to dd/mm/yyyy\n\tand vice versa.\n\n");
    printf("options:\t\tdescription:\n");
    printf("-c NUMBER\t\tColumn number where dates are located in the input csv.\n");
    printf("-dd CHAR\t\tDate delimiter. In the date 07/09/1993 -dd will be \"/\".\n");
}
