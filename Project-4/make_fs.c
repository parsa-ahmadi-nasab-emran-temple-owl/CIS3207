#include "my_file_system.h";

int make_fs(char* disk_name){
    if(disk_name != NULL){
        
        int status = make_disk(disk_name);
        if (status != 0){
            return status;z}
        
        status = open_disk(disk_name);
        if (status != 0) return status;
        
        disk_vol = (struct Volume*)malloc(sizeof(Volume));
        disk_vol.root_directory.num_directories_contained = 0;
        disk_vol.root_directory.index_of_data = 2;
        disk_vol.root_directory.directory_index = 1;
        
        char * buffer_for_volume = (char*) calloc(BLOCK_SIZE, sizeof(char));
        memcpy(buffer_for_volume, disk_vol, sizeof(struct Volume));
        block_write(0, buffer_for_volume);
        disk_vol.fat_table = (struct FAT*) malloc(256 *sizeof(struct FAT));
        disk_vol.root = &(disk_vol->fat_table[0]);
        free(buffer_for_volume);
        free(disk_vol);
        close_disk();
        return 0;
    }
    else{
        return -1;
    }
}
