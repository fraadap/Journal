# 📄 Journal Management Project

This project implements core functionalities for a journal management system in C. It provides mechanisms to format text input, structure data into rows, and handle file operations using both single-process and multi-process approaches.

---

## 🔍 Overview

The project is composed of several components and header files:

- **struct.c / struct.h:**  
  Implements functions for managing data structures.
- **manage_text.c / manage_text.h:**  
  Contains functions for processing and formatting text.

---

## 💻 Versions

There are two versions available:

1. **Single-process Version:**  
   - Located in the `singleProcess` directory.  
   - Implements the core algorithm in a single process.

2. **Multi-process Version:**  
   - Located in the `multiProcess` directory.  
   - Uses three concurrent processes for improved performance:
     - The **parent process** reads one byte at a time from the input file and writes it to a pipe.
     - The **child process** reads bytes from the pipe, processes them into rows, and calls the `addRow` function to add a row to the data structure.
     - When the maximum number of columns per page and rows in the last column is reached, the child process spawns an additional child process to write the page, while the parent continues formatting rows.
   - Main implementations are in `journal.c` and `struct.c`.

---

## 🛠️ Libraries Used

All libraries used in this project are standard C libraries:

- **stdio.h:**  
  Standard I/O library (e.g., `printf()` for console output).
- **stdlib.h:**  
  Provides functions for dynamic memory allocation (`malloc()`), error handling, and utility functions (e.g., `exit()`).
- **string.h:**  
  Contains functions for string manipulation (e.g., `strcpy()` and `strlen()`).
- **unistd.h:**  
  Provides access to Unix OS functions such as process management and pipe operations (`fork()`, `pipe()`).
- **sys/wait.h:**  
  Used for process management (e.g., `wait()` to wait for a child process termination).
- **fcntl.h:**  
  Provides functions for file descriptor operations (e.g., `open()` for file opening and creation).

---

## 📝 How It Works

### Single-process Version
- Implements the core algorithm within a single process.
- Simplifies process management by processing the entire input sequentially.

### Multi-process Version
- **Process Management:**  
  Utilizes three processes:
  - **Parent Process:**  
    Reads bytes from the input file and writes them to a pipe.
  - **Child Process:**  
    Reads bytes from the pipe, formats them into rows, and calls the `addRow` function to update the data structure.
  - **Additional Child Process (spawned as needed):**  
    Writes a complete page when the maximum allowed columns per page and rows are reached, while the parent continues processing new rows.
- **Implementation Files:**  
  - `journal.c` and `struct.c` handle the multi-process logic.
  
---

## 👤 Author

- **Francesco D'Aprile**

---

## ⚙️ Building and Running

### Prerequisites
- A C compiler (e.g., `gcc`)
- Unix-like operating system (for access to standard C libraries and Unix-specific functions)

### Build Instructions
1. **Clone the Repository:**
    ```bash
    git clone https://github.com/your-username/journal-management.git
    cd journal-management
    ```

2. **Build the Project:**
    - Navigate to the appropriate version directory (`singleProcess` or `multiProcess`).
    - Use the provided Makefile:
    ```bash
    make
    ```

3. **Run the Executable:**
    - For the multi-process version:
    ```bash
    ./your_executable_name
    ```

---

## 📑 Conclusion

This project demonstrates efficient journal management using both single-process and multi-process approaches in C. The multi-process version leverages concurrent processing through pipes and process management to handle larger inputs and write operations. All components are developed using standard C libraries, ensuring portability and ease of integration.

---

Feel free to contribute, suggest improvements, or raise issues on the [GitHub repository]([https://github.com/fraadap/Journal/).

