#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_INPUT_SIZE 1024

// Global variable
char* saved_ptr = NULL;

char** parse_input(char *input);
void execute_command(char **args, char** envp);

// Builtins
void change_directory(char *path);
void print_working_directory();
void set_environment_variable(char *name, char *value);
// TODO: exit

// System binary
// ls, cat, tail, head, env

// Utilities
void write_error(const char* msg);
int my_strlen(const char* str_1);
char* my_strchr(const char* str, int search_char);
char* my_strtok(char* str, const char* delim);

#endif