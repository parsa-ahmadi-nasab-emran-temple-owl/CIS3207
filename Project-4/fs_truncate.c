#include "my_file_system.h";

int fs_truncate(int fildes, off_t offset){
    if(fildes != NULL && fildes >= 0 && fildes <= 63 && offset != NULL){
        char index_of_file = descriptors[fildes].allocated;
        file_allocation* allocated  = &allocation[index_of_file];
        
        if(!descriptors[fildes].file_descriptor_in_use){
            printf(stderr, "error: Invalid file descriptor.\n");
            return -1;
        }
        
        if (offset > allocated->size || offset < 0) {
            printf(stderr, "error: Can't set the offset beyond the file range.\n");
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
            if(descriptors[i].file_descriptor_in_use == true && descriptors[i].allocated == index_of_file) {
                descriptors[i].offset_for_seeking = (int) offset;
            }
        }
        
        else {
            printf("This function was called successfully.\n");
            return 0;
        }
        else{
            return -1;
        }
    }
}
