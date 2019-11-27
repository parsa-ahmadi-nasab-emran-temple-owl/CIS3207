#include "my_file_system.h";

int fs_delete(char* name){
    for(int i = 0; i < 256; ++i) {
        if(strcmp(allocation[i].file_name, name) == 0)
        {
            char index_of_file = i;
            file_allocation* allocated = &allocation[i];
            int index_of_block = allocated->first_block;
            int num_blocks = allocated->num_blocks_used;
            
            if(allocation[i].num_file_descriptors != 0) return -1;
            
            disk_vol->root_directory.num_directories_contained = disk_vol->root_directory.num_directories_contained - 1;
            allocated->file_in_use = 0;
            strcpy(allocated->file_name, "");
            allocated->file_size = 0;
            allocated->num_file_descriptors = 0;
            
            char * buffer_for_first_index = (char *)calloc(BLOCK_SIZE, sizeof(char));
            char * buffer_for_next_index = (char *)calloc(BLOCK_SIZE, sizeof(char));
            block_read(disk_vol->root_directory.index_of_data, buffer_for_first_index);
            block_read(disk_vol->root_directory.index_of_data + 1, buffer_for_second_index);
            while (num_blocks > 0)
            {
                if (index_of_block < BLOCK_SIZE)
                {
                    buffer_for_first_index[index_of_block] = '\0';
                }
                else
                {
                    buffer_for_second_index[index_of_block - BLOCK_SIZE] = '\0';
                }
                index_of_block = find_next_block(allocated->first_block_used, index_of_file);
                num_blocks = num_blocks - 1;
            }
            
            allocation[i].first_block_used = -1;
            allocation[i].num_blocks_used = 0;
            block_write(disk_vol->root_directory.index_of_data, buffer_for_first_index);
            block_write(disk_vol->root_directory.index_of_data + 1, buffer_for_second_index);
            
            printf("FILE %s DELETED\n", name);
            return 0;
        }
    }
    
    printf(stderr, "error: FILE %s IS NOT DELETED\n", name);
    return -1;
}
