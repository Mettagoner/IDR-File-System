/********************************************************************************
 * Filename:     disklib.h
 * Author:       Daniel Fewell
 * Last Edited:  12/11/2023
 * Description:  Header File for the 8 functions defined in disklib.c
 * ********************************************************************************/

// Include Guards (prevents multiple inclusions and redefinition errors)
#ifndef DISKLIB_H // Has disklib_h not been defined?
#define DISKLIB_H // If disklib_h has not been defined then define disklib.h

#include <stdio.h>
#include <stdlib.h>

// Constants
#define BLOCK_SIZE 4096 // Set the size of each block in bytes
#define NUM_BLOCKS 1280 // Set the number of blocks in the disk (5MB total size)
#define BITMAP_SIZE (NUM_BLOCKS / 8) // Size of bitmap in bytes

// Function declarations
void init_disk(); // Inits disk and bitmap
int read_block(int block_num, void *block); // Reads a block from the disk
int write_block(int block_num, const void *block); // Writes a block to the disk
void init_bitmap(); // Init bitmap from the disk
void save_bitmap(); // Save bitmap to the disk
int find_free_block(); // Find the first free block using the bitmap
void mark_block_used(int blockIndex); // Marks a block as used in the bitmap
void mark_block_free(int blockIndex); // Marks a block as free in the bitmap

#endif
