#include "my_file_system.h";

int find_free_file_descriptor(int index_of_file){
    if (index_of_file != NULL && index_of_file >= 0 && index_of_file <= 255){
        for(int i = 0; i < 64; i++) {
            if(descriptors[i].file_descriptor_in_use == false) {
                descriptors[i].file_descriptor_in_use = true;
                descriptors[i].alocated = index_of_file;
                descriptors[i].offset_for_seeking = 0;
                return i;
            }
        }
        
        printf("error: no available file descriptor.\n");
        return -1;
    }
}
