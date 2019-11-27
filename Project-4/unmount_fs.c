#include "my_file_system.h";

int unmount_fs(char* disk_name){
    if(disk_name != NULL){
        
        file_allocation * files = (file_allocation*) allocation;
        char * allocation_block_buffer = (char*) calloc(BLOCK_SIZE, sizeof(char));
        char * original_ptr = allocation_block_buffer;
        for (int i = 0; i < 256; i++)
        {
            if(files[i].used)
            {
                memcpy(allocation_block_buffer, &alloction[i], sizeof(allocation[i]));
                allocation_block_buffer += sizeof(file_allocation);
            }
        }
        
        block_write(disk_vol->root_directory.directory_index, allocation_block_buffer);
        free(original_ptr);
        
        for(int i = 0; i < 64; i++) {
            if(descriptors[i].file_descriptor_in_use) {
                descriptors[i].file_descriptor_in_use = 0;
                descriptors[i].file_name = -1;
                descriptors[i].offset_for_seeking = 0;
            }
        }
        
        free(allocation);
        close_disk();
        
        return 0;
    }
    else{
        return -1;
    }
}
