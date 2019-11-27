#include "my_file_system.h";

int fs_get_filesize(int fildes){
    if(!descriptors[fildes].file_descriptor_in_use){
        printf("error: Invalid file descriptor.\n");
        return -1;
    }
    else{
        return allocation[descriptors[fildes].file_descriptor_in_use].file_size;
    }
}
