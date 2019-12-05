#ifndef my_file_system_h
#define my_file_system_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include "disk.c"
#include "disk.h"

#define SIZE_OF_BLOCK 4096
#define FIRST_BLOCK 0
#define SIZE_OF_DISK 67108864
#define LAST_BLOCK 16383
#define MAXIMUM_NUMBER_OF_FILES 256
#define LENGTH_OF_DIRECTORY_NAME 15
#define LENGTH_OF_FILE_NAME 15
#define FIRST_FILE_DESCRIPTOR 0
#define LAST_FILE_DESCRIPTOR 63
#define FIRST_FILE_INDEX 0
#define LAST_FILE_INDEX 255
#define MAXIMUM_NUMBER_OF_FILE_DESCRIPTORS 64
#define FIRST_BYTE_OF_BLOCK 0
#define LAST_BYTE_OF_BLOCK 4096
#define EXTENSION_LENGTH 10
#define PATH_LEN 100
#define MAXIMUM_NUMBER_OF_DIRECTORIES 10
#define USED 0
#define UNUSED -1
#define LENGTH_OF_DISK_NAME 15
#define LENGTH_OF_NAME_FOR_THE_FILE_SYSTEM 15

int make_fs(char* disk_name);
int mount_fs(char* disk_name);
int unmount_fs(char* disk_name);
int fs_open(char* name);
int fs_close(int fildes);
int fs_create(char* name, char file_name, char file_extension, int file_size);
int fs_delete(char* name);
int fs_mkdir(char* name);
int fs_read(int fildes, void* buf, size_t nbyte);
int fs_write(int fildes, void* buf, size_t nbyte);
int fs_get_filesize(int fildes);
int fs_lseek(int fildes, off_t offset);
int fs_truncate(int fildes, off_t offset);
int find_file(char* name);
int find_free_block(int index_of_file);
int find_free_file_descriptor(int index_of_file);
int find_next_block(int current_block, int index_of_file);
int get_file_descriptor(int index_of_file);
int file_name_char_count(char* file_name);
int directory_name_char_count(char* directory_name);
int extension_for_file_name_char_count(char* extension_for_the_file_name);
int used_entries();
int unused_entries();
int name_for_file_system_char_count(char* name_for_file_system);

int status;
int status1;
int status2;
char blank;
char* first_block_buffer;
char* second_block_buffer;
char* block_buffer;
int i;
int index_of_file;
int index_of_block;
int num_blocks;
int block_index;
char* buffer_for_first_index;
char* buffer_for_next_index;
int file_descriptor;

typedef struct
{
    int num_directories_contained;
    int index_of_data;
    int directory_index;
    int root_directory;
    int first_block;
    char directories[MAXIMUM_NUMBER_OF_DIRECTORIES];
}DIR;

typedef struct
{
    char name;
    char ext;
    int attribute;
    int create_time;
    int create_date;
    int last_access;
    int modified_time;
    int modified_date;
    int start_block;
    int size;
    int end_block;
    unsigned int offset;
}ENTRY;

typedef struct
{
    int file_in_use;
    char file_name;
    int file_size;
    int first_block_used;
    int num_blocks_used;
    int num_file_descriptors;
    int first_block;
    int size;
    int num_blocks;
    int num_files_used;
    int file_descriptor_in_use;
    int file_index;
    int num_files_contained;
}FILE_ALLOCATION;

typedef struct
{
    char file_name;
    char file_extension;
    int file_size;
    int first_block;
    int last_block;
    char create_date;
    char create_time;
    char modified_date;
    char modified_time;
    char last_access;
    int offset;
    int block;
    int used_entries[MAXIMUM_NUMBER_OF_FILES];
    int unused_entries[MAXIMUM_NUMBER_OF_FILES];
    int status;
    int entries[MAXIMUM_NUMBER_OF_FILES];
    int blocks[LAST_BLOCK];
}FAT;

typedef struct
{
    int total_blocks;
    int used_blocks;
    int free_blocks;
    int* file_storing_location;
    char file_name;
    char directory_name;
    int FAT[MAXIMUM_NUMBER_OF_FILES];
    ENTRY meta_data;
    DIR root_directory_ptr;
    FILE_ALLOCATION file_allocation_ptr;
    ENTRY entry;
    FAT fat[MAXIMUM_NUMBER_OF_FILES];
}FAT_TABLE;

typedef struct
{
    char name;
    char img;
    int state;
    unsigned int used_size;
    struct VolumeHeader *vbr;
    FAT_TABLE fat_table;
    int index_of_data;
    DIR root_directory_ptr;
}VOLUME;

typedef struct
{
    int bps;
    int spc;
    int num_fats;
    int spf;
    int root_entries;
    int total_entries;
}VOLUME_HEADER;

typedef struct
{
    int file_descriptor_in_use;
    char file_name;
    int offset_for_seeking;
    int allocated;
    
}FILE_DESCRIPTOR;

VOLUME *disk_vol;

DIR *directory;

FAT_TABLE *fat_table;

FAT *fat;

FILE_ALLOCATION *allocation;

FILE_DESCRIPTOR *descriptors;

VOLUME *p_map;

FILE_DESCRIPTOR *descriptor_ptr;

FILE_ALLOCATION *allocated;

FILE_ALLOCATION *files;

#endif
