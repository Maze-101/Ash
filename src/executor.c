#include "ash.h"

// Define the global arrays here (without the 'extern' keyword)
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &ash_cd,
    &ash_help,
    &ash_exit
};

int ash_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int ash_cd(char **args) {
    (void)args;
    if (args[1] == NULL) {
        fprintf(stderr, "ash: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("ash");
        }
    }
    return 1;
}

int ash_help(char **args) {
    (void)args;
    int i;
    printf("Ash - Awful Shell\n");
    printf("Type program names and arguments, and hit enter!\n");
    printf("The following are built in:\n");

    for (i = 0; i < ash_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
    return 1;
}

int ash_exit(char **args) {
    (void)args;
    return 0;
}

int ash_launch(char **args) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // child process
        if (execvp(args[0], args) == -1) {
            perror("ash");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // fork error
        perror("ash");
    } else {
        // parent process
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int ash_execute(char **args) {
    int i;

    if (args[0] == NULL) {
        return 1;
    }

    for (i = 0; i < ash_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return ash_launch(args);
}
