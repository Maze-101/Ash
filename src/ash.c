#include "executor.h"
#include "parser.h"
#include "read.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // Flush after every printf
    setbuf(stdout, NULL);
    
    while(1){
        printf("$ ");
        
        char *input = read_line();
        if(!input){
            break;
        }

        char **tokens = parse_line(input);
        if(!tokens){
            free(input);
            exit(EXIT_FAILURE);
        }
        
        execute(tokens);

        for (int i = 0; tokens[i] != NULL; i++) {
            free(tokens[i]);
        }
        
        free(tokens);
        free(input);
    }
    return 0;
}
