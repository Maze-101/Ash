#ifndef EXECUTER_H
#define EXECUTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/wait.h>

bool is_builtin(const char*);
void execute(char **);
void exec_echo(char **);
void exec_exit();
void exec_type(char **);
char *is_executable(char *command);

#endif