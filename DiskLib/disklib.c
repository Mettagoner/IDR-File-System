#include "disklib.h"
#include <limits.h> // For CHAR_BIT
#include <string.h> // For memset

// Global variable for the disk file
FILE *disk = NULL;
unsigned char bitmap[BITMAP_SIZE]; // Bitmap array

void init_disk() {
    disk = fopen("virtual_disk.img", "r+b");
    if (disk == NULL) {
        disk = fopen("virtual_disk.img", "w+b");
        if (disk == NULL) {
            perror("Error Code 1: Unable to create virtual disk");
            exit(1);
        }
        if (fseek(disk, BLOCK_SIZE * NUM_BLOCKS - 1, SEEK_SET) != 0) {
            perror("Error setting disk size");
            exit(1);
        }
        fputc(0, disk);
        memset(bitmap, 0, BITMAP_SIZE);
        save_bitmap();
    } else {
        init_bitmap();
    }
}

void close_disk() {
    if (disk != NULL) {
        fclose(disk);
        disk = NULL;
    }
}

void init_bitmap() {
    if (fseek(disk, 0, SEEK_SET) != 0) {
        perror("Error seeking to start of disk");
        exit(1);
    }
    size_t bytesRead = fread(bitmap, sizeof(char), BITMAP_SIZE, disk);
    if (bytesRead != BITMAP_SIZE) {
        perror("Error reading bitmap from disk");
        exit(1);
    }
}

void save_bitmap() {
    if (fseek(disk, 0, SEEK_SET) != 0) {
        perror("Error seeking to start of disk");
        exit(1);
    }
    size_t bytesWritten = fwrite(bitmap, sizeof(char), BITMAP_SIZE, disk);
    if (bytesWritten != BITMAP_SIZE) {
        perror("Error writing bitmap to disk");
        exit(1);
    }
    if (fflush(disk) != 0) {
        perror("Error flushing disk");
        exit(1);
    }
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

int read_block(int block_num, void *block) {
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
