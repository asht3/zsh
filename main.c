// steps to create a shell
// 1. Read input from the user
// 2. Parse the input into commands and arguments
// 3. Execute the commands
    // command execution can be done using fork() and exec() system calls
    // commands to execute:
        // ls
        // pwd
        // cd (built in)
        // echo
        // exit (built in)
    // binary have to be exec in your shell, To execute a binary, the exec*() functions
        // execve()
        // create a fork() to create a child process
    // built in commands
        // cd
        // exit
        // export
        // unset
        // history
        // alias
        // unalias
    // To find the different binary, use the PATH environment variable
        // PATH is a colon-separated list of directories that the shell searches for executable files
        // use getenv("PATH") to get the PATH variable
        // use strtok() to split the PATH variable into individual directories
        // use access() to check if the binary exists in the directory
/*
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
int main (int argc, char** argv, char** envp) {
    // zsh shell main function
    /*
        |  Read: Read the command from standard input.
        |  Parse: Separate the command string into a program and arguments.
        |  Execute: Run the parsed command.
        |-- (repeat)
    */

    // read command
    char command[1024];
    // while (1) {
    //     printf("my_zsh> ");
    //     if (!fgets(command, sizeof(command), stdin)) {
    //         break; // Exit on EOF
    //     }
    // }

    // parse command
    // char* args[100];
    // int i = 0;
    // char* token = strtok(command, " \n");
    // while (token != NULL) {
    //     args[i++] = token;
    //     token = strtok(NULL, " \n");
    // }
    // args[i] = NULL;
    return 0;
}