// steps to create a shell
// 1. Read input from the user
// 2. Parse the input into commands and arguments
// 3. Execute the commands
    // command execution can be done using fork() and exec() system calls
    // commands to execute: ls - cat - tail - head - env
        // ls, cat, tail, head, env (system binary)
        // [x] pwd (built in)
        // [x] cd (built in)
        // [x] exit (built in)
        // [x] setenv (built in)
    // binary have to be exec in your shell, To execute a binary, the exec*() functions
        // execve()
        // create a fork() to create a child process
    // To find the different binary, use the PATH environment variable
        // PATH is a colon-separated list of directories that the shell searches for executable files
        // use getenv("PATH") to get the PATH variable
        // use strtok() to split the PATH variable into individual directories
/*
you will either check the PATH for a particular executable and forking the child process or builtin.
Here is an earlier message: make sure you code `pwd`, `cd`, `setenv`, `unsetenv` and `env` and other shell builtins 
you think it would be cool to implement, without deviating too much of the end goal of the project. Rest of it, run 
the one within the system, with `exec` family functions

Authorized functions:
    malloc, free, exit, opendir, readdir, closedir, getcwd, chdir
    fork, stat, lstat, fstat, open, close, getline
    read, write, execve, isatty, wait, waitpid
    wait3, wait4, signal, kill, getpid, strerror, perror.

Requirements:
    Your code must be compiled with the flags -Wall -Wextra -Werror.
    No memory leaks are allowed.
    Parser can be a split on spaces.
    Multiline macros are forbidden
    Include another .c is forbidden
    Macros with logic (while/if/variables/...) are forbidden
*/

#include "main.h"

int main (int argc, char** argv, char** envp) {
    if (argc > 1) {
        write_error(argv[0]);
        return EXIT_FAILURE;
    }

    // printf("\n███████╗███████╗██╗  ██╗\n");
    // printf("╚══███╔╝██╔════╝██║  ██║\n");
    // printf("  ███╔╝ ███████╗███████║\n");
    // printf(" ███╔╝  ╚════██║██╔══██║\n");
    // printf("███████╗███████║██║  ██║\n");
    // printf("╚══════╝╚══════╝╚═╝  ╚═╝\n");

    // Read command
    char* input = NULL;
    size_t input_len = 0;
    ssize_t read_size;

    while (1) {
        if (isatty(STDIN_FILENO)) {
            write(STDOUT_FILENO, "my_zsh $> ", 10);
        }
        read_size = getline(&input, &input_len, stdin);

        // if (read_size == -1) {
        //     break; // EOF
        // }

        // Remove newline character
        if (input[read_size - 1] == '\n') {
            input[read_size - 1] = '\0';
        }

        // Parse command
        char** args = parse_input(input);
        if (args == NULL) {
            free(args);
            continue;
        }

        // Execute command
        if (execute_command(args, envp) > 0) {
            free(args);
            break;
        }
        free(args);
    }
    free(input);

    return 0;
}

void write_error(const char* msg) {
    write(STDERR_FILENO, "Usage: ", 7);
    write(STDERR_FILENO, msg, my_strlen(msg));
    write(STDERR_FILENO, "\n", 1);
}

char** parse_input(char *input) {
    if (input == NULL) return NULL;

    char** args = malloc(sizeof(char*) * (MAX_INPUT_SIZE / 2 + 1));
    if (args == NULL) {
        perror("malloc failed");
        return NULL;
    }
    char* token;
    int position = 0;
    token = my_strtok(input, " "); // TODO: change to my_strtok
    while (token != NULL) {
        args[position] = token;
        position++;
        token = my_strtok(NULL, " "); // TODO: change to my_strtok
    }
    args[position] = NULL;
    return args;
}

int execute_command(char **args, char** envp) {
    if (args[0] == NULL) {
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
        print_environment(envp);
    } else if (my_strcmp(args[0], "ls") == 0) {
        list_directory(args[1]);
    } else if (my_strcmp(args[0], "cat") == 0) {
        concat_files(args);
    } else if (my_strcmp(args[0], "head") == 0) {
        display_file_head(args);
    } else if (my_strcmp(args[0], "tail") == 0) {
        display_file_tail(args);
    }
     else if (my_strcmp(args[0], "exit") == 0) {
        return 1;
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
            // return envp[i] + var_len;
            return &envp[i][var_len + 1];
        }
        // if (my_strcmp(envp[i], "PATH=") == 0) {
        //     return envp[i] + var_len;
        // }
    }
    return NULL;
}

void change_directory(char *path) {
    if (chdir(path) != 0) {
        perror("cd failed");
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

void list_directory(char* path) {
    DIR* dir;
    struct dirent* entry;

    if (path == NULL) path = ".";

    dir = opendir(path);
    if (dir == NULL) perror("ls: failed to open directory");

    while ((entry = readdir(dir)) != NULL) {
        write(STDOUT_FILENO, entry->d_name, my_strlen(entry->d_name));
        write(STDOUT_FILENO, "\n", 1);
    }

    closedir(dir);
}

void concat_files(char **args) {
    if (args[1] == NULL) {
        // perror("cat: expected argument");
        write(STDERR_FILENO, "cat: expected argument\n", 22);
        return;
    }

    // loop thorugh args and open each file
        // write each file to stdout
        // if '>' is found, write to the output file instead (extra)
            // search for > first, then open the file after it
            // go back to previous files, open them, then write into the file
    for (int i = 1; args[i] != NULL; i++) {
        int file_fd = open(args[i], O_RDONLY);
        if (file_fd < 0) {
            perror("cat: failed to open file");
            continue;
        }

        char buffer[MAX_INPUT_SIZE];
        ssize_t bytes_read;
        while ((bytes_read = read(file_fd, buffer, sizeof(buffer))) > 0) {
            write(STDOUT_FILENO, buffer, bytes_read);
        }
        close(file_fd);
    }
}

void display_file_head(char **args) {
    if (args[1] == NULL) {
        write(STDERR_FILENO, "head: expected argument\n", 24);
        return;
    }

    int file_fd = open(args[1], O_RDONLY);
    if (file_fd < 0) {
        perror("head: failed to open file");
        return;
    }

    char char_buf;
    ssize_t bytes_read;
    int line_count = 0;
    while ((bytes_read = read(file_fd, &char_buf, 1)) > 0) {
        write(STDOUT_FILENO, &char_buf, bytes_read);
        if (char_buf == '\n') {
            line_count++;
        }
        if (line_count == 10) break;
    }

    close(file_fd);
}

void display_file_tail(char **args) {
    if (args[1] == NULL) {
        write(STDERR_FILENO, "tail: expected argument\n", 24);
        return;
    }

    int file_fd = open(args[1], O_RDONLY);
    if (file_fd < 0) {
        perror("tail: failed to open file");
        return;
    }

    // char buffer[MAX_INPUT_SIZE];
    // size_t bytes_read;
    // int line_count = 0;
    // while ((bytes_read = read(file_fd, buffer, sizeof(buffer)))> 0) {
    //     for (size_t i = 0; i < bytes_read; i++) {
    //         if (buffer[i] == '\n' || buffer[i] == '\0') {
    //             line_count++;
    //         }
    //     }
    // }
    char ch;
    int total_lines = 0;
    int last_char = 0;
    
    while (read(file_fd, &ch, 1) > 0) {
        last_char = ch;
        if (ch == '\n') total_lines++;
    }
    
    // File does not end with a newline, count the last line
    if (last_char != '\n' && last_char != 0) {
        total_lines++;
    }
    close(file_fd);

    file_fd = open(args[1], O_RDONLY);
    int lines_to_skip = (total_lines > 10) ? total_lines - 10 : 0;
    int current_line = 0;
    int output_started = (lines_to_skip == 0) ? 1 : 0;

    while (read(file_fd, &ch, 1) > 0) {
        if (output_started) {
            write(STDOUT_FILENO, &ch, 1);
        }
        if (ch == '\n') {
            current_line++;
        }
        if (current_line == lines_to_skip) {
            output_started = 1;
        }
    }

    // int skip_lines = line_count > 9 ? line_count - 9 : 0;
    // char char_buf;
    // line_count = 0;
    // while (read(file_fd, &char_buf, 1) > 0) {
    //     if (char_buf == '\n') {
    //         line_count++;
    //     }
    //     if (line_count > skip_lines) {
    //         write(STDOUT_FILENO, &char_buf, 1);
    //     }
    // }
    // while ((bytes_read = read(file_fd, buffer, sizeof(buffer)))> 0) {
    //     for (size_t i = 0; i < bytes_read; i++) {
    //         if (buffer[i] == '\n') {
    //             line_count++;
    //         }
    //         if (line_count > skip_lines) {
    //             write(STDOUT_FILENO, &buffer[i], 1);
    //         }
    //     }
    // }
    close(file_fd);    
}

void print_environment(char **envp) {
    for (int i = 0; envp[i] != NULL; i++) {
        write(STDOUT_FILENO, envp[i], my_strlen(envp[i]));
        write(STDOUT_FILENO, "\n", 1);
    }
}

int my_strlen(const char* str_1) {
    int length = 0;

    while (*str_1 != '\0') {
        length++;
        str_1++;
    }

    return length;
}

char* my_strchr(const char* str, int search_char) {
    while (*str) { 
        if (*str == (char)search_char) {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

char* my_strtok(char* str, const char* delim) {
    static char* saved_ptr = NULL;
    if (str == NULL) {
        if (saved_ptr == NULL) {
            return NULL;
        }
        str = saved_ptr;
    }
    
    // Skip leading delimiters
    while (*str && my_strchr(delim, *str)) {
        str++;
    }
    
    if (*str == '\0') {
        saved_ptr = NULL;
        return NULL;
    }
    
    char* token_start = str;
    
    // Find the end of the token
    while (*str && !my_strchr(delim, *str)) {
        str++;
    }
    
    if (*str) {
        *str = '\0';
        saved_ptr = str + 1;
    } else {
        saved_ptr = NULL;
    }
    
    return token_start;
}

int my_strcmp(const char* str_1, const char* str_2) {
    while (*str_1 != '\0' || *str_2 != '\0') {
        if (*str_1 < *str_2) {
            return -1;
        }
        else if (*str_1 > *str_2) {
            return 1;
        }
        str_1++;
        str_2++;
    }
    return 0;
}

int my_strncmp(const char *str1, const char *str2, int n) {
    for (int i = 0; i < n; i++) {
        if (str1[i] == '\0' && str2[i] == '\0') {
            return 0;
        }
        if (str1[i] == '\0') {
            return -1;
        }
        if (str2[i] == '\0') {
            return 1;
        }
        if (str1[i] < str2[i]) {
            return -1;
        }
        if (str1[i] > str2[i]) {
            return 1;
        }
    }
    return 0;
}

char* my_strcpy(char* dest, const char* src) {
    char* original_dest = dest;
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
    return original_dest;
}