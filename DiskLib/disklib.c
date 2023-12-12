/********************************************************************************
 * Filename: disklib.c
 * Author: Daniel Fewell
 * Last Edited: 12/11/2023
 * Description: Implementation of the 8 functions declared in disklib.h
 ********************************************************************************/

#include "disklib.h"
#include <limits.h> // For CHAR_BIT
#include <string.h> // For memset

// ... rest of your code ...

// Global variable for the disk file
FILE *disk = NULL;
unsigned char bitmap[BITMAP_SIZE]; // Bitmap array



// Initialize the virtual disk:
void init_disk() {
    disk = fopen("virtual_disk.img", "r+b");
    if (disk == NULL) {
        disk = fopen("virtual_disk.img", "w+b");
        if (disk == NULL) {
            perror("Error Code 1: Unable to create virtual disk");
            exit(1);
        }
        // Initialize disk to specified size and create bitmap
        fseek(disk, BLOCK_SIZE * NUM_BLOCKS - 1, SEEK_SET);
        fputc(0, disk);
        memset(bitmap, 0, BITMAP_SIZE); // Initialize all blocks as free
        save_bitmap(); // Save the initial bitmap to disk
    } else {
        init_bitmap(); // Load bitmap from disk
    }
}



// Bitmap Functions
void init_bitmap() {
    fseek(disk, 0, SEEK_SET);// Set the file position to the beginning of the disk file
    size_t bytesRead = fread(bitmap, sizeof(char), BITMAP_SIZE, disk); // Read the bitmap data from the disk into the bitmap array
    if (bytesRead != BITMAP_SIZE) { // Check if the number of bytes read is equal to the expected bitmap size
        perror("Error reading bitmap from disk"); // If not, output an error message and exit the program
        exit(1);
    }
}

void save_bitmap() {
    fseek(disk, 0, SEEK_SET);// Set the file position to the beginning of the disk file where the bitmap is stored
    size_t bytesWritten = fwrite(bitmap, sizeof(char), BITMAP_SIZE, disk); // Write the bitmap data from the bitmap array to the disk
    if (bytesWritten != BITMAP_SIZE) { // Check if the number of bytes written is equal to the expected bitmap size
        perror("Error writing bitmap to disk"); // If not, output an error message and exit the program
        exit(1);
    }
    fflush(disk); // Flush the output buffer to ensure all data is written to the disk
}

int find_free_block() {
    for (int i = 0; i < BITMAP_SIZE * CHAR_BIT; ++i) { // Iterate through each bit in the bitmap
        int byteIndex = i / CHAR_BIT; // Calculate the byte index in the bitmap array
        int bitIndex = i % CHAR_BIT; // Calculate the bit index within the byte
        if (!(bitmap[byteIndex] & (1 << bitIndex))) { // Check if the bit is 0 (free)
            return i; // Return the index of the free block
        }
    }
    return -1; // No free block found
}

void mark_block_used(int blockIndex) {
    int byteIndex = blockIndex / CHAR_BIT; // Calculate the byte index in the bitmap array
    int bitIndex = blockIndex % CHAR_BIT; // Calculate the bit index within the byte
    bitmap[byteIndex] |= (1 << bitIndex); // Set the bit at blockIndex to 1 (used)
    save_bitmap(); // Save the updated bitmap to the disk
}

void mark_block_free(int blockIndex) {
    int byteIndex = blockIndex / CHAR_BIT; // Calculate the byte index in the bitmap array
    int bitIndex = blockIndex % CHAR_BIT; // Calculate the bit index within the byte
    bitmap[byteIndex] &= ~(1 << bitIndex); // Set the bit at blockIndex to 0 (free)
    save_bitmap(); // Save the updated bitmap to the disk
}



// Read_block, write_block functions here:
int read_block(int block_num, void *block) { // void* block allows us to pass a pointer to a buffer of any data type
        if (block_num < 0 || block_num >= NUM_BLOCKS) { // If conditions are met, return invalid block number
                return -1; // Return failure upon completion
        }

        fseek(disk, block_num * BLOCK_SIZE, SEEK_SET); // Position the file pointer
        fread(block, BLOCK_SIZE, 1, disk); // Use fread function to read data into the buffer (block)
        return 0; // Return success upon completion
}

int write_block(int block_num, const void *block) {
        if (block_num < 0 || block_num >= NUM_BLOCKS) { // If conditions are met, return invalid block number
                return -1; // Return failure upon completion
        }

        fseek(disk, block_num * BLOCK_SIZE, SEEK_SET); // Position the file pointer
        fwrite(block, BLOCK_SIZE, 1, disk); // Use fwrite function to write data to the position of the file pointer
        return 0; // Return success upon completion
}
