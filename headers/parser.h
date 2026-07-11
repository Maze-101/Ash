#ifndef PARSER_H
#define PARSER_H

#define MAX_ARG_COUNTER 64
#define MAX_ARG_LEN 1024

typedef enum PARSE_STATE {
    STATE_NORMAL,
    STATE_IN_SINGLE_QUOTE,
    STATE_IN_DOUBLE_QUOTE,
    STATE_IN_BACKSLASH
} parse_state_t;

char **parse_line(char *input);

#endif
