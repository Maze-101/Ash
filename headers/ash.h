#ifndef ASH_H
#define ASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define ASH_RL_BUFSIZE 1024
#define ASH_TOK_BUFSIZE 64
#define ASH_TOK_DELIM " \t\r\n\a"

// Core Shell Functions
char *ash_read_line(void);
char **ash_split_line(char *line);
int ash_execute(char **args);
int ash_launch(char **args);
void ash_loop(void);

// Builtin Declarations
int ash_cd(char **args);
int ash_help(char **args);
int ash_exit(char **args);
int ash_num_builtins(void);

// We use 'extern' here so the arrays can be defined in executor.c 
// but accessed elsewhere if needed without causing duplicate definition errors.
extern char *builtin_str[];
extern int (*builtin_func[]) (char **);

#endif