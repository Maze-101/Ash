#include "read.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_line(void){
    size_t buffer_size = LINE_BUFFER_SIZE;
    char *input = malloc(sizeof(char) * buffer_size);

    if(!input) {
        fprintf(stderr, "ash: allocation error\n");
        exit(EXIT_FAILURE);
    }

    // reads content written in stdin and write it to string <input>
    // appends an endline character to the string
    if (!fgets(input, sizeof(char) * buffer_size, stdin)) {
        free(input);
        return NULL;
    }

    // gets the number of characters before the endline character
    // NOTE: it will be the index of the endline character
    input[strcspn(input, "\n")] = '\0';

    return input;
}
