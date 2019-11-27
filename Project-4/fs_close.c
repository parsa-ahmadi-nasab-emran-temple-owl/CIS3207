#include "my_file_system.h";

int fs_close(int fildes){
    if(&fildes != NULL){
        if(fildes < 0) {
            return -1;
        }
        if (fildes >= 64) {
            return -1;
        }
        if(descriptors[fildes].file_descriptor_in_use == 0) {
            return -1;
        }
        file_descriptor* descriptor_ptr = &(descriptors[fildes]);
        allocation[descriptor_ptr->file_name].num_file_descriptors = allocation[descriptor_ptr->file_name].num_file_descriptors - 1;
        descriptor_ptr->file_descriptor_in_use = false;
        
        printf("file at given file descriptor is closed\n");
        printf("file at given file descriptor is closed\n");
        
        return 0;
    }
    else{
        return -1;
    }
}
