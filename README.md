# Journal
Journal is a C project for a Bachelor Degree course that allows formatting a text file (given parameters) to resemble newspaper-style columns.

Installation
To compile the program, navigate to the folder where the source code is located and type the following command in the terminal:
```make``` 
An executable file will be generated in the same directory.

To delete the executable file, you can use the following command:

```bash
make clean
``` 

## Usage

To run the program, use the following command in the terminal:

```bash
./journal
```
Followed by the parameters:
- File name (specify the relative or absolute path): string
- Number of rows per column (>0): int (length of a column)
- Number of characters per column (>0): int (width of a column)
- Number of columns per page (>0): int (width of the page)
- Number of spaces between columns (>0): int (spacing between columns)

Any error during parameter insertion will be reported on the screen, specifying which parameter was invalid.

Possible parameter input errors:
- Unable to open the file with the provided name
- A numerical character was input instead of the file name
- A character or string was entered instead of an integer
- The integer corresponding to a numerical parameter is <= 0
- A word in the file is longer than the column width parameter


## Examples

For example, the following code executes the program with:

- File name -> file.txt
- Number of rows per column -> 10
- Number of characters per column -> 25
- Number of columns per page -> 5
- Number of spaces between columns -> 8
```bash
./journal file.txt 10 25 5 8
``` 

## Composition

The project consists of 5 files:

- journal.c This file contains the main program. It reads the input file and assembles individual lines that will make up the rows of the columns in the output file.

- struct.c This file manages all operations related to the data structure (inserting rows, creating columns, memory deallocation, counting rows and columns). It also includes a function to write a page to the file once the page composition is complete.

- manage_text.c This file contains functions for string manipulation (alignment, counting effective characters, checking if a character is UTF-8 or not, and copying substrings).

- struct.h This header file defines the structures of the linked list nodes for columns and rows. It also includes function signatures implemented in struct.c.

- manage_text.h This header file contains the function signatures implemented in manage_text.c.


## Versions

There are two versions available:

#### Single-process
#### Multi-process
These versions can be found in the singleProcess and multiProcess directories, respectively.

Both versions share the same structure, core algorithm, and functionality, but they differ in how they manage processes. The multi-process version uses 3 concurrent processes.

The multi-process version is implemented in journal.c and struct.c using 3 processes: the parent and child processes.

- The parent process reads one byte at a time from the input file and writes it to a pipe.
- The child process reads the bytes from the pipe, processes them into rows, and calls the addRow function to add a row to the data structure.
- If the maximum number of columns per page and rows in the last column is reached, the child process creates another child process to write the page, while the parent continues formatting rows.

## Libraries used

Tutte le librerie utilizzate sono standard C.

All libraries used are standard C libraries.

- **stdio.h**: Standard input/output library. Used for printf().
- **stdlib.h**: Provides functions for dynamic memory allocation, error handling, and utility functions. Used for malloc() and exit().
- **string.h**: Contains functions for string manipulation. Used for strcpy() and strlen().
- **unistd.h**: Provides access to Unix operating system functions, such as process and pipe management. Used for fork() and pipe().
- **sys/wait.h**: Library for process management, used for wait() to wait for a child process to terminate.
- **fcntl.h**: Library for file descriptor management, used for open() to open and create files.

_@author: Francesco D'Aprile_
