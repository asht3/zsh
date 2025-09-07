#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

char **parse_input(char *input);
void execute_command(char **args);

// builtins
void change_directory(char *path);
void print_working_directory();
void set_environment_variable(char *name, char *value);
// TODO: exit

// system binary
// ls, cat, tail, head, env

#endif