// steps to create a shell
// 1. Read input from the user
// 2. Parse the input into commands and arguments
// 3. Execute the commands
    // command execution can be done using fork() and exec() system calls
    // commands to execute: ls - cat - tail - head - pwd - env - setenv - cd
        // ls, cat, tail, head, env (system binary)
        // pwd (built in)
        // cd (built in)
        // exit (built in)
        // setenv (built in)
    // binary have to be exec in your shell, To execute a binary, the exec*() functions
        // execve()
        // create a fork() to create a child process
    // To find the different binary, use the PATH environment variable
        // PATH is a colon-separated list of directories that the shell searches for executable files
        // use getenv("PATH") to get the PATH variable
        // use strtok() to split the PATH variable into individual directories
        // use access() to check if the binary exists in the directory
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
    /*
        |  Read: Read the command from standard input.
        |  Parse: Separate the command string into a program and arguments.
        |  Execute: Run the parsed command.
        |-- (repeat)
    */

    // read command
    char command[1024];
    while (1) {
        printf("my_zsh $> ");
        if (!fgets(command, sizeof(command), stdin)) { // TODO: change to use getline
            break; // EOF
        }
        if (command[0] == '\0') {
            continue;
        }
        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        // parse command
        // execute command
    }

    return 0;
}

void change_directory(char *path) {
    if (chdir(path) != 0) {
        perror("cd failed");
    }
}

void print_working_directory() {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("getcwd failed");
    }
}

void set_environment_variable(char *name, char *value) {
    if (setenv(name, value, 1) != 0) {
        perror("setenv failed");
    }
}