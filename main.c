#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define ASH_RL_BUFSIZE 1024
#define ASH_TOK_BUFSIZE 64
#define ASH_TOK_DELIM " \t\r\n\a"

// List of builtin commands, followed by their corresponding functions.

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

// Builtin function implementations.

int ash_cd(char **args)
{
    if (args[1] == NULL) {
        fprintf(stderr, "ash: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("ash");
        }
    }
    return 1;
}

int ash_help(char **args)
{
    int i;
    printf("Shikigami's ash\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("The following are built in:\n");

    for (i = 0; i < ash_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");
  return 1;
}

int ash_exit(char **args){
    return 0;
}

int ash_launch(char** args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if(pid == 0) {
        // child process
        if(execvp(args[0], args) == -1){
            perror("ash");
        }
        exit(EXIT_FAILURE);
    } else if(pid < 0) {
        // error forking
        perror("ash");
    } else {
        // parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

char** ash_split_line(char* line){
    int    bufsize   = ASH_TOK_BUFSIZE;
    int    position  = 0;
    char** tokens    = malloc(sizeof(char*) * bufsize);
    char*  token;
    
    // validate pointer
    if(!tokens){
        fprintf(stderr, "ash: allocation error\n");
        exit(EXIT_FAILURE);
    }

    // start tokenization
    token = strtok(line, ASH_TOK_DELIM);

    while(token != NULL){
        tokens[position] = token;
        position++;

        if(position >= bufsize){
            bufsize += ASH_TOK_BUFSIZE;
            tokens   = realloc(tokens, sizeof(char*) * bufsize);

            // validate reallocation
            if(!tokens){
                fprintf(stderr, "ash: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        // tokenize from where we left off
        token = strtok(NULL, ASH_TOK_DELIM);
    }

    // null terminator
    tokens[position] = NULL;
    return tokens;
}

char* ash_read_line(){
    int   bufsize  = ASH_RL_BUFSIZE;
    int   position = 0;
    char* buffer   = malloc(sizeof(char) * bufsize);
    int   c;

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
            buffer   = realloc(buffer, bufsize);
            
            // validate reallocation
            if(!buffer){
                fprintf(stderr, "ash: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

void ash_loop(){
    char*  line;
    char** args;
    int    status;

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