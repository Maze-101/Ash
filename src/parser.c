#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** parse_line(char *input) {
    if (!input) {
        return NULL;
    }

    char **tokens = malloc(sizeof(char *) * MAX_ARG_COUNTER);
    if (!tokens) {
        return NULL;
    }

    int argCounter = 0;
    int i = 0;

    // Temporary buffer to build the current argument character-by-character
    char current_arg[MAX_ARG_LEN];
    int arg_idx = 0;

    parse_state_t state = STATE_NORMAL;

    while (input[i] != '\0' && argCounter < MAX_ARG_COUNTER - 1) {
        char c = input[i];

        if (state == STATE_NORMAL) {
            if (c == '\'') {
                state = STATE_IN_SINGLE_QUOTE;
            } else if (c == '"') {
                state = STATE_IN_DOUBLE_QUOTE;
            } else if (c == ' ' || c == '\t' || c == '\n') {
                // If we hit a space and have built an argument, save it
                if (arg_idx > 0) {
                    current_arg[arg_idx] = '\0';
                    tokens[argCounter++] = strdup(current_arg);
                    arg_idx = 0; // Reset buffer for next token
                }
            } else {
                current_arg[arg_idx++] = c;
            }
        } 
        else if (state == STATE_IN_SINGLE_QUOTE) {
            if (c == '\'') {
                state = STATE_NORMAL; // Closing single quote
            } else {
                current_arg[arg_idx++] = c; // Take literally
            }
        } 
        else if (state == STATE_IN_DOUBLE_QUOTE) {
            if (c == '"') {
                state = STATE_NORMAL; // Closing double quote
            } else if (c == '\\') {
                // Handle backslash escaping inside double quotes
                char next_c = input[i + 1];
                if (next_c == '"' || next_c == '\\' || next_c == '$') {
                    current_arg[arg_idx++] = next_c;
                    i++; // Skip next character since we processed it
                } else {
                    current_arg[arg_idx++] = c; // Keep literal backslash
                }
            } else {
                current_arg[arg_idx++] = c;
            }
        }
        i++;
    }

    // Flush any remaining characters left in the buffer as the final token
    if (arg_idx > 0 && argCounter < MAX_ARG_COUNTER - 1) {
        current_arg[arg_idx] = '\0';
        tokens[argCounter++] = strdup(current_arg);
    }

    tokens[argCounter] = NULL; // Null terminate the token array
    return tokens;
}
