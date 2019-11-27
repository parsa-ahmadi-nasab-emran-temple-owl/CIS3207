#include "my_file_system.h";

int get_file_descriptor(int index_of_file)
{
    for(int i = 0; i < 64; i++) {
        if(!descriptors[i].file_descriptor_in_use) {
            descriptors[i].file_descriptor_in_use = true;
            descriptors[i].file_name = index_of_file;
            descriptors[i].offset_for_seeking = 0;
            return i;
        }
    }
    printf("no file descriptors\n");
    return -1;
}
