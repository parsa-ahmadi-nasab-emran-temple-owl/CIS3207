#include "my_file_system.h";

int find_free_block(int index_of_file) {
    if (index_of_file != NULL && index_of_file >= 0 && index_of_file <= 255){
        char first_block_buffer[BLOCK_SIZE] = "";
        char second_block_buffer[BLOCK_SIZE] = "";
        block_read(disk_vol->index_of_data, first_block_buffer);
        block_read(disk_vol->index_of_data + 1, second_block_buffer);
        for(int i = 0; i < BLOCK_SIZE; i++) {
            if(first_block_buffer[i] == '\0') {
                first_block_buffer[i] = (int)(index_of_file + 1);
                block_write(disk_vol->index_of_data, first_block_buffer);
                return i;
            }
        }
        for(i = 0; i < BLOCK_SIZE; i++) {
            if(second_block_buffer[i] == '\0') {
                second_block_buffer[i] = (int)(index_of_file + 1);
                block_write(disk_vol->index_of_data, second_block_buffer);
                return i;
            }
        }
        else{
            printf("error: no available blocks.\n");
            return -1;
        }
    }
}
