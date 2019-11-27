#include "my_file_system.h";

int mount_fs(char* disk_name){
    
    if(disk_name != NULL){
        
        status = open_disk(disk_name);
        if (status != 0) {
            return status;
        }
        
        // Create buffer to read volume from disk
        char * buffer_for_volume = (char*) calloc(BLOCK_SIZE, sizeof(char));
        block_read(0, buffer_for_volume);
        disk_vol = (struct Volume*)malloc(sizeof(Volume));
        memcpy(disk_vol, buffer_for_volume, sizeof(struct Volume));
        
        allocation = calloc(1, sizeof(struct file_allocation));
        free(buffer_for_volume);
        buffer_for_volume = (char*) calloc(BLOCK_SIZE, sizeof(char));
        block_read(disk_vol->root_directory.directory_index, buffer_for_volume);
        
        memcpy(allocation, buffer_for_volume, sizeof(file_allocation) * disk_vol->root_directory.num_directories_contained);
        
        for (int i = 0; i < MAX_FILE_DESCRIPTOR; i++)
        {
            descriptors.file_descriptor_in_use = 0;
        }
        
        return 0;
    }
    else{
        return -1;
    }
}
