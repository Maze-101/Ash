#include "executor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

builtins_t builtins[BUILTINS_COUNT] = {
    {"echo", exec_echo},
    {"exit", exec_exit},
    {"type", exec_type},
    {"pwd" , exec_pwd},
    {"cd"  , exec_cd}
};

bool is_builtin(const char* command){
	for(int i=0; i < BUILTINS_COUNT; i++){
		if(strcmp(command, builtins[i].command) == 0){
			return true;
		}
	}
	return false;
}

void exec_echo(char **tokens){
    for(int i = 1; tokens[i] != NULL; i++){
        if(i > 1){
            printf(" ");
        }
        printf("%s", tokens[i]);
    }
    printf("\n");
}

void exec_exit(char **tokens){
    exit(0);
}

char *is_executable(char *command){
    char *env_path = getenv("PATH");
    if(!env_path){
        printf("%s: not found\n", command);
        return NULL;
    }

    char *path = malloc(strlen(env_path) + 1);
    if(!path){
        fprintf(stderr, "ash: allocation error\n");
        exit(EXIT_FAILURE);
    }
    strcpy(path, env_path);
    for (char *dir = strtok(path, ":"); dir != NULL; dir = strtok(NULL, ":")) {
        size_t fpath_size = strlen(dir) + strlen(command) + 10;
        char *fpath = malloc(fpath_size);
        if(!fpath){
            free(path);
            fprintf(stderr, "ash: allocation error\n");
            exit(EXIT_FAILURE);
        }

        snprintf(fpath, fpath_size, "%s/%s", dir, command);
        if (access(fpath, X_OK) == 0) {
            free(path);
            return fpath;
        }
        free(fpath);
    }
    free(path);
    return NULL;
}

void exec_type(char **tokens){
    char *command = tokens[1];

    if(!command){
        printf("type: missing argument\n");
        return;
    }

    if(is_builtin(command)){
        printf("%s is a shell builtin\n", command);
    } else {
        char *fpath = is_executable(command);
        if(fpath){
            printf("%s is %s\n", command, fpath);
        } else {
            printf("%s: not found\n", command);
        }
    }
}

void exec_pwd(char **tokens){
    char *cwd = getcwd(NULL, 0);
    if(cwd){
        printf("%s\n", cwd);
        free(cwd);
    } else {
        perror("pwd error");
    }
}

char *expand_tilde(const char *path) {
    if (path == NULL) return NULL;

    char *home = getenv("HOME");
    if (home == NULL) {
        fprintf(stderr, "ash: cd: HOME not set\n");
        return NULL;
    }

    char *expanded = malloc(strlen(home) + strlen(path) - 1 + 1);
    if (expanded == NULL) {
        perror("malloc failed");
        return NULL;
    }

    strcpy(expanded, home);
    strcat(expanded, path + 1); // path + 1 skips the '~'

    return expanded;
}

void exec_cd(char **tokens){
    char *path = tokens[1];
    int status;
    if (path == NULL) {
        status = chdir(getenv("HOME"));
    } else if(path[0] == '~'){
        char *expanded = expand_tilde(path);
        status = chdir(expanded);
    } else {
        status = chdir(path);
    }
    if(status){
        fprintf(stderr, "ash: cd: %s: No such file or directory\n", path);
    }    
}

void execute_external(char **tokens){
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // child process
        if (execvp(tokens[0], tokens) == -1) {
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
}

void execute(char **tokens){
    if(!tokens || !tokens[0]){
        return;
    }

    char *command = tokens[0];

    if(is_builtin(command)){
        for(int i = 0; i < BUILTINS_COUNT; i++){
            if(strcmp(command, builtins[i].command) == 0){
                builtins[i].handler(tokens);
                return;
            }
        }
    } else if(is_executable(command)){
        execute_external(tokens);
    } else {
        printf("%s: command not found\n", command);
    }
}
