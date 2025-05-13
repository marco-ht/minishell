# minishell

This repository contains my implementation of the **minishell** project, developed as part of the 42 School curriculum. The goal is to build a small, POSIX‑like command‑line shell in C, gaining hands‑on experience with processes, file descriptors, signals, and terminal handling.

> **Note:** This shell is for educational purposes only. It is not intended to replace a production shell.

## Table of Contents

- [Overview](#overview)
- [Common Instructions](#common-instructions)
- [Mandatory Part](#mandatory-part)
  - [Prompt & History](#prompt--history)
  - [Parsing & Execution](#parsing--execution)
  - [Built‑in Commands](#built‑in-commands)
  - [Redirections & Pipes](#redirections--pipes)
  - [Environment & Exit Status](#environment--exit-status)
  - [Signal Handling](#signal-handling)
- [Bonus Part](#bonus-part)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [Error Handling](#error-handling)
- [Acknowledgments](#acknowledgments)
- [Disclaimer](#disclaimer)
- [License](#license)

## Overview

**minishell** implements a subset of Bash features:

- Interactive prompt with command history  
- Parsing of simple commands, quotes, and environment‑variable expansions  
- I/O redirections (`<`, `>`, `>>`, `<<`)  
- Pipelines (`|`)  
- Built‑in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`  
- Signal handling: `Ctrl‑C`, `Ctrl‑D`, `Ctrl‑\`  

The project uses the GNU Readline library for line editing and history management.

## Common Instructions

- **Language & Norm:** Write in C, strictly following the 42 Norm. Norm errors in either mandatory or bonus code yield zero.  
- **Memory:** No leaks in your own code. Readline leaks are acceptable.  
- **Makefile:** Must compile with `cc -Wall -Wextra -Werror` and include rules: `NAME`, `all`, `clean`, `fclean`, `re`. Avoid unnecessary relinking.  
- **Allowed Functions:**  
  - **Readline APIs:** `readline`, `add_history`, `rl_*`  
  - **Process & I/O:** `fork`, `execve`, `wait`, `pipe`, `dup2`, `open`, `read`, `write`, `close`, …  
  - **Signals & Terminal:** `signal`, `sigaction`, `kill`, `tcgetattr`, `tcsetattr`, `ioctl`, …  
  - **Utilities:** `getcwd`, `chdir`, `stat`, `opendir`, `readdir`, `getenv`, `exit`, `perror`, `strerror`, `printf`, `malloc`, `free`  
- **Libft:** You may include your own libft in a `libft/` folder; compile it via your Makefile.

## Mandatory Part

### Prompt & History

- Display a prompt each time (`minishell$ `, for example).  
- Maintain a history of commands via Readline; pressing up/down navigates history.  
- Exit cleanly on `Ctrl‑D`.

### Parsing & Execution

- Tokenize input respecting **single** and **double** quotes:  
  - **Single quotes** prevent interpretation of all special characters.  
  - **Double quotes** prevent interpretation except for `$` (variable expansion).  
- Split commands and their arguments correctly.  
- Search executables in `$PATH` or execute via absolute/relative path.

### Built‑in Commands

Implement the following built‑ins **internally** (no `execve`):

- `echo [-n] [args...]`  
- `cd [directory]`  
- `pwd`  
- `export [KEY[=VALUE] ...]`  
- `unset KEY ...`  
- `env`  
- `exit [n]`

### Redirections & Pipes

- **Redirections:**  
  - `< file` — redirect stdin  
  - `> file` — redirect stdout (truncate)  
  - `>> file` — redirect stdout (append)  
  - `<< DELIM` — here‑document (read until line equals `DELIM`)  
- **Pipelines:** Connect multiple commands with `|`.

### Environment & Exit Status

- Expand `$VAR` to its environment value.  
- Expand `$?` to the last foreground pipeline's exit status.  
- Maintain and modify the environment for `export`/`unset`.

### Signal Handling

- Use **one** global `volatile sig_atomic_t` variable to store the last received signal number—no other globals.  
- **Interactive mode:**  
  - `Ctrl‑C` prints a new prompt on a fresh line.  
  - `Ctrl‑D` exits the shell.  
  - `Ctrl‑\` is ignored.  
- Child processes should inherit default signal behaviors.

## Bonus Part

Implement **perfectly** (mandatory must be flawless first):

- **Command chaining:** `&&`, `||` with correct precedence and parentheses.  
- **Globbing:** Support `*` wildcards for filename expansion in the current directory.

## Project Structure

```
minishell/
├── includes/         # minishell.h, parser.h, exec.h, etc.
├── srcs/             # main.c, parser/*.c, exec/*.c, builtins/*.c, signals/*.c
├── libft/            # (optional) your libft and its Makefile
├── Makefile          # rules: NAME, all, clean, fclean, re (and bonus if used)
├── .gitignore
└── README.md         # this file
```

## Installation

1. **Clone the Repository:**

   ```sh
   git clone https://github.com/yourusername/minishell.git
   cd minishell
   ```

2. **Build the Project:**

   Use the provided Makefile to compile your source files:

   ```sh
   make
   ```

   This command will produce the executable named `minishell`.

## Usage

Run the shell:

```sh
./minishell
```

- Type commands as in Bash.
- Use `<`, `>`, `>>`, `<<`, and `|` as described.
- Use built‑ins without forking.
- Press `Ctrl‑D` or enter `exit` to quit.

## Error Handling

- **Syntax errors** (unclosed quotes, misplaced pipes/redirections) should print `syntax error…` and return a non‑zero status.
- **Command not found** prints `minishell: command: not found`.
- **Redirection failures** print the appropriate `perror` message.
- **Fatal errors** exit with a non‑zero status and free all allocated resources.

## Acknowledgments

- Thanks to the 42 community, mentors, and fellow students for their guidance and support.
- Inspired by the Bash manual and GNU Readline documentation.

## Disclaimer

**IMPORTANT**:
This project was developed solely as part of the educational curriculum at 42 School. The code in this repository is published only for demonstration purposes to showcase my programming and system-level development skills.

**Academic Integrity Notice**:
It is strictly prohibited to copy or present this code as your own work in any academic submissions at 42 School. Any misuse or uncredited use of this project will be considered a violation of 42 School's academic policies.

## License

This repository is licensed under the Creative Commons - NonCommercial-NoDerivatives (CC BY-NC-ND 4.0) license. You are free to share this repository as long as you:

- Provide appropriate credit.
- Do not use it for commercial purposes.
- Do not modify, transform, or build upon the material.

For further details, please refer to the CC BY-NC-ND 4.0 license documentation.

Developed with dedication and in strict adherence to the high standards of 42 School.