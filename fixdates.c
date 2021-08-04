#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect number of arguments.\nusage: fixdates csvfile\n");
        return EXIT_FAILURE;
    }

    char buffer[512];
    FILE *ptr;

    if ((ptr = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Cannot open the file: %s\n", argv[1]);
        return EXIT_FAILURE;
    }
}
