#include "my_file_system.h";

int fs_open(char* name){
    if(name != NULL){
        char index_of_file = -1;
        
        for(int i = 0; i < 256; i++) {
            if(allocation[i].file_in_use == 1)
            {
                if (strcmp(allocation[i].file_name, name) == 0))
                {
                    break;
                }
            }
        }
        if (i != 256)
        {
            index_of_file = i;
        }
        if(file_index == -1)
        {
            printf(" file %s does not exist\n", name);
            return -1;
        }
        
        int file_description = get_description(file_index);
        if (file_description == -1){
            return file_description;
        }
        
        allocation[file_index].numFileDescriptors++;
        printf("FILE IS %s OPENED\n", name);
        return file_description;
    }
    else{
        return -1;
    }
}
