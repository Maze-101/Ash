#include "../headers/read.h"
#include "../headers/parser.h"
#include "../headers/executer.h"

int main(void) {
    // Flush after every printf
    setbuf(stdout, NULL);
    
    while(1){
        char *input;
        char **tokens;
        printf("$ ");
        
        input = read_line();
        if(!input){
            break;
        }

        tokens = parse_line(input);
        if(!tokens){
            free(input);
            exit(EXIT_FAILURE);
        }
        
        execute(tokens);

        free(tokens);
        free(input);
    }
    return 0;
}
