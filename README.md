# ash

`ash` is a small interactive shell written in C. It reads a command, parses its arguments, runs built-in commands in-process, and executes available external programs from `PATH`.

## Prerequisites

- A POSIX-like environment (such as Linux or macOS)
- A C compiler compatible with `gcc` (GCC or Clang)
- `make`

## Build

From the project root, build the shell with:

```sh
make
```

This creates the `ash` executable and uses `build/` for intermediate object and dependency files.

To remove generated build files and the executable:

```sh
make clean
```

To perform a clean rebuild:

```sh
make re
```

## Run and use

Start the shell after building it:

```sh
./ash
```

At the `$` prompt, enter a command and press Enter. Leave the shell with `exit` or send end-of-file (for example, `Ctrl-D`).

```text
$ pwd
/path/to/ash
$ echo "hello world"
hello world
$ type ls
ls is /usr/bin/ls
$ cd ~
$ exit
```

## Main features

- Built-in commands: `echo`, `exit`, `type`, `pwd`, and `cd`.
- `cd` supports the home directory by omitting its argument and expands paths beginning with `~`.
- Executes external commands found through the `PATH` environment variable.
- Waits for each external command to finish before showing the next prompt.
- Splits arguments on whitespace and supports single quotes, double quotes, and backslash escaping.
- Double-quoted strings support escaping `"`, `\\`, `$`, and backticks with a preceding backslash.
- A straightforward Makefile build with automatic header dependency tracking.

## Current limits

- Input lines are limited to 1,023 characters.
- Commands accept up to 63 arguments, and each parsed argument is limited to 1,023 characters.
- Pipelines, redirection, environment-variable expansion, command substitution, and job control are not implemented.

<!-- ## TO-DO -->

<!-- Add planned work here. -->

