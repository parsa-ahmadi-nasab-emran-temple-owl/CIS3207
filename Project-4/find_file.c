#include "my_file_system.h";

int find_file(char* name){
    if (name != NULL) {
        for(int i = 0; i < 256; i++) {
            if(allocation[i].file_descriptor_in_use == 1 && strcmp(allocation[i].name, name) == 0){
                return i;
            }
        }
    }
    else{
        return -1;
    }
}
