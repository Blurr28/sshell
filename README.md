# Custom Shell Implementation (In Progress)

This is a custom shell implementation created as a part of a personal project. It is designed to mimic the basic functionality of a Unix/Linux shell, with support for both built-in shell commands and external program execution. The project is written in C and makes use of system calls and file descriptors to handle various shell functionalities.

## Features

- **Shell Builtins**: Supports built-in commands like `echo`, `exit`, `type`, `pwd`, and `cd`.
- **External Programs**: Allows execution of external programs with argument parsing.
- **Navigation**: Implements `pwd` and `cd` to navigate the filesystem.
- **Output Redirection**: Supports redirecting output to a file using `>` and `>>`.
- **Quoted Strings**: Handles both single-quoted and double-quoted arguments.
- **Error Handling**: Basic error handling for common shell operations and built-in commands.
