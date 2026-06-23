#include "ash.h"

void ash_loop() {
    char *line;
    char **args;
    int status;

    printf("Welcome to the worst shell ever created - Awful shell !\n");
    do {
        char cwd[PATH_MAX];

        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            // Print the directory, a cool separator, and the prompt symbol
            printf("\033[1;34m%s\033[0m > ", cwd);
        } else {
            // Fallback just in case getcwd fails for some reason
            perror("getcwd() error");
            printf("> ");
        }

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
