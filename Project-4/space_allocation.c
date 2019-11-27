#include "my_file_system.h"

struct Volume{
    char name[NAME_LEN];
    char img[PATH_LEN];
    int state;
    unsigned int used_size;
    struct VolumeHeader *vbr;
    struct FAT* fat_table;
    struct RootDir root_directory;
    struct Data_blub;
}

struct RootDir{
    int num_directories_contained;
    int index_of_data;
    int directory_index;
}

struct VolumeHeader{
    uint16_t bps;
    uint8_t spc;
    uint16_t num_fats;
    uint16_t spf;
    uint16_t root_entries;
    uint16_t root_entries;
    uint32_t total_entries;
}

struct FAT{
    int total_blocks;
    int used_blocks;
    int free_blocks;
    int* file_storing_location;
    char file_name[15];
    char directory_name[15];
    Entry meta_data;
}

struct Entry{
    char name[FILENAME_LENGTH];
    char ext[EXTENSION_LENGTH];
    uint8_t attribute;
    uint16_t create_time;
    uint16_t create_date;
    uint16_t last_access;
    uint16_t modified_time;
    uint16_t modified_date;
    uint16_t start_block;
    uint32_t size;
    unsigned int offset;
}

struct file_allocation {
    int file_in_use;
    char file_name[15];
    int file_size;
    int first_block_used;
    int num_blocks_used;
    int num_file_descriptors;
}

struct file_descriptor {
    int file_descriptor_in_use;
    char file_name;
    int offset_for_seeking;
}
