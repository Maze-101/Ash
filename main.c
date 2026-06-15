#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define ASH_RL_BUFSIZE 1024
#define ASH_TOK_BUFSIZE 64
#define ASH_TOK_DELIM " \t\r\n\a"

char** ash_split_line(char* line){
    int bufsize = ASH_TOK_BUFSIZE, position = 0;
    char** tokens = malloc(sizeof(char*) * bufsize);
    char* token;

    if(!tokens){
        fprintf(stderr, "ash: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, ASH_TOK_DELIM);

    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position >= bufsize){
            bufsize += ASH_TOK_BUFSIZE;
            tokens = realloc(tokens, sizeof(char*) * bufsize);
            if(!tokens){
                fprintf(stderr, "ash: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, ASH_TOK_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

void *ash_read_line(){
    int bufsize = ASH_RL_BUFSIZE;
    int position = 0;
    char* buffer = malloc(sizeof(char) * bufsize);
    int c;

    // validate allocation
    if(!buffer){
        fprintf(stderr, "ash: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while(1){
        // read a character
        c  = getchar();

        // if we hit EOF, replace it with null character, and return
        if(c == EOF || c == '\n'){
            buffer[position] = '\n';
            return buffer;
        } else {
            buffer[position] = c;
        }

        position++;

        // if we exceeded the buffer, reallocate
        if(position >= bufsize){
            bufsize += ASH_RL_BUFSIZE;
            buffer = realloc(buffer, bufsize);
            
            // validate reallocation
            if(!buffer){
                fprintf(stderr, "ash: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void ash_loop(){
    char* line;
    char** args;
    int status;

    do {
        // prompt section
        printf("> ");

        // basic shell life time
        line   = ash_read_line();      // read
        args   = ash_split_line(line); // parse
        status = ash_execute(args);    // execute
        
        free(line);
        free(args);
    } while(status);
}

int main (){
    // load config files, if any;

    // shell loop
    ash_loop();

    // perform any shutdown or cleanup
    return EXIT_SUCCESS;
}