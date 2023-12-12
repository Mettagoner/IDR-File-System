#include "../FSCore/fscore.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For dynamic memory allocation

int main() {
    char command[256];
    char filename[MAX_FILENAME_LENGTH];
    char *buffer; // Buffer for read and write operations
    int num_bytes, offset, result;

    // Initialize the disk
    init_disk();

    while (1) {
        printf("fs> ");
        fgets(command, sizeof(command), stdin);

        if (strncmp(command, "create", 6) == 0) {
            sscanf(command, "create %255s", filename);
            result = create_file(filename);
            printf(result == 0 ? "File '%s' created successfully.\n" : "Error creating file '%s'.\n", filename);
        } 
        else if (strncmp(command, "delete", 6) == 0) {
            sscanf(command, "delete %255s", filename);
            result = delete_file(filename);
            printf(result == 0 ? "File '%s' deleted successfully.\n" : "Error deleting file '%s'.\n", filename);
        }
        else if (strncmp(command, "read", 4) == 0) {
            sscanf(command, "read %255s %d %d", filename, &num_bytes, &offset);
            buffer = (char *)malloc(num_bytes);
            if (buffer == NULL) {
                printf("Error allocating memory.\n");
                continue;
            }
            result = read_file(filename, buffer, num_bytes, offset);
            if (result >= 0) {
                printf("Read %d bytes from file '%s': %.*s\n", result, filename, result, buffer);
            } else {
                printf("Error reading file '%s'.\n", filename);
            }
            free(buffer);
        }
        else if (strncmp(command, "write", 5) == 0) {
            // Example write command: write filename.txt Hello, world! 0
            char *data = strstr(command, " ") + 1; // Get the rest of the command after 'write'
            char *next_space = strstr(data, " ");
            if (next_space) {
                *next_space = '\0'; // Separate filename and data
                strncpy(filename, data, MAX_FILENAME_LENGTH);
                data = next_space + 1;
                next_space = strstr(data, " ");
                if (next_space) {
                    *next_space = '\0'; // Separate data and offset
                    offset = atoi(next_space + 1);
                } else {
                    offset = 0; // Default to offset 0
                }
                result = write_file(filename, data, strlen(data), offset);
                printf(result >= 0 ? "Wrote %d bytes to file '%s'.\n" : "Error writing to file '%d'.\n", result, filename);
            }
        }
        else if (strncmp(command, "list", 4) == 0) {
            list_files();
        }
        else if (strncmp(command, "exit", 4) == 0) {
            break;
        }
        else {
            printf("Unknown command.\n");
        }
    }

    // Close the disk before exiting
    close_disk();

    return 0;
}
