#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>

// Global variable
extern char* saved_ptr;

void write_error(const char* msg);
void print_title();
int my_strlen(const char* str_1);
char* my_strchr(const char* str, int search_char);
char* my_strtok(char* str, const char* delim);
int my_strcmp(const char* str_1, const char* str_2);
int my_strncmp(const char *str1, const char *str2, int n);
char* my_strcpy(char* dest, const char* src);


#endif // UTILS_H