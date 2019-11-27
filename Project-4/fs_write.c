#include "my_file_system.h";

int fs_write(int fildes, void* buf, size_t nbyte){
    if(fildes != NULL && fildes >= 0 && fildes <= 63 && buf != NULL && nbyte != NULL && nbyte >= 0 && nbyte <= 4096){
        if(!descriptors[fildes].file_descriptor_in_use) {
            return -1;
        }
        
        int i = 0;
        char *src = buf;
        char block[BLOCK_SIZE] = "";
        char index_of_file = descriptors[fildes].file_name;
        file_allocation* allocated = &allocation[index_of_file];
        int index_of_block = allocated->first_block_used;
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
            for(i = selected_descriptor_offset; i < BLOCK_SIZE; i++) {
                block[i] = src[write_count++];
                if (write_count == (int) nbyte || write_count == strlen(src)) {
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
        while(write_count < (int) nbyte && write_count < strlen(src) && num_blocks < allocated->num_blocks) {
            index_of_block = find_next_block(index_of_block, index_of_file);
            for(i = 0; i < BLOCK_SIZE; i++) {
                block[i] = src[write_count++];
                if(write_count == (int) nbyte || write_count == strlen(src)) {
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
        while(write_count < (int) nbyte && write_count < strlen(src)) {
            index_of_block = find_free_block(index_of_file);
            allocated->num_blocks++;
            if (allocated->first_block_used == -1){
                allocated->first_block_used = index_of_block;
            }
            if (index_of_block < 0){
                fprintf(stderr, "error: No free blocks.\n");
                return -1;
            }
            for(i = 0; i < BLOCK_SIZE; i++) {
                block[i] = src[write_count++];
                if(write_count == (int) nbyte || write_count == strlen(src)) {
                    block_write(block_index, block);
                    descriptors[fildes].offset_for_seeking += write_count;
                    if(size < descriptors[fildes].offset_for_seeking){
                        allocated->size = descriptors[fildes].offset_for_seeking;
                    }
                    return write_count;
                }
            }
            block_write(index_of_block, block);
        }
        
        descriptors[fildes].offset_for_seeking += write_count;
        if(size < descriptors[fildes].offset_for_seeking){
            allocated->size = descriptors[fildes].offset_for_seeking;
        }
        return write_count;
    }
    else{
        return -1;
    }
}
