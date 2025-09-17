#include "../include/process.h"
#include "../include/utils.h"
#include "../include/builtins.h"

char** parse_input(char *input) {
    if (input == NULL) return NULL;

    char** args = malloc(sizeof(char*) * (MAX_INPUT_SIZE / 2 + 1));
    if (args == NULL) {
        perror("malloc failed");
        return NULL;
    }
    char* token;
    int position = 0;
    token = my_strtok(input, " ");
    while (token != NULL) {
        args[position] = token;
        position++;
        token = my_strtok(NULL, " ");
    }
    args[position] = NULL;
    return args;
}

int execute_command(char **args, char** envp) {
    if (args[0] == NULL) {
        return 1;
    }
    if (my_strcmp(args[0], "exit") == 0) {
        return 1;
    }
    if (my_strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            write(STDERR_FILENO, "cd: expected argument\n", 22);
            return 0;
        } else change_directory(args[1]);
    } else if (my_strcmp(args[0], "pwd") == 0) {
        print_working_directory();
    } else if (my_strcmp(args[0], "setenv") == 0) {
        if (args[1] == NULL || args[2] == NULL) {
            write(STDERR_FILENO, "setenv: expected two arguments\n", 32);
            return 0;
        } else set_environment_variable(args[1], args[2]);
    } else if (my_strcmp(args[0], "env") == 0) {
        print_environment();
    } else if (my_strcmp(args[0], "unsetenv") == 0) {
        if (args[1] == NULL) {
            write(STDERR_FILENO, "unsetenv: expected argument\n", 26);
            return 0;
        } else unset_environment_variable(args[1]);
    } else if (my_strcmp(args[0], "echo") == 0) {
        print_echo(args);
    } else if (my_strcmp(args[0], "which") == 0) {
        which(args);
    } else {
        start_process(args, envp);
    }
    return 0;
}

int start_process(char** args, char** envp) {
    pid_t pid = fork();

    if (pid == 0) { // Child process
        if (execve(args[0], args, envp) == -1) {
            char* path_val = get_env_var(envp);
            if (path_val != NULL) {
                char* path_copy = malloc(my_strlen(path_val) + 1);
                my_strcpy(path_copy, path_val);
                // Split PATH by colons to get individual directories
                char* dir = my_strtok(path_copy, ":");
                while (dir) {
                    char full_path[MAX_INPUT_SIZE];
                    my_strcpy(full_path, dir);
                    my_strcpy(full_path + my_strlen(full_path), "/");
                    my_strcpy(full_path + my_strlen(full_path), args[0]);
                    
                    execve(full_path, args, envp);
                    dir = my_strtok(NULL, ":");
                }
                free(path_copy);
            }
        }
    } else if (pid < 0) {
        perror("fork failed");
        return 0;
    } else { // Parent process
        waitpid(pid, NULL, 0);
    }

    return 1;
}

char* get_env_var(char** envp) {
    int var_len = my_strlen("PATH");
    for (int i = 0; envp[i] != NULL; i++) {
        if (my_strncmp(envp[i], "PATH", var_len) == 0 && envp[i][var_len] == '=') {
            return &envp[i][var_len + 1];
        }
    }
    return NULL;
}