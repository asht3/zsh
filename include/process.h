#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>

// Main functions
char** parse_input(char *input);
int execute_command(char **args, char** envp);
int start_process(char** args, char** envp);
char* get_env_var(char** envp);

#endif // PROCESS_H