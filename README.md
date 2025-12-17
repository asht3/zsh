# Welcome to My Zsh
***

## Task
Create your own shell that can read input from standard input, parse the input, then run the parsed command. Use only authorized system calls.

It should be able to support the following commands:  ls, cat, tail, head, env, echo, cd, setenv, unsetenv, env, exit, pwd and which.

## Description
The standard input was read using `getline()` and essential shell built-ins were implemented. Then `fork()` and `execve()` were used to create child processes for external commands. Implmented custom utility functions for tasks such as `quicksort` to sort files by date and time.

### Supported Built-in Commands
- cd - Change directory
- pwd - Print working directory
- echo - Display text to stdout
- exit - Terminate the shell session
- env - Display or manipulate environment variables
- setenv - Set environment variables
- unsetenv - Remove environment variables
- which - Locate command executables

### Supported External Commands (via execve)
- ls with flags: -l, -a, -t
- cat - Concatenate and display files
- head - Output first part of files
- tail - Output last part of files

## Installation
Users can compile the program using the provided Makefile. Once the repository is cloned, navigate to the project directory and run the following command:
```
git clone <repository-url>
cd zsh
make
```

## Usage
After compiling the code, run the following commands.
```
./my_zsh
```
From here you are able to use any of the supported commands listed above
```
my_zsh $> pwd
/home/user/projects

my_zsh $> ls -la
total 24
drwxr-xr-x  3 user user 4096 Nov 15 10:30 .
drwxr-xr-x 18 user user 4096 Nov 15 09:15 ..
-rw-r--r--  1 user user  567 Nov 15 10:25 file1.txt
-rw-r--r--  1 user user 1234 Nov 15 10:20 file2.c
drwxr-xr-x  2 user user 4096 Nov 15 09:45 directory

my_zsh $> setenv MY_VARIABLE "Hello World"
my_zsh $> echo $MY_VARIABLE
Hello World

my_zsh $> cd tmp
my_zsh $> cat example.txt
This is example file content.

my_zsh $> which ls
/bin/ls

my_zsh $> exit
```

### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>
