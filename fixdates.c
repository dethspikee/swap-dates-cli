#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>


#define FILENAME_SIZE 64
#define LINE_BUFFER_SIZE 512


void show_help(void); 
void validate_field(char *date, char delimiter); 
void write_csv(char *field, FILE *fp_write);
char *swap_date(char *date);


int main(int argc, char *argv[]) {
    if (argc == 1) {
        show_help();
        exit(EXIT_SUCCESS);
    }

    int c_flag = false;
    int d_flag = false;
    int h_flag = false;
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
                h_flag = true;
                break;
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
    char *next_field_ptr, *new_date;
    char filename[FILENAME_SIZE];
    char line_buffer[LINE_BUFFER_SIZE];
    int counter = 0;
    int column = 0;

    strcpy(filename, "new-");
    strcat(filename, argv[optind]);

    if ((fp_read = fopen(argv[optind], "r")) == NULL) {
        perror("Cannot open input file.\n");
        return EXIT_FAILURE;
    }

    if ((fp_write = fopen(filename, "w")) == NULL) {
        fprintf(stderr, "Error");
        exit(EXIT_FAILURE);
    }

    while ((fgets(line_buffer, sizeof(line_buffer), fp_read) != NULL)) {
        if (h_flag) {
            write_csv(line_buffer, fp_write);
            h_flag = false;
            continue;
        }

        next_field_ptr = strtok(line_buffer, ",");
        column++;
        while (next_field_ptr != NULL) {
            if (column == dob_column) {
                validate_field(next_field_ptr, delimiter);
                new_date = swap_date(next_field_ptr);
                write_csv(new_date, fp_write);
                free(new_date);
                counter++;
                next_field_ptr = strtok(NULL, ",");
            } else {
                write_csv(next_field_ptr, fp_write);
                next_field_ptr = strtok(NULL, ",");
            }

            if (next_field_ptr != NULL) {
                write_csv(",", fp_write);
            }
            column++;
        }
        column = 0;
    }

    return EXIT_SUCCESS;
}


void write_csv(char *field, FILE *fp_write) {
    fprintf(fp_write, "%s", field);
}


/*
 * Swap mm/dd to dd/mm and vice versa in the date field.
 */
char *swap_date(char *date) {
    char *new_date_buffer = malloc(100);
    if (new_date_buffer == NULL) {
        fprintf(stderr, "Error allocating memory with malloc.\n");
        exit(EXIT_FAILURE);
    }
    char buffer[3] = {0};
    char buffer2[30] = {0};
    int i;

    for (i = 0; *date != '\0'; date++, i++) {
        if (*date == '/') {
            buffer[i] = '\0';
            date++;
            break;
        }
        buffer[i] = *date;
    }

    for (i = 0; *date != '\0'; date++, i++) {
        if (*date == '/') {
            buffer2[i] = '\0';
            date++;
            break;
        }
        buffer2[i] = *date;
    }

    strcat(buffer2, "/");
    strcat(buffer2, buffer);
    strcat(buffer2, "/");
    strcat(buffer2, date);

    memcpy(new_date_buffer, buffer2, sizeof(buffer2));

    return new_date_buffer;
}


/*
 * Validate each date field to see if the delimiter provided by the user in the
 * program arguments (-d option) matches the delimiter used in the date field.
 */
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


/*
 * Display help menu for the program.
 */
void show_help(void) {
    printf("usage:\tfixdate -c 3 -d / file.csv\n\n");
    printf("\tCreate new csv file based on the input csv file with swapped dates."
            "\n\tExample: dates in the mm/dd/yyyy format will be changed to dd/mm/yyyy\n\tand vice versa.\n\n");
    printf("options:\t\tdescription:\n");
    printf("-c NUMBER\t\tColumn number where dates are located in the input csv.\n");
    printf("-dd CHAR\t\tDate delimiter. In the date 07/09/1993 -dd will be \"/\".\n");
}
