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

char* expand_variable(char *token) {
    extern char **environ;  // Use the current environment
    
    char *var_name = token + 1;  // Skip $
    
    for (int i = 0; environ[i] != NULL; i++) {
        int var_len = my_strlen(var_name);
        if (my_strncmp(environ[i], var_name, var_len) == 0 && environ[i][var_len] == '=') {
            return environ[i] + var_len + 1;  // Return value after =
        }
    }
    return ""; 
}

int execute_command(char **args, char** envp) {
    if (args[0] == NULL || my_strcmp(args[0], "exit") == 0 || my_strcmp(args[0], "quit") == 0) {
        return 1;
    }

    for (int i = 0; args[i] != NULL; i++) {
        if (args[i][0] == '$') {
            char *expanded = expand_variable(args[i]);
            // Might need to handle memory allocation here
            args[i] = expanded;
        }
    }

    if (my_strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            write(STDERR_FILENO, "cd: expected argument\n", 22);
            return 0;
        } else change_directory(args[1]);
    } else if (my_strcmp(args[0], "pwd") == 0) {
        print_working_directory();
    } else if (my_strcmp(args[0], "setenv") == 0) {
        if (args[1] == NULL) {
            write(STDERR_FILENO, "setenv: expected argument\n", 26);
        } else {
            // Handle "setenv VAR VALUE" and "setenv VAR=VALUE"
            char *name = args[1];
            char *value = args[2];
            
            char *equals_sign = my_strchr(args[1], '=');
            if (equals_sign != NULL) {
                *equals_sign = '\0';  // Split into name and value
                value = equals_sign + 1;
            }
            
            if (value == NULL) {
                write(STDERR_FILENO, "setenv: expected value\n", 23);
            } else {
                set_environment_variable(name, value);
            }
        }
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
    int status;

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
            write(STDERR_FILENO, "Command not found: ", 19);
            write(STDERR_FILENO, args[0], my_strlen(args[0]));
            write(STDERR_FILENO, "\n", 1);
            exit(EXIT_FAILURE);
        }
    } else if (pid < 0) {
        perror("fork failed");
        return 0;
    } else { // Parent process
        // int result = waitpid(pid, &status, 0);
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int signal = WTERMSIG(status);
            if (signal == SIGSEGV) {
                write(STDERR_FILENO, "Segmentation fault\n", 19);
            }
        }
        // printf("DEBUG: waitpid returned: %d, child exit status: %d\n", result, status);
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