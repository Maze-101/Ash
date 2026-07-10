#ifndef EXECUTER_H
#define EXECUTER_H

#define BUILTINS_COUNT 4

#include <stdbool.h>

typedef struct BUILTINS {
    char *command;
    void (*handler)(char **);
} builtins_t;

bool is_builtin(const char *);
void execute(char **);
void exec_echo(char **);
void exec_exit(char **);
void exec_type(char **);
void exec_pwd(char **);
void execute_external(char **);
char *is_executable(char *);

#endif
