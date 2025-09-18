#ifndef PROCESS_H
#define PROCESS_H

#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>

#define SIGSEGV 11

// Main functions
char** parse_input(char *input);
char* expand_variable(char *token);
int execute_command(char **args, char** envp);
int start_process(char** args, char** envp);
char* get_env_var(char** envp);

#endif // PROCESS_H