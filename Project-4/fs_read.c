#include "my_file_system.h";

int fs_read(int fildes, void* buf, size_t nbyte){
    if(fildes != NULL && fildes >= 0 && fildes <= 63 && buf != NULL && nbyte != NULL && nbyte >=0 && nbyte <= 4096){
        
        if (!descriptors[fildes].file_descriptor_in_use) {
            return -1;
        }
        
        char * data_buffer = buf;
        char * block_buffer = (char*) calloc(BLOCK_SIZE, sizeof(char));
        char index_of_file = descriptors[fildes].file_name;
        file_allocation* allocated = &allocation[index_of_file];
        int index_of_block = file->first_block_used;
        int num_blocks = 0;
        int selected_descriptor_offset = descriptors[fildes].offset_for_seek;
        
        while (selected_descriptor_offset >= BLOCK_SIZE){
            index_of_block = get_next_free_block(index_of_block, index_of_file);
            num_blocks++;
            selected_descriptor_offset  = selected_descriptor_offset - BLOCK_SIZE;
        }
        block_read(index_of_block, block_buffer);
        
        int num_blocks_read = 0;
        for(i = selected_descriptor_offset; i < BLOCK_SIZE; i++) {
            data_buffer[num_blocks_read] = block_buffer[i];
            num_blocks_read == num_blocks_read + 1;
            if (num_blocks_read == (int) nbyte) {
                descriptors[fildes].offset_for_seek = descriptors[fildes].offset_for_seeking + num_blocks_read;
                return num_blocks_read;
            }
        }
        num_blocks_read = num_blocks_read + 1;
        
        
        strcpy(block_buffer, "");
        while(num_blocks_read < (int)nbyte && num_blocks <= allocated->num_blocks_used) {
            block_index = find_next_block(index_of_block, index_of_file);
            strcpy(block_buffer,"");
            block_read(index_of_block, block_buffer);
            for(int i=0; i < BLOCK_SIZE; i++) {
                data_buffer[num_blocks_read] = block_buffer[i];
                num_blocks_read = num_blocks_read + 1;
                if(num_blocks_read == (int)nbyte ) {
                    descriptors[fildes].offset_for_seeking += num_blocks_read;
                    return num_blocks_read;
                }
            }
            num_blocks_read++;
        }
        descriptors[fildes].offset_for_seeking += num_blocks_read;
        return num_blocks_read;
    }
    else{
        return -1;
    }
}
