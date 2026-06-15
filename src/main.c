#include "ash.h"

void ash_loop() {
    char *line;
    char **args;
    int status;

    printf("Welcome to the worst shell ever created - Awful shell !\n");
    do {
        printf("> ");
        line = ash_read_line();
        args = ash_split_line(line);
        status = ash_execute(args);
        
        free(line);
        free(args);
    } while (status);
}

int main() {
    // shell loop
    ash_loop();

    return EXIT_SUCCESS;
}