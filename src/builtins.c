#include "../include/builtins.h"
#include "../include/utils.h"

char *previous_dir = NULL;

void change_directory(char *path) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    
    if (my_strcmp(path, "-") == 0) {
        if (previous_dir == NULL) {
            write(STDERR_FILENO, "cd: no previous directory\n", 26);
            return;
        }
        path = previous_dir;
    }
    
    if (chdir(path) == 0) {
        // Store current dir as previous
        if (previous_dir != NULL) free(previous_dir);
        previous_dir = malloc(my_strlen(cwd) + 1);
        my_strcpy(previous_dir, cwd);
    } else {
        perror("cd");
    }
}

void print_working_directory() {
    char cwd[MAX_INPUT_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        write(STDOUT_FILENO, cwd, my_strlen(cwd));
        write(STDOUT_FILENO, "\n", 1);
    } else {
        perror("getcwd failed");
    }
}

void set_environment_variable(char *name, char *value) {
    if (setenv(name, value, 1) != 0) {
        perror("setenv failed");
    }
}

void unset_environment_variable(char *name) {
    if (unsetenv(name) != 0) {
        perror("unsetenv failed");
    }
}

void print_echo(char **args) {
    for (int i = 1; args[i] != NULL; i++) {
        write(STDOUT_FILENO, args[i], my_strlen(args[i]));
        if (args[i + 1] != NULL) {
            write(STDOUT_FILENO, " ", 1);
        }
    }
    write(STDOUT_FILENO, "\n", 1);
}

int which(char **args) {
    if (args[1] == NULL) {
        write(STDERR_FILENO, "which: expected argument\n", 24);
        return -1;
    }

    char *command = args[1];
    // Check if absolute path exists and is executable
    struct stat st;
    if (stat(command, &st) == 0) {
        if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR)) {
            write(STDOUT_FILENO, command, my_strlen(command));
            write(STDOUT_FILENO, "\n", 1);
            return 0;
        }
    }
    
    // Search in PATH
    extern char **environ;
    char *path_value = NULL;
    
    for (char **env = environ; *env != NULL; env++) {
        if (my_strncmp(*env, "PATH=", 5) == 0) {
            path_value = *env + 5;
            break;
        }
    }
    
    if (path_value != NULL) {
        char *path_copy = malloc(my_strlen(path_value) + 1);
        my_strcpy(path_copy, path_value);
        
        char *dir = my_strtok(path_copy, ":");
        while (dir != NULL) {
            char full_path[1024];
            my_strcpy(full_path, dir);
            my_strcpy(full_path + my_strlen(full_path), "/");
            my_strcpy(full_path + my_strlen(full_path), command);
            
            if (stat(full_path, &st) == 0) {
                if (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR)) {
                    write(STDOUT_FILENO, full_path, my_strlen(full_path));
                    write(STDOUT_FILENO, "\n", 1);
                    free(path_copy);
                    return 0;
                }
            }
            dir = my_strtok(NULL, ":");
        }
        free(path_copy);
    }
    
    write(STDERR_FILENO, command, my_strlen(command));
    write(STDERR_FILENO, ": not found\n", 12);
    return 1;
}

void print_environment() {
    extern char **environ;
    
    for (char **env = environ; *env != NULL; env++) {
        write(STDOUT_FILENO, *env, my_strlen(*env));
        write(STDOUT_FILENO, "\n", 1);
    }
}