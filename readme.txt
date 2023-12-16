Description:
The "IDR File System" project, designed and implemented by Ivana, Dan, and Romy, is a user-space file system in the Unix environment. It operates within a single Unix file, emulating disk operations at a block level and managing basic file operations like creation, listing, adding, and removal. The project comprises three main modules: Disk Library for low-level disk operations, File System Core for file management, and Interface for user interaction. Key functions include initializing disks, reading/writing blocks, and file operations. The project aims to achieve robust error handling and advanced features like directories and efficient storage.


User Guide:


To use the IDR File System project, follow these steps:


1. **Initialize the Disk**: Run the program to automatically initialize the disk.


2. **Create a File**: Type `create [filename]` to create a new file.


3. **Delete a File**: Use `delete [filename]` to remove a file.


4. **Read from a File**: Enter `read [filename] [num_bytes] [offset]` to read a specific number of bytes from a file, starting at a given offset.


5. **Write to a File**: Use `write [filename] [data] [offset]` to write data to a file at a specified offset.


6. **List Files**: Type `list` to display all files.


7. **Exit the Program**: Enter `exit` to close the program and the disk.


The program will continuously prompt for commands with `fs>` until `exit` is entered. Always ensure to close the disk properly before exiting.