int mount_fs(char* disk_name){
    if(disk_name != NULL){
        status = open_disk(disk_name);
        if (status != 0){
            return status;
        }
        char * buffer_for_volume = (char*) calloc(BLOCK_SIZE, sizeof(char));
        block_read(0, buffer_for_volume);
        disk_vol = (VOLUME*)malloc(sizeof(SIZE_OF_DISK));
        memcpy(disk_vol, buffer_for_volume, sizeof(VOLUME));
        allocation = (FILE_ALLOCATION*)calloc(1, sizeof(SIZE_OF_DISK));
        buffer_for_volume = (char*)calloc(BLOCK_SIZE, sizeof(char));
        block_read(disk_vol->root_directory_ptr.directory_index, buffer_for_volume);
        memcpy(allocation, buffer_for_volume, sizeof(allocation) * disk_vol->root_directory_ptr.num_directories_contained);
        for (i = FIRST_FILE_DESCRIPTOR; i <= LAST_FILE_DESCRIPTOR; i++){
            descriptors->file_descriptor_in_use = 0;
        }
        char * buffer_for_fat_table = (char*) calloc((MAXIMUM_NUMBER_OF_FILES), sizeof(char));
        for(i = FIRST_FILE_INDEX + 1; i <= LAST_FILE_INDEX; i++){
            block_read(i, buffer_for_fat_table);
        }
        fat_table = (FAT_TABLE*) malloc(sizeof(MAXIMUM_NUMBER_OF_FILES));
        memcpy(fat_table, buffer_for_fat_table, sizeof(FAT_TABLE));
        allocation = (FILE_ALLOCATION*)calloc(1, sizeof(MAXIMUM_NUMBER_OF_FILES));
        buffer_for_fat_table = (char*) calloc((MAXIMUM_NUMBER_OF_FILES), sizeof(char));
        block_read(fat_table->file_allocation_ptr.file_index, buffer_for_fat_table);
        memcpy(allocation, buffer_for_fat_table, sizeof(allocation) * fat_table->file_allocation_ptr.num_files_contained);
        for(i = FIRST_FILE_DESCRIPTOR; i <= LAST_FILE_DESCRIPTOR; i++){
            descriptors->file_descriptor_in_use = 0;
        }
        char* buffer_for_directory_names = (char*)calloc(MAXIMUM_NUMBER_OF_DIRECTORIES, sizeof(char));
        block_read(0, buffer_for_directory_names);
        directory = (DIR*)malloc(sizeof(MAXIMUM_NUMBER_OF_DIRECTORIES));
        memcpy(directory, buffer_for_directory_names, sizeof(DIR));
        allocation = (FILE_ALLOCATION*)calloc(0, sizeof(MAXIMUM_NUMBER_OF_DIRECTORIES));
        buffer_for_directory_names = (char*) calloc(MAXIMUM_NUMBER_OF_DIRECTORIES, sizeof(char));
        block_read((int) directory->directories[i], buffer_for_directory_names);
        memcpy(allocation, buffer_for_directory_names, sizeof(allocation) * directory->directories[i]);
        return 0;
    }
    if(disk_name == NULL){
        return -1;
    }
}
