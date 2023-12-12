/********************************************************************************
 * Filename:     fscore.c
 * Author:       Daniel Fewell
 * Last Edited:  12/11/2023
 * Description:  Implementation of the 5 functions contained in fscore.h
 * ********************************************************************************/

#include "fscore.h"
#include <limits.h> // for CHAR_BIT
#include <string.h> // for strncpy, memset, strncmp, memcpy

// Define min and max if they're not already defined
#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

int save_metadata(const FileMetadata *metadata) {
    int metadata_block = (BITMAP_SIZE * CHAR_BIT) / BLOCK_SIZE; // Calculate the block number where metadata starts
    if (write_block(metadata_block, metadata) == -1) { // Write metadata to the calculated block
        return -1; // Error handling
    }
    return 0; // Metadata saved successfully
}

int create_file(const char *filename) {
    // Check if file already exists (this requires implementation based on your filesystem structure)

    // Find a free block for the new file
    int free_block = find_free_block();
    if (free_block == -1) {
        return -1; // No free blocks available
    }

    // Initialize file metadata
    FileMetadata metadata;
    strncpy(metadata.filename, filename, MAX_FILENAME_LENGTH);
    metadata.size = 0;
    memset(metadata.blockIndices, -1, sizeof(metadata.blockIndices)); // Initialize block indices to -1
    metadata.blockIndices[0] = free_block; // Assign the first free block
    time(&metadata.creationTime);
    metadata.modificationTime = metadata.creationTime;

    // Save the file metadata
    if (save_metadata(&metadata) != 0) {
        return -1; // Error saving metadata
    }

    // Mark the block as used
    mark_block_used(free_block);

    return 0; // File created successfully
}

int delete_file(const char *filename) {
    // Calculate the starting block index for metadata (right after the bitmap)
    int metadata_block_index = BITMAP_SIZE / BLOCK_SIZE;
    if (BITMAP_SIZE % BLOCK_SIZE > 0) {
        metadata_block_index++; // Increment if bitmap doesn't align perfectly with block size
    }
    FileMetadata metadata;
    int found = 0;

    for (int i = 0; i < NUM_BLOCKS; ++i) { // Iterate through blocks to find metadata
        if (read_block(metadata_block_index + i, &metadata) == -1) {
            break; // Error or end of metadata region
        }
        if (strncmp(metadata.filename, filename, MAX_FILENAME_LENGTH) == 0) {
            found = 1; // File found
            break;
        }
    }

    if (!found) {
        return -1; // File not found
    }

    // Free up the blocks used by the file
    for (int i = 0; i < MAX_BLOCKS_PER_FILE; ++i) {
        if (metadata.blockIndices[i] != -1) {
            mark_block_free(metadata.blockIndices[i]);
        }
    }

    // Clear the metadata
    memset(&metadata, 0, sizeof(metadata));
    write_block(metadata_block_index, &metadata); // Save the cleared metadata

    return 0; // File deleted successfully
}

int read_file(const char *filename, void *buffer, int num_bytes, int offset) {
    // Calculate the starting block index for metadata (right after the bitmap)
    int metadata_block_index = BITMAP_SIZE / BLOCK_SIZE;
    if (BITMAP_SIZE % BLOCK_SIZE > 0) {
        metadata_block_index++; // Increment if bitmap doesn't align perfectly with block size
    }
    FileMetadata metadata;
    int found = 0;

    for (int i = 0; i < NUM_BLOCKS; ++i) { // Iterate through blocks to find metadata
        if (read_block(metadata_block_index + i, &metadata) == -1) {
            break; // Error or end of metadata region
        }
        if (strncmp(metadata.filename, filename, MAX_FILENAME_LENGTH) == 0) {
            found = 1; // File found
            break;
        }
    }

    if (!found) {
        return -1; // File not found
    }

    // Read data from file's blocks
    int start_block = offset / BLOCK_SIZE;
    int block_offset = offset % BLOCK_SIZE;
    int bytes_read = 0;
    char block[BLOCK_SIZE];

    while (bytes_read < num_bytes && metadata.blockIndices[start_block] != -1) {
        read_block(metadata.blockIndices[start_block], &block);
        int to_read = min(BLOCK_SIZE - block_offset, num_bytes - bytes_read);
        memcpy(buffer + bytes_read, block + block_offset, to_read);
        
        bytes_read += to_read;
        start_block++;
        block_offset = 0; // Reset block offset for subsequent blocks
    }

    return bytes_read; // Return the number of bytes actually read
}

int write_file(const char *filename, const void *buffer, int num_bytes, int offset) {
    // Calculate the starting block index for metadata (right after the bitmap)
    int metadata_block_index = BITMAP_SIZE / BLOCK_SIZE;
    if (BITMAP_SIZE % BLOCK_SIZE > 0) {
        metadata_block_index++; // Increment if bitmap doesn't align perfectly with block size
    }
    FileMetadata metadata;
    int found = 0;

    for (int i = 0; i < NUM_BLOCKS; ++i) { // Iterate through blocks to find metadata
        if (read_block(metadata_block_index + i, &metadata) == -1) {
            break; // Error or end of metadata region
        }
        if (strncmp(metadata.filename, filename, MAX_FILENAME_LENGTH) == 0) {
            found = 1; // File found
            break;
        }
    }

    if (!found) {
        return -1; // File not found
    }

    // Write data to file's blocks
    int start_block = offset / BLOCK_SIZE;
    int block_offset = offset % BLOCK_SIZE;
    int bytes_written = 0;
    char block[BLOCK_SIZE];

    while (bytes_written < num_bytes) {
        if (metadata.blockIndices[start_block] == -1) {
            // Allocate a new block if necessary
            int new_block = find_free_block();
            if (new_block == -1) {
                return -1; // No space left
            }
            metadata.blockIndices[start_block] = new_block;
            mark_block_used(new_block);
        }

        read_block(metadata.blockIndices[start_block], &block);
        int to_write = min(BLOCK_SIZE - block_offset, num_bytes - bytes_written);
        memcpy(block + block_offset, buffer + bytes_written, to_write);
        write_block(metadata.blockIndices[start_block], &block);
        
        bytes_written += to_write;
        start_block++;
        block_offset = 0; // Reset block offset for subsequent blocks
    }

    // Update file size and modification time
    metadata.size = max(metadata.size, offset + bytes_written);
    time(&metadata.modificationTime);
    write_block(metadata_block_index, &metadata); // Save updated metadata

    return bytes_written; // Return the number of bytes written
}

void list_files() {
    int metadata_block_index = BITMAP_SIZE / BLOCK_SIZE;
    if (BITMAP_SIZE % BLOCK_SIZE > 0) {
        metadata_block_index++; // Increment if bitmap doesn't align perfectly with block size
    }
    FileMetadata metadata;

    printf("List of Files:\n");
    for (int i = 0; i < NUM_BLOCKS; ++i) { // Iterate through blocks to read metadata
        if (read_block(metadata_block_index + i, &metadata) == -1) {
            break; // Error or end of metadata region
        }
        if (metadata.filename[0] != '\0') { // Check if filename is not empty
            printf("File: %s, Size: %d bytes, Created: %s, Modified: %s\n",
                   metadata.filename, metadata.size,
                   ctime(&metadata.creationTime), ctime(&metadata.modificationTime));
        }
    }
}
