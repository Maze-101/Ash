#ifndef EXECUTER_H
#define EXECUTER_H

#include <stdbool.h>

bool is_builtin(const char *command);
void execute(char **);
void exec_echo(char **);
void exec_exit(void);
void exec_type(char **);
char *is_executable(char *command);

#endif
