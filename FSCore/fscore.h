/********************************************************************************
Filename:     fscore.h
Author:       Daniel Fewell
Last Edited:  12/11/2023
Description:  Header File for the functions contained in fscore.c
********************************************************************************/
#ifndef FSCORE_H
#define FSCORE_H

#include "../DiskLib/disklib.h"
#include <time.h>

#define MAX_FILENAME_LENGTH 255
#define MAX_BLOCKS_PER_FILE 1280 

typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    int size;
    int blockIndices[MAX_BLOCKS_PER_FILE];
    time_t creationTime;
    time_t modificationTime;
} FileMetadata;

int create_file(const char *filename);
int delete_file(const char *filename);
int read_file(const char *filename, void *buffer, int num_bytes, int offset);
int write_file(const char *filename, const void *buffer, int num_bytes, int offset);
void list_files();
int save_metadata(const FileMetadata *metadata); // Added function prototype

#endif 

