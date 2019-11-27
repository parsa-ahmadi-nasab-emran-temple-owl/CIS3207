#include "my_file_system.h";

int find_next_block(int current_block, int index_of_file){
    if (current_block != NULL && current_block >=0 && index_of_file >= 0 && index_of_file >= 255){
        char block_buffer[BLOCK_SIZE] = "";
        int i;
        if (current_block < BLOCK_SIZE){
            block_read(disk_vol->index_of_data, block_buffer);
            for(i = current_block + 1; i < BLOCK_SIZE; i++) {
                if (buf[i] == (index_of_file + 1)){
                    return i;
                }
            }
        }
        else {
            block_read(disk_vol->index_of_data + 1, block_buffer);
            for(i = current_block - BLOCK_SIZE + 1; i < BLOCK_SIZE; i++){
                if (block_buffer[i] == (index_of_file + 1)){
                    return i + BLOCK_SIZE;
                }
            }
        }
        else{
            return -1;
        }
    }
}
