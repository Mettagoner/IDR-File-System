// Last Edited By: Daniel Fewell
// Last Edited Date: 12/2/2023

// Include Guards (prevents multiple inclusions and redefinition errors)
#ifndef DISKLIB_H // Has disklib_h not been defined?
#define DISKLIB_H // If disklib_h has not been defined then define disklib.h

#include <stdio.h>
#include <stdlib.h>

// Constants
#define BLOCK_SIZE 4096 // Set the size of each block in bytes
#define NUM_BLOCKS 1280 // Set the number of blocks in the disk (5MB total size)

// Function declarations
void init_disk();
int read_block(int block_num, void *block);
int write_block(int block_num, const void *block);

#endif
