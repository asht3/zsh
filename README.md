# Welcome to My Zsh
***

## Task
Create your own shell that can read input from standard input, parse the input, then run the parsed command. Use only authorized system calls.

It should be able to support the following commands:  ls, cat, tail, head, env, echo, cd, setenv, unsetenv, env, exit, pwd and which.

## Description
The standard input was read using `getline()` and essential shell built-ins were implemented. Then `fork()` and `execve()` were used to create child processes for external commands.

## Installation
Users can compile the program using the provided Makefile. Once the repository is cloned, navigate to the project directory and run the following command:
```
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
```
```
my_zsh $> ls
file1.txt  file2.c  directory
```
```
my_zsh $> setenv MY_VAR hello
```
```
my_zsh $> cd /tmp
```
```
my_zsh $> cat file.txt
```
```
my_zsh $> exit
```

### The Core Team


<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px' /></span>
