#include "../FSCore/fscore.h"
#include <stdio.h>
#include <string.h>

int main() {
    char command[256];
    char filename[MAX_FILENAME_LENGTH];
    int result;

    while (1) {
        printf("fs> "); // Prompt for input
        fgets(command, sizeof(command), stdin); // Read the command

        if (strncmp(command, "create", 6) == 0) {
            sscanf(command, "create %s", filename);
            result = create_file(filename);
            if (result == 0) {
                printf("File '%s' created successfully.\n", filename);
            } else {
                printf("Error creating file '%s'.\n", filename);
            }
        }
        // Add similar blocks for 'delete', 'read', 'write', and 'list'

        else if (strncmp(command, "exit", 4) == 0) {
            break; // Exit the loop
        }

        else {
            printf("Unknown command.\n");
        }
    }

    return 0;
}
