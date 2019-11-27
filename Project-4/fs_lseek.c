#include "my_file_system.h";

int fs_lseek(int fildes, off_t offset){
    if(fildes != NULL && fildes >= 0 && fildes <= 63 && offset != NULL && offset >= 0){
        if (offset > allocation[descriptors[fildes].file_descriptor_in_use].file_size || offset < 0){
            printf("error: Can't set the file pointer beyond the file range.\n");
            return -1;
        } else if(!allocation[fildes].file_size){
            printf("error: Invalid file descriptor.\n");
            return -1;
        } else {
            descriptors[fildes].offset_for_seeking = (int) offset;
            printf("This function was called successfully.\n");
            return 0;
        }
    }
    else{
        return -1;
    }
}
