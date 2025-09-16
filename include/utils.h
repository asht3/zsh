#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <sys/stat.h>

// Global variable
extern char* saved_ptr;

void write_error(const char* msg);
int my_strlen(const char* str_1);
char* my_strchr(const char* str, int search_char);
char* my_strtok(char* str, const char* delim);
int my_strcmp(const char* str_1, const char* str_2);
int my_strncmp(const char *str1, const char *str2, int n);
char* my_strcpy(char* dest, const char* src);
int compare_alphabet(const void* file_1, const void* file_2);
int compare_mod_times(const void* file_1, const void* file_2);
void swap(void** arr, int i, int j);
int partition(void** arr, int low, int high, int (*compare)(const void*, const void*));
void quicksort(void** arr, int low, int high, int (*compare)(const void*, const void*));
int num_to_str(char *buffer, long num);

#endif // UTILS_H