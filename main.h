#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

// Global variable
char* saved_ptr = NULL;

char** parse_input(char *input);
void execute_command(char **args, char** envp);
int start_process(char** args, char** envp);
void get_env_var(char** envp);

// Builtins
void change_directory(char *path);
void print_working_directory();
void set_environment_variable(char *name, char *value);

// System binary
// ls, cat, tail, head, env
void list_directory();
void concat_files(char **args);
void display_file_head(char **args);
void display_file_tail(char **args);
void print_environment(char **envp);

// Utilities
void write_error(const char* msg);
int my_strlen(const char* str_1);
char* my_strchr(const char* str, int search_char);
char* my_strtok(char* str, const char* delim);
int my_strcmp(const char* str_1, const char* str_2);

#endif