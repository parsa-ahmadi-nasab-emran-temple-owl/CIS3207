//Parsa Ahmadi Nasab Emran
//TU ID: 915408114
//ACCESSNET USERNAME: tug81042
//Project 4: Implementing A Simple File System On Top Of A Virtual Disk

//header
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include "disk.c"
#include "disk.h"

//defined constants
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

//defined functions
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
int status_make_fs(char* name_for_file_system);
int status_mount_fs(char* name_for_disk);
int status_fs_create(char* file_name, char* file_name1, int extension_for_the_file_name, int size_of_the_file);
int status_fs_delete(char* file_name);
int status_get_filesize(int fd);
int status_fs_mkdir(char* directory_name);
int status_fs_open(char* file_name);
int status_fs_truncate(int fd, off_t offset);
int status_unmount_fs(char* name_for_disk);
int status_fs_close(int fd);
int status_fs_write(int fd, void* buf, size_t nbyte);
int status_fs_read(int fd, void* buf, size_t nbyte);
int status_fs_lseek(int fd, off_t offset);

//defined variables
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
int fd;
char file_name[LENGTH_OF_FILE_NAME + 1];
char extension_for_the_file_name[EXTENSION_LENGTH + 1];
int size_of_the_file;
char directory_name[LENGTH_OF_DIRECTORY_NAME + 1];
char name_for_disk[LENGTH_OF_DISK_NAME + 1];
char name_for_file_system[LENGTH_OF_NAME_FOR_THE_FILE_SYSTEM + 1];

//definition of typedef structs
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

//declaration of pointers to typedef structs
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

//main
//also test for my FAT File System
int main(int argc, char *argv[]) {
    file_name[LENGTH_OF_FILE_NAME + 1];
    extension_for_the_file_name[EXTENSION_LENGTH + 1];
    size_of_the_file;
    directory_name[LENGTH_OF_DIRECTORY_NAME + 1];
    name_for_disk[LENGTH_OF_DISK_NAME + 1];
    name_for_file_system[LENGTH_OF_NAME_FOR_THE_FILE_SYSTEM + 1];
    int status1;
    int status2;
    int status3;
    int status4;
    int status5;
    printf("Welcome to my FAT file system!\n");
    printf("Please enter a file name: ");
    gets(file_name);
    status1 = file_name_char_count(file_name);
    while(status1 == -1){
        printf("The length of the file name, %s, is longer than 15 characters or less than 1 character.\n", file_name);
        printf("Please enter a file name that is between 1 and 15 characters inclusive: ");
        gets(file_name);
        status1 = file_name_char_count(file_name);
    }
    if(status1 != -1){
        printf("You have entered a file name, %s, that is between 1 and 15 characters long.\n", file_name);
    }
    status2 = extension_for_file_name_char_count(extension_for_the_file_name);
    while(status2 == -1){
        printf("The length of the extension for the file name, %s, is longer than 10 characters or less than 1 character.\n", extension_for_the_file_name);
        printf("Please enter an extension for the file name that is between 1 and 10 characters long: ");
        gets(extension_for_the_file_name);
        status2 = extension_for_file_name_char_count(extension_for_the_file_name);
    }
    if(status2 != -1){
        printf("You have entered extension for the file name, %s, that is between 1 and 10 characters long.\n", extension_for_the_file_name);
    }
    printf("Please enter a directory name: ");
    gets(directory_name);
    status3 = directory_name_char_count(directory_name);
    while(status3 == -1){
        printf("The length of the file name, %s, is longer than 15 characters or less than 1 character.\n", directory_name);
        printf("Please enter a directory name that is between 1 and 15 characters long: ");
        gets(directory_name);
        status3 = directory_name_char_count(directory_name);
    }
    if(status3 != -1){
        printf("You have entered a directory name, %s, that is between 1 and 15 characters long.\n", directory_name);
    }
    printf("Please enter a name for the disk: ");
    gets(name_for_disk);
    status4 = disk_name_char_count(name_for_disk);
    while(status4 == -1){
        printf("The length of the disk name, %s, is longer than 15 characters or less than 1 character.\n", name_for_disk);
        printf("Please enter a disk name that is between 1 and 15 characters long: ");
        gets(name_for_disk);
        status4 = disk_name_char_count(name_for_disk);
    }
    if(status4 != -1){
        printf("You have entered a disk name, %s, that is between 1 and 15 characters long.\n", name_for_disk);
    }
    printf("Please enter a size for the file: ");
    scanf("%d", &size_of_the_file);
    printf("Please enter a name for the file system: ");
    gets(name_for_file_system);
    status5 = name_for_file_system_char_count(name_for_file_system);
    while(status5 == -1){
        printf("The length of the disk name, %s, is longer than 15 characters or less than 1 character.\n", name_for_file_system);
        printf("Please enter a name for the file system that is between 1 and 15 characters long: ");
        gets(name_for_file_system);
    }
    if(status5 != -1){
        printf("You have entered a name for the file system, %s, that is between 1 and 15 characters long.\n", name_for_file_system);
    }
    status_make_fs(name_for_file_system);
    status_mount_fs(name_for_disk);
    int user_input = 0;
    printf("1. fs_create\n");
    printf("2. fs_open\n");
    printf("3. fs_get_filesize\n");
    printf("4. fs_truncate\n");
    printf("5. fs_close\n");
    printf("6. fs_mkdir\n");
    printf("7. fs_lseek\n");
    printf("8. fs_read\n");
    printf("9: fs_write\n");
    printf("10: fs_delete\n");
    printf("Please enter 1: ");
    scanf("%d", user_input);
    status_fs_create(file_name, file_name, extension_for_the_file_name, size_of_the_file);
    printf("Please enter 2: ");
    scanf("%d", user_input);
    status_fs_open(file_name);
    printf("Please enter 3: ");
    scanf("%d", user_input);
    status_fs_get_filesize((fs_open(file_name)));
    printf("Please enter 4: ");
    scanf("%d", user_input);
    off_t offset = 15;
    status_fs_truncate((fs_open(file_name)), offset);
    printf("Please enter 5: ");
    status_fs_close((fs_open(file_name)));
    printf("Please enter 6: ");
    scanf("%d", user_input);
    status_fs_mkdir(directory_name);
    printf("PLease enter 7: ");
    scanf("%d", user_input);
    fd = fs_open(file_name);
    status_fs_lseek(fd, offset);
    printf("Please enter 8: ");
    scanf("%d", user_input);
    char buffer1[BLOCK_SIZE] = "";
    size_t nbyte = 100;
    fd = fs_open(file_name);
    status_fs_read(fd, buffer1, nbyte);
    printf("Please enter 9: ");
    scanf("%d", user_input);
    fd = fs_open(file_name);
    status_fs_write(fd, buffer1, nbyte);
    printf("Please enter 10: ");
    scanf("%d", user_input);
    status_fs_delete(file_name);
    status_unmount_fs(name_for_disk);
    return 0;
}

//status_fs_lseek
int status_fs_lseek(int fd, off_t offset){
    fd = fs_open(file_name);
    int status_fs_lseek = fs_lseek(fd, BLOCK_SIZE);
    if(status_fs_lseek == 0){
        printf("The function, fs_lseek, was called successfully.\n");
        return 0;
    }
    if(status_fs_lseek == -1){
        printf("The function, fs_lseek, was not called successfully.\n");
        return -1;
    }
}

//status_fs_read
int status_fs_read(int fd, void* buf, size_t nbyte){
    int p = 0;
    char buffer1[BLOCK_SIZE];
    char value1[BLOCK_SIZE];
    for(i = 0; i < BLOCK_SIZE; i++){
        value1[i + BLOCK_SIZE] = "hello student";
    }
    fd = fs_open(file_name);
    if((fs_read(fd, buffer1, BLOCK_SIZE)) >= 0){
        p = fs_read(fd, buffer1, BLOCK_SIZE);
    }
    return p;
}

//status_fs_write
int status_fs_write(int fd, void* buf, size_t nbyte){
    int i = 0;
    int j = 0;
    int k = 0;
    int status_fs_write = 0;
    fd = fs_open(file_name);
    char string1[BLOCK_SIZE];
    for(i = 0; i < BLOCK_SIZE; i++){
        string1[i + BLOCK_SIZE] = "hello world";
    }
    char string2[BLOCK_SIZE];
    for(i = 0; i < BLOCK_SIZE; i++){
        string2[i + BLOCK_SIZE] = "welcome to the world";
    }
    char string3[BLOCK_SIZE];
    for(i = 0; i < BLOCK_SIZE; i++){
        string1[i + BLOCK_SIZE] = "welcome to planet earth";
    }
    int status_fs_write_1 = fs_write(fd, string1, BLOCK_SIZE);
    int status_fs_write_2 = fs_write(fd, string2, BLOCK_SIZE);
    int status_fs_write_3 = fs_write(fd, string3, BLOCK_SIZE);
    if(status_fs_write_1 >= 0){
        i = status_fs_write_1;
    }
    if(status_fs_write_1 == -1){
        i = -1;
    }
    if(status_fs_write_2 >= 0){
        j = status_fs_write_2;
    }
    if(status_fs_write_2 == -1){
        j = -1;
    }
    if(status_fs_write_3 >= 0){
        k = status_fs_write_3;
    }
    if(status_fs_write_3 == -1){
        k = -1;
    }
    return i, j, k;
}

//status_make_fs
int status_make_fs(char* name_for_file_system){
    int status_make_fs = 0;
    status_make_fs = make_fs(name_for_file_system);
    if(status_make_fs == 0){
        printf("The file system with the name, %s, was created.\n", name_for_file_system);
        return 0;
    }
    if(status_make_fs == -1){
        printf("The file system with the name, %s, was not created.\n", name_for_file_system);
        return -1;
    }
}

//status_mount_fs
int status_mount_fs(char* name_for_disk){
    int status_mount_fs = mount_fs(name_for_disk);
    if(status_mount_fs == 0){
        printf("The disk with the name, %s, was mounted.\n", name_for_disk);
        return 0;
    }
    if(status_mount_fs == -1){
        printf("The disk with the name, %s, was not mounted.\n", name_for_disk);
        return -1;
    }
}

//status_fs_create
int status_fs_create(char* file_name, char* file_name1, int extension_for_file_name, int size_of_the_file){
    int status_fs_create = 0;
    status_fs_create = fs_create(file_name, file_name1, extension_for_file_name, size_of_the_file);
    if(status_fs_create == 0){
        printf("The file with the name, %s, was created.\n", file_name);
        return 0;
    }
    if(status_fs_create == -1){
        printf("The file with the name, %s, was not created.\n", file_name);
        return -1;
    }
}

//status_fs_delete
int status_fs_delete(char* file_name){
    int status_fs_delete = 0;
    status_fs_delete = fs_delete(file_name);
    if(status_fs_delete == 0){
        printf("The file with the name, %s, was deleted.\n", file_name);
        return 0;
    }
    if(status_fs_delete == -1){
        printf("The file with the name, %s, was not deleted.\n", file_name);
        return -1;
    }
}

//status_get_filesize
int status_fs_get_filesize(int fd){
    int status_fs_get_filesize = 0;
    fd = fs_open(file_name);
    if(fd >= 0 && fd <= 63){
        status_fs_get_filesize = fs_get_filesize(fd);
        if(status_fs_get_filesize >= 0){
            printf("The size of the file with the name, %s, is %d bytes.\n", file_name, size_of_the_file);
            return 0;
        }
        if(status_fs_get_filesize < 0){
            printf("The size of the file with the name, %s, is not valid.\n", file_name);
            return -1;
        }
    }
}

//status_fs_mkdir
int status_fs_mkdir(char* directory_name){
    int status_fs_mkdir = 0;
    status_fs_mkdir = fs_mkdir(directory_name);
    if(status_fs_mkdir == 0){
        printf("The directory with the name, %s, was created.\n", directory_name);
        return 0;
    }
    if(status_fs_mkdir == -1){
        printf("The directory with the name, %s, was not created.\n", directory_name);
        return -1;
    }
}

//status_fs_open
int status_fs_open(char* file_name){
    int status_fs_open = 0;
    status_fs_open = fs_open(file_name);
    fd = status_fs_open;
    int i = 0;
    int j = 0;
    if(fd >= 0 && fd <= 63){
        printf("The file descriptor, %d, is valid.\n", fd);
        i = 0;
    }
    if(fd < 0){
        printf("The file descriptor, %d, is not valid.\n", fd);
        i = -1;
    }
    
    if(status_fs_open >= 0){
        printf("The file name with the name, %s, was opened.\n", file_name);
        j = 0;
    }
    if(status_fs_open == -1){
        printf("The file name with the name, %s, was not opened.\n", file_name);
        j = -1;
    }
    return i, j;
}

//status_fs_truncate
int status_fs_truncate(int fd, off_t offset){
    int status_fs_truncate = 0;
    fd = fs_open(file_name);
    offset = 15;
    status_fs_truncate = fs_truncate(fd, offset);
    if(fd >= 0 && fd <= 63){
        if(status_fs_truncate == 0){
            printf("The file with the name, %s, is truncated by %d bytes.\n", file_name, offset);
            return 0;
        }
        if(status_fs_truncate == -1){
            printf("The file with the name, %s, is not truncated by %d bytes since %d is not valid.\n", file_name, offset);
            return -1;
        }
    }
}

//status_unmount_fs
int status_unmount_fs(char* name_for_disk){
    int status_unmount_fs = 0;
    status_unmount_fs = mount_fs(name_for_disk);
    if(status_unmount_fs == 0){
        printf("The disk with the name, %s, was unmounted.\n", name_for_disk);
        return 0;
    }
    if(status_unmount_fs == -1){
        printf("The disk with the name, %s, was not unmounted.\n", name_for_disk);
        return -1;
    }
}

//status_fs_close
int status_fs_close(int fd){
    int status_fs_close = 0;
    fd = fs_open(file_name);
    status_fs_close = fs_close(fd);
    if(status_fs_close == 0){
        printf("The file with the name, %s, is closed.\n", file_name);
        return 0;
    }
    if(status_fs_close == -1){
        printf("The file with the name, %s, is not closed.\n", file_name);
        return -1;
    }
}

//extension_for_file_mame_char_count
int extension_for_file_name_char_count(char* extension_for_the_file_name){
    int extension_for_file_name_char_count = strlen(extension_for_the_file_name);
    if(extension_for_file_name_char_count == 1 || extension_for_file_name_char_count <= EXTENSION_LENGTH){
        return extension_for_file_name_char_count;
    }
    if(extension_for_file_name_char_count < 1){
        return -1;
    }
    if(extension_for_file_name_char_count > EXTENSION_LENGTH){
        return -1;
    }
}

//file_name_char_count
int file_name_char_count(char* file_name){
    int file_name_char_count = strlen(file_name);
    if(file_name_char_count == 1 || file_name_char_count <= LENGTH_OF_FILE_NAME){
        return file_name_char_count;
    }
    if(file_name_char_count < 1){
        return -1;
    }
    if(file_name_char_count > LENGTH_OF_FILE_NAME){
        return -1;
    }
}

//find_free_block
int find_free_block(int index_of_file){
    if (index_of_file >= FIRST_FILE_INDEX && index_of_file <= LAST_FILE_INDEX){
        first_block_buffer[1] = 0;
        second_block_buffer[1] = 0;
        block_read(disk_vol->index_of_data, first_block_buffer);
        block_read(disk_vol->index_of_data + 1, second_block_buffer);
        for(i = FIRST_BLOCK; i < BLOCK_SIZE; i++){
            if(first_block_buffer[i] == '\0'){
                first_block_buffer[i] = (int)(index_of_file + 1);
                block_write(disk_vol->index_of_data, first_block_buffer);
                return i;
            }
        }
        for(i = FIRST_BLOCK; i < BLOCK_SIZE; i++){
            if(second_block_buffer[i] == '\0'){
                second_block_buffer[i] = (int)(index_of_file + 1);
                block_write(disk_vol->index_of_data, second_block_buffer);
                return i;
            }
            if(second_block_buffer[i] != '\0'){
                printf("error: no available blocks.\n");
                return -1;
            }
        }
    }
    if(index_of_file < FIRST_FILE_INDEX){
        return -1;
    }
    if(index_of_file > LAST_FILE_INDEX){
        return -1;
    }
}

//directory_name_char_count
int directory_name_char_count(char* directory_name){
    int directory_name_char_count = strlen(directory_name);
    if(directory_name_char_count == 1 || directory_name_char_count <= LENGTH_OF_DIRECTORY_NAME){
        return directory_name_char_count;
    }
    if(directory_name_char_count < 1){
        return -1;
    }
    if(directory_name_char_count > LENGTH_OF_DIRECTORY_NAME){
        return -1;
    }
}

//disk_name_char_count
int disk_name_char_count(char* name_for_disk){
    int disk_name_char_count = strlen(name_for_disk);
    if(disk_name_char_count == 1 || disk_name_char_count <= LENGTH_OF_DISK_NAME){
        return disk_name_char_count;
    }
    if(disk_name_char_count < 1){
        return -1;
    }
    if(disk_name_char_count <= LENGTH_OF_DISK_NAME){
        return -1;
    }
}

//find_free_file_descriptor
int find_free_file_descriptor(int index_of_file){
    if (index_of_file >= FIRST_FILE_INDEX && index_of_file <= LAST_FILE_INDEX){
        for(i = FIRST_FILE_DESCRIPTOR; i <= LAST_FILE_DESCRIPTOR; i++){
            if(descriptors[i].file_descriptor_in_use == -1){
                descriptors[i].file_descriptor_in_use = 0;
                descriptors[i].allocated = index_of_file;
                descriptors[i].offset_for_seeking = 0;
                return i;
            }
        }
        printf("error: no available file descriptor.\n");
        return -1;
    }
    if(index_of_file < FIRST_FILE_INDEX){
        return -1;
    }
    if(index_of_file > LAST_FILE_INDEX){
        return -1;
    }
}

//find_next_block
int find_next_block(int current_block, int index_of_file){
    if (current_block >= FIRST_BLOCK && current_block <= LAST_BLOCK && index_of_file >= FIRST_FILE_INDEX && index_of_file >= LAST_FILE_INDEX){
        block_buffer[1] = 0;
        if (current_block < BLOCK_SIZE){
            block_read(disk_vol->index_of_data, block_buffer);
            for(i = current_block + 1; i < BLOCK_SIZE; i++){
                if (block_buffer[i] == (index_of_file + 1)){
                    return i;
                }
            }
        }
        if(current_block > BLOCK_SIZE){
            block_read(disk_vol->index_of_data + 1, block_buffer);
            for(i = current_block - BLOCK_SIZE + 1; i < BLOCK_SIZE; i++){
                if (block_buffer[i] == (index_of_file + 1)){
                    return i + BLOCK_SIZE;
                }
            }
        }
    }
    if(current_block < FIRST_BLOCK){
        return -1;
    }
    if(current_block > LAST_BLOCK){
        return -1;
    }
    if(index_of_file < FIRST_FILE_INDEX){
        return -1;
    }
    if(index_of_file > LAST_FILE_INDEX){
        return -1;
    }
}

//fs_close
int fs_close(int fildes){
    if(fildes >= FIRST_FILE_DESCRIPTOR && fildes <= LAST_FILE_DESCRIPTOR){
        if(fildes < 0){
            return -1;
        }
        if (fildes >= LAST_FILE_DESCRIPTOR){
            return -1;
        }
        if(descriptors[fildes].file_descriptor_in_use == 0){
            return -1;
        }
        allocation[descriptor_ptr->file_name].num_file_descriptors = allocation[descriptor_ptr->file_name].num_file_descriptors - 1;
        descriptor_ptr->file_descriptor_in_use = -1;
        printf("file at given file descriptor is closed\n");
        printf("file at given file descriptor is closed\n");
        return 0;
    }
    if(fildes < FIRST_FILE_DESCRIPTOR){
        return -1;
    }
    if(fildes > LAST_FILE_DESCRIPTOR){
        return -1;
    }
}

//fs_create
int fs_create(char* name, char file_name, char file_extension, int file_size){
    if(name != NULL && file_name != (char) NULL && file_extension != (char) NULL && file_size >= FIRST_BLOCK && file_size <= SIZE_OF_BLOCK){
        index_of_file = -1;
        buffer_for_first_index = (char*)calloc(BLOCK_SIZE, sizeof(SIZE_OF_DISK));
        buffer_for_next_index = (char*)calloc(BLOCK_SIZE, sizeof(SIZE_OF_DISK));
        for(i = FIRST_FILE_INDEX; i <= LAST_FILE_INDEX; i++){
            if(allocation[i].file_in_use == 1){
                if (strcmp(&allocation[i].file_name, name) == 0){
                    break;
                }
            }
        }
        if (i != LAST_FILE_INDEX && i != 0){
            index_of_file = i;
            for(i = FIRST_FILE_INDEX; i <= LAST_FILE_INDEX; i++){
                if(fat_table->fat[i].status == fat->unused_entries[i]){
                    fat_table->fat[i].status = USED;
                    fat_table->fat[i].file_name = (char) file_name;
                    fat_table->fat[i].file_extension = (char) file_extension;
                    fat_table->fat[i].file_size = file_size;
                    fat_table->fat[i].first_block = find_free_block(index_of_file);
                    fat_table->fat[i].last_block = find_next_block(fat_table->fat[i].first_block, index_of_file);
                    time_t t = time(NULL);
                    struct tm tm = *localtime(&t);
                    fat_table->fat[i].create_date = (char) ("%d/%d/%d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);
                    fat_table->fat[i].create_time = (char) ("%d:%d:%d", tm.tm_hour, tm.tm_min, tm.tm_sec);
                    used_entries();
                    unused_entries();
                    return 0;
                }
            }
        }
        if(index_of_file == -1){
            for(i = FIRST_FILE_INDEX; i <= LAST_FILE_INDEX; i++){
                if(!allocation[i].file_in_use){
                    disk_vol->root_directory_ptr.num_directories_contained = disk_vol->root_directory_ptr.num_directories_contained + 1;
                    allocation[i].file_in_use = 0;
                    strcpy(&allocation[i].file_name, name);
                    allocation[i].file_size = 0;
                    allocation[i].first_block_used = 0;
                    allocation[i].num_blocks_used = 0;
                    allocation[i].num_file_descriptors = 0;
                    printf("FILE IS %s CREATED\n", name);
                    return 0;
                }
                
                if(allocation[i].file_in_use){
                    printf("error: file [%s] already exists\n", name);
                    return 0;
                }
            }
        }
    }
    if(name == NULL){
        return -1;
    }
    if(file_name == (char) NULL){
        return -1;
    }
    if(file_extension == (char) NULL){
        return -1;
    }
    if(file_size >= FIRST_BLOCK){
        return -1;
    }
    if(file_size <= SIZE_OF_BLOCK){
        return -1;
    }
}

//fs_delete
int fs_delete(char* name){
    if(name != NULL){
        for(i = FIRST_FILE_INDEX; i <= LAST_FILE_INDEX; i++){
            if(strcmp(&allocation[i].file_name, name) == 0){
                index_of_file = i;
                allocated = &allocation[i];
                index_of_block = allocated->first_block;
                num_blocks = allocated->num_blocks_used;
            }
            if(allocation[i].num_file_descriptors != 0){
                return -1;
            }
        }
        disk_vol->root_directory_ptr.num_directories_contained = disk_vol->root_directory_ptr.num_directories_contained - 1;
        allocated->file_in_use = 0;
        strcpy(&allocated->file_name, "");
        allocated->file_size = 0;
        allocated->num_file_descriptors = 0;
        block_read(disk_vol->root_directory_ptr.index_of_data, buffer_for_first_index);
        block_read(disk_vol->root_directory_ptr.index_of_data + 1, buffer_for_next_index);
        while (num_blocks > 0){
            if (index_of_block < BLOCK_SIZE){
                buffer_for_first_index[index_of_block] = '\0';
            }
            if(index_of_block > BLOCK_SIZE){
                buffer_for_next_index[index_of_block - BLOCK_SIZE] = '\0';
            }
            index_of_block = find_next_block(allocated->first_block_used, index_of_file);
            num_blocks = num_blocks - 1;
        }
        for(i = FIRST_FILE_INDEX; i <= LAST_FILE_INDEX; i++){
            if(fat_table->fat[i].file_name == (char) name){
                fat_table->fat[i].status = UNUSED;
                fat_table->fat[i].file_name = (char) "";
                fat_table->fat[i].file_extension = (char) "";
                fat_table->fat[i].file_size = 0;
                fat_table->fat[i].first_block = 0;
                fat_table->fat[i].last_block = 0;
                fat_table->fat[i].create_date = (char) "";
                fat_table->fat[i].create_time = (char) "";
                used_entries();
                unused_entries();
                return 0;
            }
        }
        allocation[i].first_block_used = -1;
        allocation[i].num_blocks_used = 0;
        block_write(disk_vol->root_directory_ptr.index_of_data, buffer_for_first_index);
        block_write(disk_vol->root_directory_ptr.index_of_data + 1, buffer_for_next_index);
        printf("FILE %s DELETED\n", name);
        return 0;
    }
    if(name == NULL){
        printf("error: FILE %s IS NOT DELETED\n", name);
        return -1;
    }
}

//fs_get_filesize
int fs_get_filesize(int fildes){
    if(fildes >= FIRST_FILE_DESCRIPTOR && fildes <= LAST_FILE_DESCRIPTOR){
        if(!descriptors[fildes].file_descriptor_in_use){
            printf("error: Invalid file descriptor.\n");
            return -1;
        }
        if(descriptors[fildes].file_descriptor_in_use){
            return allocation[descriptors[fildes].file_descriptor_in_use].file_size;
        }
    }
    if(fildes < FIRST_FILE_DESCRIPTOR){
        return -1;
    }
    if(fildes > LAST_FILE_DESCRIPTOR){
        return -1;
    }
}

//fs_lseek
int fs_lseek(int fildes, off_t offset){
    if(fildes >= FIRST_FILE_DESCRIPTOR && fildes <= LAST_FILE_DESCRIPTOR && offset >= 0){
        if (offset > allocation[descriptors[fildes].file_descriptor_in_use].file_size || offset < 0){
            printf("error: Can't set the file pointer beyond the file range.\n");
            return -1;
        }
        if(!allocation[fildes].file_size){
            printf("error: Invalid file descriptor.\n");
            return -1;
        }
        if(offset < allocation[descriptors[fildes].file_descriptor_in_use].file_size || offset > 0){
            descriptors[fildes].offset_for_seeking = (int) offset;
            printf("This function was called successfully.\n");
            return 0;
        }
        if(fildes < FIRST_FILE_DESCRIPTOR){
            return -1;
        }
    }
    if(fildes > LAST_FILE_DESCRIPTOR){
        return -1;
    }
    if(offset < 0){
        return -1;
    }
}

//fs_mkdir
int fs_mkdir(char* name){
    if(name != NULL){
        blank = (char) "";
        for(i = 0; i < MAXIMUM_NUMBER_OF_DIRECTORIES; i++){
            if(directory->directories[i] == (char) name){
                printf("directory %s with the name already exists\n", name);
                return 0;
            }
            if(directory->directories[i] == blank){
                directory->directories[i] = (char) name;
                printf("directory %s with the name is created\n", name);
                return 0;
            }
        }
    }
    if(name == NULL){
        return -1;
    }
}

//fs_open
int fs_open(char* name){
    if(name != NULL){
        index_of_file = -1;
        for(i = FIRST_FILE_INDEX; i <= LAST_FILE_INDEX; i++){
            if(allocation[i].file_in_use == 1){
                if (strcmp(&allocation[i].file_name, name) == 0){
                    break;
                }
            }
        }
        if (i != LAST_FILE_INDEX){
            index_of_file = i;
        }
        
        if(index_of_file == -1){
            printf(" file %s does not exist\n", name);
            return -1;
        }
        
        file_descriptor = get_file_descriptor(index_of_file);
        if (file_descriptor == -1){
            return file_descriptor;
        }
        
        allocation[index_of_file].num_file_descriptors++;
        printf("FILE IS %s OPENED\n", name);
        return file_descriptor;
    }
    if(name == NULL){
        return -1;
    }
}

//fs_read
int fs_read(int fildes, void* buf, size_t nbyte){
    if(fildes >= FIRST_FILE_DESCRIPTOR && fildes <= LAST_FILE_DESCRIPTOR && buf != NULL && nbyte >= FIRST_BYTE_OF_BLOCK && nbyte <= LAST_BYTE_OF_BLOCK){
        if (!descriptors[fildes].file_descriptor_in_use){
            return -1;
        }
        char * data_buffer = buf;
        char * block_buffer = (char*)calloc(BLOCK_SIZE, sizeof(char));
        index_of_file = descriptors[fildes].file_name;
        allocated = &allocation[index_of_file];
        index_of_block = files->first_block_used;
        int num_blocks = 0;
        int selected_descriptor_offset = descriptors[fildes].offset_for_seeking;
        
        while (selected_descriptor_offset >= BLOCK_SIZE){
            index_of_block = find_free_block(index_of_file);
            num_blocks++;
            selected_descriptor_offset  = selected_descriptor_offset - BLOCK_SIZE;
        }
        block_read(index_of_block, block_buffer);
        int num_blocks_read = 0;
        for(i = selected_descriptor_offset; i < BLOCK_SIZE; i++){
            data_buffer[num_blocks_read] = block_buffer[i];
            num_blocks_read = num_blocks_read + 1;
            if (num_blocks_read == (int) nbyte){
                descriptors[fildes].offset_for_seeking = descriptors[fildes].offset_for_seeking + num_blocks_read;
                return num_blocks_read;
            }
        }
        num_blocks_read = num_blocks_read + 1;
        strcpy(block_buffer, "");
        while(num_blocks_read < (int)nbyte && num_blocks <= allocated->num_blocks_used){
            block_index = find_next_block(index_of_block, index_of_file);
            strcpy(block_buffer,"");
            block_read(index_of_block, block_buffer);
            for(int i= FIRST_BLOCK; i < BLOCK_SIZE; i++){
                data_buffer[num_blocks_read] = block_buffer[i];
                num_blocks_read = num_blocks_read + 1;
                if(num_blocks_read == (int)nbyte ){
                    descriptors[fildes].offset_for_seeking += num_blocks_read;
                    return num_blocks_read;
                }
            }
            num_blocks_read++;
            descriptors[fildes].offset_for_seeking += num_blocks_read;
            return num_blocks_read;
        }
    }
    if(fildes < FIRST_FILE_DESCRIPTOR){
        return -1;
    }
    if(fildes > LAST_FILE_DESCRIPTOR){
        return -1;
    }
    if(buf == NULL){
        return -1;
    }
    if(nbyte < FIRST_BYTE_OF_BLOCK){
        return -1;
    }
    if(nbyte > LAST_BYTE_OF_BLOCK){
        return -1;
    }
}

//fs_truncate
int fs_truncate(int fildes, off_t offset){
    if(fildes >= 0 && fildes <= 63 && offset > 0){
        char index_of_file = descriptors[fildes].allocated;
        FILE_ALLOCATION* allocated  = &allocation[index_of_file];
        if(!descriptors[fildes].file_descriptor_in_use){
            printf("error: Invalid file descriptor.\n");
            return -1;
        }
        if (offset > allocated->size || offset < 0) {
            printf("error: Can't set the offset beyond the file range.\n");
            return -1;
        }
        int new_block_num = (int) (offset + BLOCK_SIZE - 1) / BLOCK_SIZE;
        int i;
        int index_of_block = allocated->first_block_used;
        for (i = 0; i < new_block_num; ++i) {
            index_of_block = find_next_block(index_of_block, index_of_file);
        }
        while (index_of_block > 0){
            char buf[BLOCK_SIZE] = "";
            if (index_of_block < BLOCK_SIZE){
                block_read(disk_vol->index_of_data, buf);
                buf[index_of_block] = '\0';
                block_write(disk_vol->index_of_data, buf);
            }
            else {
                block_read(disk_vol->index_of_data + 1, buf);
                buf[index_of_block - BLOCK_SIZE] = '\0';
                block_write(disk_vol->index_of_data + 1, buf);
            }
            index_of_block = find_next_block(index_of_block, index_of_file);
        }
        allocated->size = (int) offset;
        allocated->num_blocks = new_block_num;
        
        for(i = 0; i < 64; i++) {
            if(descriptors[i].file_descriptor_in_use == USED && descriptors[i].allocated == index_of_file) {
                descriptors[i].offset_for_seeking = (int) offset;
            }
        }
        printf("This function was called successfully.\n");
        return 0;
    }
    else{
        return -1;
    }
}

//fs_write
int fs_write(int fildes, void* buf, size_t nbyte){
    if(fildes >= FIRST_FILE_DESCRIPTOR && fildes <= LAST_FILE_DESCRIPTOR && buf != NULL && nbyte >= FIRST_BYTE_OF_BLOCK && nbyte <= LAST_BYTE_OF_BLOCK){
        if(!descriptors[fildes].file_descriptor_in_use){
            return -1;
        }
        char *src = buf;
        char block[BLOCK_SIZE] = "";
        index_of_file = descriptors[fildes].file_name;
        allocated = &allocation[index_of_file];
        index_of_block = allocated->first_block_used;
        int size = allocated->file_size;
        int num_blocks = 0;
        int selected_descriptor_offset = descriptors[fildes].offset_for_seeking;
        while (selected_descriptor_offset >= BLOCK_SIZE){
            index_of_block = find_next_block(index_of_block, index_of_file);
            num_blocks++;
            selected_descriptor_offset -= BLOCK_SIZE;
        }
        int write_count = 0;
        if (index_of_block != -1){
            block_read(block_index, block);
            for(i = selected_descriptor_offset; i < BLOCK_SIZE; i++){
                block[i] = src[write_count++];
                if (write_count == (int) nbyte || write_count == strlen(src)){
                    block_write(index_of_block, block);
                    descriptors[fildes].offset_for_seeking += write_count;
                    if(size < descriptors[fildes].offset_for_seeking){
                        allocated->size = descriptors[fildes].offset_for_seeking;
                    }
                    return write_count;
                }
            }
            block_write(index_of_block, block);
            num_blocks++;
        }
        strcpy(block, "");
        while(write_count < (int) nbyte && write_count < strlen(src) && num_blocks < allocated->num_blocks){
            index_of_block = find_next_block(index_of_block, index_of_file);
            for(i = 0; i < BLOCK_SIZE; i++){
                block[i] = src[write_count++];
                if(write_count == (int) nbyte || write_count == strlen(src)){
                    block_write(index_of_block, block);
                    descriptors[fildes].offset_for_seeking += write_count;
                    if(size < descriptors[fildes].offset_for_seeking){
                        allocated->size = descriptors[fildes].offset_for_seeking;
                    }
                    return write_count;
                }
                block_write(index_of_block, block);
                num_blocks++;
            }
            strcpy(block, "");
        }
        while(write_count < (int) nbyte && write_count < strlen(src)){
            index_of_block = find_free_block(index_of_file);
            allocated->num_blocks++;
            if (allocated->first_block_used == -1){
                allocated->first_block_used = index_of_block;
            }
            if (index_of_block < 0){
                fprintf(stderr, "error: No free blocks.\n");
                return -1;
            }
            for(i = 0; i < BLOCK_SIZE; i++){
                block[i] = src[write_count++];
                if(write_count == (int) nbyte || write_count == strlen(src)){
                    block_write(block_index, block);
                    descriptors[fildes].offset_for_seeking += write_count;
                    if(size < descriptors[fildes].offset_for_seeking){
                        allocated->size = descriptors[fildes].offset_for_seeking;
                    }
                    return write_count;
                }
                block_write(index_of_block, block);
            }
            
            descriptors[fildes].offset_for_seeking += write_count;
            if(size < descriptors[fildes].offset_for_seeking){
                allocated->size = descriptors[fildes].offset_for_seeking;
            }
            return write_count;
        }
    }
    if(fildes < FIRST_FILE_DESCRIPTOR){
        return -1;
    }
    if(fildes > LAST_FILE_DESCRIPTOR){
        return -1;
    }
    if(buf == NULL){
        return -1;
    }
    if(nbyte < FIRST_BYTE_OF_BLOCK){
        return -1;
    }
    if(nbyte > LAST_BYTE_OF_BLOCK){
        return -1;
    }
}

//get_file_descriptor
int get_file_descriptor(int index_of_file){
    if(index_of_file >= FIRST_FILE_INDEX && index_of_file <= LAST_FILE_INDEX){
        for(i = FIRST_FILE_DESCRIPTOR; i <= LAST_FILE_DESCRIPTOR; i++){
            if(!descriptors[i].file_descriptor_in_use){
                descriptors[i].file_descriptor_in_use = -1;
                descriptors[i].file_name = (char) "";
                descriptors[i].offset_for_seeking = 0;
                return i;
            }
            else{
                printf("no file descriptors\n");
                return -1;
            }
        }
    }
    if(index_of_file < FIRST_FILE_INDEX){
        return -1;
    }
    if(index_of_file > LAST_FILE_INDEX){
        return -1;
    }
}

//make_fs
int make_fs(char* disk_name){
    if(disk_name != NULL){
        status1 = make_disk(disk_name);
        if (status1 != 0){
            return status1;
        }
        if (status1 == 0){
            return -1;
        }
        status2 = open_disk(disk_name);
        if (status2 != 0){
            return status2;
        }
        if (status2 == 0){
            return -1;
        }
        disk_vol = (VOLUME*)malloc(sizeof(SIZE_OF_DISK));
        disk_vol->root_directory_ptr.num_directories_contained = 0;
        disk_vol->root_directory_ptr.index_of_data = 2;
        disk_vol->root_directory_ptr.directory_index = 1;
        char * buffer_for_volume = (char*) calloc(BLOCK_SIZE, sizeof(char));
        memcpy(buffer_for_volume, disk_vol, sizeof(SIZE_OF_DISK));
        block_write(0, buffer_for_volume);
        fat_table = (FAT_TABLE*)malloc((MAXIMUM_NUMBER_OF_FILES) * sizeof(fat_table));
        fat_table->root_directory_ptr.root_directory = 0;
        for(i = FIRST_FILE_INDEX + 1; i <= LAST_FILE_INDEX; i++){
            fat_table->fat[i].file_name = (char) "";
            fat_table->fat[i].file_extension = (char) "";
            fat_table->fat[i].file_size = 0;
            fat_table->fat[i].first_block = 0;
            fat_table->fat[i].last_block = 0;
            fat_table->fat[i].create_date = (char) "";
            fat_table->fat[i].create_time = (char) "";
            fat_table->fat[i].modified_date = (char) "";
            fat_table->fat[i].modified_time = (char) "";
            fat_table->fat[i].last_access = (char) "";
            fat_table->fat[i].offset = 0;
            fat_table->fat[i].block = i;
            fat_table->fat[i].entries[i] = -1;
            fat_table->fat[i].blocks[i] = -1;
            fat_table->fat[i].status = UNUSED;
        }
        char * buffer_for_fat_table = (char*) calloc((MAXIMUM_NUMBER_OF_FILES), sizeof(char));
        for(i = FIRST_FILE_INDEX; i <= LAST_FILE_INDEX; i++){
            block_write(i, buffer_for_fat_table);
        }
        memcpy(buffer_for_fat_table, fat_table, sizeof(FAT_TABLE));
        directory = (DIR*)malloc((MAXIMUM_NUMBER_OF_DIRECTORIES) * sizeof(directory));
        directory->directories[0] = fat_table->root_directory_ptr.root_directory;
        for(i = 1; i < MAXIMUM_NUMBER_OF_DIRECTORIES; i++){
            directory->directories[i] = (char) "";
        }
        char * buffer_for_directory_names = (char*) calloc((MAXIMUM_NUMBER_OF_DIRECTORIES), sizeof(char));
        for(i = 0; i < MAXIMUM_NUMBER_OF_DIRECTORIES; i++){
            block_write(i, buffer_for_directory_names);
        }
        memcpy(buffer_for_directory_names, directory, sizeof(DIR));
        free(buffer_for_volume);
        free(buffer_for_fat_table);
        free(buffer_for_directory_names);
        free(disk_vol);
        free(fat_table);
        free(directory);
        close_disk();
        return 0;
    }
    if(disk_name == NULL){
        return -1;
    }
}

//mount_fs
int mount_fs(char* disk_name){
    if(disk_name != NULL){
        status = open_disk(disk_name);
        if (status != 0){
            return status;
        }
        char * buffer_for_volume = (char*) calloc(BLOCK_SIZE, sizeof(char));
        block_read(0, buffer_for_volume);
        disk_vol = (VOLUME*)malloc(sizeof(SIZE_OF_DISK));
        memcpy(disk_vol, buffer_for_volume, sizeof(VOLUME));
        allocation = (FILE_ALLOCATION*)calloc(1, sizeof(SIZE_OF_DISK));
        buffer_for_volume = (char*)calloc(BLOCK_SIZE, sizeof(char));
        block_read(disk_vol->root_directory_ptr.directory_index, buffer_for_volume);
        memcpy(allocation, buffer_for_volume, sizeof(allocation) * disk_vol->root_directory_ptr.num_directories_contained);
        for (i = FIRST_FILE_DESCRIPTOR; i <= LAST_FILE_DESCRIPTOR; i++){
            descriptors->file_descriptor_in_use = 0;
        }
        char * buffer_for_fat_table = (char*) calloc((MAXIMUM_NUMBER_OF_FILES), sizeof(char));
        for(i = FIRST_FILE_INDEX + 1; i <= LAST_FILE_INDEX; i++){
            block_read(i, buffer_for_fat_table);
        }
        fat_table = (FAT_TABLE*) malloc(sizeof(MAXIMUM_NUMBER_OF_FILES));
        memcpy(fat_table, buffer_for_fat_table, sizeof(FAT_TABLE));
        allocation = (FILE_ALLOCATION*)calloc(1, sizeof(MAXIMUM_NUMBER_OF_FILES));
        buffer_for_fat_table = (char*) calloc((MAXIMUM_NUMBER_OF_FILES), sizeof(char));
        block_read(fat_table->file_allocation_ptr.file_index, buffer_for_fat_table);
        memcpy(allocation, buffer_for_fat_table, sizeof(allocation) * fat_table->file_allocation_ptr.num_files_contained);
        for(i = FIRST_FILE_DESCRIPTOR; i <= LAST_FILE_DESCRIPTOR; i++){
            descriptors->file_descriptor_in_use = 0;
        }
        char* buffer_for_directory_names = (char*)calloc(MAXIMUM_NUMBER_OF_DIRECTORIES, sizeof(char));
        block_read(0, buffer_for_directory_names);
        directory = (DIR*)malloc(sizeof(MAXIMUM_NUMBER_OF_DIRECTORIES));
        memcpy(directory, buffer_for_directory_names, sizeof(DIR));
        allocation = (FILE_ALLOCATION*)calloc(0, sizeof(MAXIMUM_NUMBER_OF_DIRECTORIES));
        buffer_for_directory_names = (char*) calloc(MAXIMUM_NUMBER_OF_DIRECTORIES, sizeof(char));
        block_read((int) directory->directories[i], buffer_for_directory_names);
        memcpy(allocation, buffer_for_directory_names, sizeof(allocation) * directory->directories[i]);
        return 0;
    }
    if(disk_name == NULL){
        return -1;
    }
}

//name_for_file_system_char_count
int name_for_file_system_char_count(char* name_for_file_system){
    int name_for_file_system_char_count = strlen(name_for_file_system);
    if(name_for_file_system_char_count == 1 || name_for_file_system_char_count <= LENGTH_OF_NAME_FOR_THE_FILE_SYSTEM){
        return name_for_file_system_char_count;
    }
    if(name_for_file_system_char_count < 1){
        return -1;
    }
    if(name_for_file_system_char_count > LENGTH_OF_NAME_FOR_THE_FILE_SYSTEM){
        return -1;
    }
}

//unmount_fs
int unmount_fs(char* disk_name){
    if(disk_name != NULL){
        files = allocation;
        char * allocation_block_buffer = (char*) calloc(BLOCK_SIZE, sizeof(char));
        char * original_ptr = allocation_block_buffer;
        for (i = FIRST_FILE_INDEX; i <= LAST_FILE_INDEX; i++){
            if(files[i].num_files_used){
                memcpy(allocation_block_buffer, &allocation[i], sizeof(allocation[i]));
                allocation_block_buffer += sizeof(allocation);
            }
        }
        block_write(disk_vol->root_directory_ptr.directory_index, allocation_block_buffer);
        free(original_ptr);
        for(i = FIRST_FILE_DESCRIPTOR; i <= LAST_FILE_DESCRIPTOR; i++){
            if(descriptors[i].file_descriptor_in_use){
                descriptors[i].file_descriptor_in_use = 0;
                descriptors[i].file_name = (char) "";
                descriptors[i].offset_for_seeking = 0;
            }
        }
        free(allocation);
        close_disk();
        return 0;
    }
    if(disk_name == NULL){
        return -1;
    }
}

//unused_entries
int unused_entries(){
    for(i = 0; i < MAXIMUM_NUMBER_OF_FILES; i++){
        if(fat_table->fat[i].status == UNUSED){
            fat->unused_entries[i] = UNUSED;
        }
    }
    return 0;
}

//used_entries
int used_entries(){
    for(i = 0; i < MAXIMUM_NUMBER_OF_FILES; i++){
        if(fat_table->fat[i].status == USED){
            fat->used_entries[i] = USED;
        }
    }
    return 0;
}
