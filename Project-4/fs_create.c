#include "my_file_system.h";

int fs_create(char* name){
    if(name != NULL)
    {
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
            for(int i = 0; i < 256; i++) {
                if(!allocation[i].file_in_use) {
                    
                    Volume->root_directory.num_directories_contained = Volume->root_directory.num_directories_contained + 1;
                    
                    allocation[i].file_in_use = 1;
                    
                    strcpy(allocation[i].file_name, name);
                    
                    allocation[i].file_size = 0;
                    allocation[i].first_block_used = -1;
                    allocation[i].num_blocks_used = 0;
                    allocation[i].num_file_descriptors = 0;
                    printf("FILE %s CREATED\n", name);
                    return 0;
                }
            }
        }
        else
        {
            printf("error: file [%s] already exists\n", name);
            return 0;
        }
    }
    else{
        return -1;
    }
}
