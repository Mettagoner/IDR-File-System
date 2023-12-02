// Last Edited By: Daniel Fewell 
// Last Edited Date: 12/2/2023
// For more in-depth documentation on the functions within this file see: IDR File System Documentation - OPSYS Final Project Document


#include "disklib.h"


// Global variable for the disk file
FILE *disk = NULL;

// Initialize the virtual disk
void init_disk() {
	disk = fopen("virtual_disk.img", "r+b"); // Call the 'fopen' function in C standard lib to open file named 'virtual_disk.img' for reading & writing in binary mode
	if disk = NULL {
	// Create file if it does not already exist
	disk = fopen("virtual_disk.img", "w+b"); // Open file for writing & reading in binary mode
	if (disk == NULL) {
		perror("Error Code 1: Unable to reate virtual disk"); // Print Error Code #1: Unable to create virtual disk
		exit(1);
	}
	fseek(disk, BLOCK_SIZE * NUM_BLOCKS -1, SEEK_SET); // Move the file pointer to the last byte of the calculated size 
	fputc(0, disk); // Writes a single char to the current file pointer position. 0 is the char byte being written. This ensures the file is extended or reduced to our specified 5MB size 
	}
}

//Read a block from the disk into a buffer
int read_block(int block_num, void *block) { // void* block allows us to pass a pointer to a buffer of any data type
	if (block_num < 0 || block_num >= NUM_BLOCKS) { // If conditions are met, return invalid block number
		return -1; // Return failure upon completion
	}

	fseek(disk, block_num * BLOCK_SIZE, SEEK_SET); // Position the file pointer
	fread(block, BLOCK_SIZE, 1, disk); // Use fread function to read data into the buffer (block)
	return 0; // Return success upon completion
}

//Write a block to the virtual disk
int write_block(int block_num, const void *block) {
	if (block_num < 0 || block_num >= NUM_BLOCKS) { // If conditions are met, return invalid block number
		return -1; // Return failure upon completion
	}

	fseek(disk, block_num * BLOCK_SIZE, SEEK_SET); // Position the file pointer
	fwrite(block, BLOCK_SIZE, 1, disk); // Use fwrite function to write data to the position of the file pointer
	return 0; // Return success upon completion
}
