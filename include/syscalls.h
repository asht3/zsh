#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>

void list_directory(char** args);

// ls helpers
void display_entries(char** entries, int count, int l_flag);
void print_long_format(const char* name, struct stat* file_stat);

void concat_files(char **args);
void display_file_head(char **args);
void display_file_tail(char **args);

#endif // SYSCALLS_H