#ifndef BUILTINS_H
#define BUILTINS_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

extern char *previous_dir;
#define MAX_INPUT_SIZE 1024

void change_directory(char *path);
void print_working_directory();
void set_environment_variable(char *name, char *value);
void unset_environment_variable(char *name);
void print_echo(char **args);
int which(char **args);
void print_environment();

#endif