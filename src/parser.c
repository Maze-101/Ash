#include "parser.h"

#include <stdlib.h>
#include <string.h>

char** parse_line(char *input){
    if(!input){
        return NULL;
    }

    int argCounter = 0;
    char **tokens = malloc(sizeof(char *) * MAX_ARG_COUNTER);
    char *token = strtok(input, " ");

    if(!tokens){
        return NULL;
    }

    while(token && argCounter < MAX_ARG_COUNTER - 1){
        tokens[argCounter] = token;
        argCounter++;
        token = strtok(NULL, " ");
    }

    tokens[argCounter] = NULL;

    return tokens;
}
