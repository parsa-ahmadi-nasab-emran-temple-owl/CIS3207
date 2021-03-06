# CIS3207
Project 4: Implementing a Simple File System on Top of a Virtual Disk
data structures:
super block
  the first block of disk
  it stores information about the location of the other data structures
  for example, it can be stored in the upper block the whereabouts of the file allocation table (FAT), the directory, and the  start of the data blocks
root directory
  it holds the names of the files
  when using a FAT-based design, the directory also stores, for each file, its file size and the head of the list of corresponding data block
file allocation table (FAT)
  is convenient because it can be used to keep track of empty blocks and the mapping between files and their data blocks
I will be modeling my file system after the FAT (file allocation table) design.
Also, I will be borrowing ideas from existing file system designs to make it easier to implement it.

The header file for my file system will include the following:
  #define int make_fs(char* disk_name);
  #define int mount_fs(char* disk_name);
  #define int unmount_fs(char* disk_name);
  #define int fs_open(char* name);
  #define int fs_close(int fildes);
  #define int fs_create(char* name);
  #define int fs_delete(char* name);
  #define int fs_mkdir(char* name);
  #define int fs_read(int fildes, void* buf, size_t nbyte);
  #define int fs_write(int fildes, void* buf, size_t nbyte);
  #define int fs_get_filesize(int fildes);
  #define int fs_lseek(int fildes, off_t offset);
  #define int fs_truncate(int fildes, off_t offset);
  #define char* disk_name;
  #define char* name;
  #define int fildes;
  #define void* buf;
  #define size_t nbyte;
  #define off_t offset;
  #define off_t length;
  #define int length_of_block = 8192;
  #define int first_block = 0;
  #define int last_block = 16383;
  #define int size_disk = 4429185024;
  #define int current_number_of_files = 0;
  #define int number_of_files = 256;
  #define int length_of_directory_name = 15;
  #define int length_of_file_name = 15;
  #define int length_of_current_file_name = 0;
  #define int length_of_current_directory_name = 0;
  #include <stdio.h>;
  #include <stdlib.h>;
  #include "disk.c";
  #include "disk.h";
  #include "make_fs.c";
  #include "mount_fs.c";
  #include "unmount_fs.c";
  #include "fs_open.c"
  #include "fs_close.c";
  #include "fs_create.c";
  #include "fs_delete.c";
  #include "fs_mkdir.c";
  #include "fs_write.c";
  #include "fs_lseek.c";
  #include "fs_read.c"
  #include "fs_get_filesize.c";
  #include "fs_truncate.c";
  #include "my_test_program.c";
  #define int maximum_file_size = 32;
  #define int first_file_descriptor = 0;
  #define int last_file_descriptor = 63;
  #define int fd;
  #include <sys/mman.h>;
  
To manage my file system, I have to provide the following three functions:
- int make_fs(char* disk_name);
    It will have its own make_fs.c file. (For organization purposes)
    creates a fresh and empty file system on the virtual disk
    make_disk(disk_name);
      first invoke this function to create a new disk
    invoke nmap(void* start, size_t, int port, int flags, int fd, off_t offset) to create the FAT table for the file system
    open this disk and write and initialize the necessary meta-information for my file system so that it can be later used (mounted)
    returns 0 on success
    returns -1 when the disk_name could not be created, opened, or properly initialized
int make_fs(char* disk_name){
	if(disk_name != null){
		first invoke make_disk(disk_name); to create a new disk
		open the disk using the fopen file pointer to open the disk
		write and initialize the necessary meta-information for the disk
		return 0;
	}
	else{
		return -1;
	}
}
- int mount_fs(char* disk_name);
    It will have its own mount_fs.c file. (For organization purposes)
    mounts my file system that is stored on a virtual disk with name disk_name
    my file system becomes ready for use
    I need to open the disk and then load the meta-information that is necessary to handle the file system operations.
    returns 0 on success
    returns -1 on failure:
      when the disk_name could not be opened
      when the disk does not contain a valid file system
int mount_fs(char* disk_name){
	if(disk_name != null){
		open the disk using the fopen file pointer to open the disk
		load the meta-information that is used for handling the file system operations
		return 0;
	}
	else{
		return -1;
	}
}
- int unmount_fs(char* disk_name);
    It will have its own unmount_fs.c file. (For organization purposes)
    unmounts my file system from a virtual disk with name disk_name
    As part of this operation, I need to write back all meta-information so that the disk persistently reflects all changes that were made to the file system.
    I should also close the disk.
    returns 0 on success
    returns -1 on failure:
      when the disk disk_name could not be closed
      when data could not be written to the disk
    If there are any file descriptors(that point to files in the file system that is about to be unmounted), this function will close them.
int unmount_fs(char* disk_name){
	if(disk_name != null){
		write back all meta-information so the disk persistently reflects al changes that were made to the file system
		close the disk
		return 0;
	}
	else{
		return -1;
	}
} 

In addition to the management routines, I am to implement the following file system functions(which are very similar to the corresponding Linux file system operations). These file system fuctions require that a file system was previously mounted. The file systems functions are as follows:
- int fs_open(char* name);
    It will have its own fs_open.c file. (For organization purposes)
    The file specified by name(in this case, name is a path to the file that is to be opened, including the actual filename part of the path) is opened for reading and writing, and the file descriptor corresponding to this file is returned to the calling function and add it to the FAT.
    If successful, it returns a non-negative integer, which is a file descriptor that can be used to subsequently access this file.
    Note that the same file(file with the same name) can be opened multiple times.
    When this happens, the file system is supposed to provide multiple, independent file descriptors.
    returns -1 on failure:
      when the file with name can't be found
      when there are already 64 file descriptors open
     When a file is opened, the file offset(seek pointer) is set to 0(the beginning of the file).
int fs_open(char* name){
	if(name != null){
		open the file specified by name for reading and writing using fopen file pointer
		add it to the FAT
		the file descriptor is returned to the calling function
		returns the file descriptor
	}
	else{
		returns -1;
	}
}
- int fs_close(int fildes);
    It will have its own fs_close.c file. (For organization purposes)
    The file descriptor fildes is closed.
    A closed file descriptor can no longer be used to access the corresponding file and remove it from the FAT.
    Upon successful completion, a value of 0 is returned.
    In case the file descriptor fildes doesn't exist or isn't open, it returns -1.
int fs_close(int fildes){
	if(fildes != null){
		close the file descriptor fildes that corresponds to its accessed file
		remove it from the FAT
		return 0;
	}
	else{
		return -1;
	}
}
- int fs_create(char* name);
    It will have its own fs_create.c file. (For organization purposes)
    It creates a new file with name name in the file system(name is the path to the file including the name of the file itself) and add it to the FAT.
    The file is initially empty.
    The maximum length for a file name is 15 characters(this is also the maximum length of a directory name).
    Also, there can be at most 256 files in the directory that are also included in the FAT.
    Upon successful completion, a value of 0 is returned.
    It returns -1 on failure:
      when the file with name already exists(using the full path specified in name) in the FAT
      when the file name is too long(it exceeds 15 characters for the directory name and 15 characters for the name of the file)
      when there are already 256 files present in a specified directory
int fs_create(char* name){
	if(name != null && current_number_files < number_of_files && length_of_current_file_name < length_of_file && length_of_current_directory_name < length_of_directory_name){
		create a new file and map it to its entry in the FAT
		include its information in the meta-information file
		go through all data blocks in the file system and grouped the occupied ones together and group the unoccupied ones together
		put the grouped occupied ones to the left of the grouped unoccupied ones
		put the grouped unoccupied ones to the right of the grouped occupied ones
		return 0;
	}
	else{
		return -1;
	}
}
- int fs_delete(char* name);
    It will have its own fs_delete.c file. (For oyyrganization purposes)
    It deletes the file with the path and name name from the directory of the file system and frees all data blocks and meta-information that corresponds to that file and removes it from the FAT.
    The file that is being deleted must not be opened. (In other words, the file that is being deleted must be closed.)
    That is, there can't be any open file descriptor that refers to the file name.
    When the file is open at the time that this function is called, the call fails and the file is not deleted.
    Upon successful completion, a value of 0 is returned.
    It returns -1 on failure:
      when the file with name doesn't exist
      when the file is currently open(there exists at least one open file descriptor that is associated with this file)
int fs_delete(char* name){
	if(name != null && the file is not open && the file descriptor(s) is/are not open){
		close the file descriptor
		close the file
		remove its entry from the FAT
		update its information in the meta information file
		return 0;
	}
	else{
		return -1;
	}
}
- int fs_mkdir(char* name);
    It will have its own fs_mkdir.c file. (For organization purposes)
    It attempts to create a directory with the name name.
    It returns 0 on success.
    It returns -1 on failure:
      when the directory with name already exists
      when the file name is too long(it exceeds 15 characters for the directory name)
      when there are already 256 files present in the directory
int fs_mkdir(char* name){
	if(name != null && length_of_current_directory_name < length_of_directory_name && current_number_files < number_of_files){
		create a new directory and map it to its entry in the FAT
		include its information in the meta-information file
		return 0
	}
	else{
		return -1;
	}
}
- int fs_write(int fildes, void* buf, size_t nbyte);
    It will have its own fs_write.c file. (For organization purposes)
    It attempts to write nbyte bytes of data to the file referenced by the descriptor fildes from the buffer printed by the buf.
    It assumes that the buffer buf holds at least nbyte bytes.
    When it attempts to write past the end of the file, the file is automatically extended to hold additional bytes.
    In this case, it attempts to write as many bytes as possible(i.e., to fill up the entire space that is left).
    Upon successful completion, the number of bytes that were actually written is returned.
    This number could be smaller that nbyte when the disk runs out of space(when writing to a full disk, the function returns 0).
    It returns -1 on failure when the file descriptor fildes isn't valid.
    It implicitely increments the file pointer by the number of bytes that were actually written.
int fs_write(int fildes, void* buf, size_t nbyte){
	if(all bytes of the file have not been filled && fildes >= 0 && nbyte != maximum_file_size){
		keep writing to the file
		update its entry in the FAT
		update its information in the meta-information file
		return 0;
	}
	else{
		return -1;
	}
}
- int fs_lseek(int fildes, off_t offset);
    It will have its own fs_lseek.c file. (For organization purposes)
    It sets the file pointer(the offset used for read and write operations) associated with the file descriptor fildes to the argument offset.
    It is an error to set the file pointer beyond the end of the file.
    To append to a file, one can set the file pointer to the end of a file, for example. by calling fs_lseek(fd, fs_get_filesize(fd));
    Upon successful completion, a value of 0 is returned.
    It returns -1 on failure:
      when the file descriptor fildes in invalid
      when the requested offset is larger than the file size
      when the offset is less than 0
int fs_lseek(int fildes, off_t offset){
	if(fildes >= 0 && offset >= 0){
		set the file pointer associated with the file descriptor fildes to argument offset
		update the file pointer with fildes to offset in the FAT
		update its information in the meta-information table
		return 0;
	}
	if(fildes == end of file){
		return -1;
	}
	else{
		return -1;
	}
}
- int fs_read(int fildes, void* buf, size_t nbyte);
    It will have its own fs_read.c file. (For organization purposes)
    It attempts to read nbyte bytes of data from the file referenced by the descriptor fildes into the buffer pointed to by buf.
    It assumes that the buffer buf is large enough to hold at least nbyte bytes.
    When it attempts to read past the end of the file, it reads all bytes until the end of the file.
    Upon successful completion, the number of bytes that were actually read will be returned.
    This number could be smaller than nbyte when attempting to read past the end of the file(when trying to read while the file pointer is at the end of the file, it returns 0).
    It returns -1 on error when file descriptor fildes isn't valid.
    This function implicitely increments the file pointer by the number of bytes that were actually read.
int fs_read(int fildes, void* buf, size_t nbyte){
	if(fildes >=0 && buf != null && nbyte >= 0 && buf is not full && nbyte != the end of file){
		read nbyte bytes data from the file referenced by fildes in to the buffer pointed to by buf using the information in its FAT entry
		return the number of bytes that were actually read
	}
	if(nbyte == end of file){
		return 0
	}
	else{
		return -1;
	}
}
- int fs_get_filesize(int fildes);
    It will have its own fs_get_filesize.c file. (For organization purposes)
    It returns the current size of the file pointed to by the file descriptor fildes.
    In case fildes isn't valid, it returns -1.
int fs_get_filesize(int fildes){
	if(fildes != null && current_size_the_file != null){
		return the size of the file using the information in its FAT entry
	}
	else{
		return -1;
	}
}
- int fs_truncate(int fildes, off_t length);
    It will have its own fs_truncate.c file. (For organization purposes)
    It causes the file referenced by fildes to be truncated to length bytes in size.
    If the file was previously larger than this new size, the extra data are lost and the corresponding data blocks on disk(if any) must be freed.
    It's not possible to extend the file using this function.
    When the file pointer is larger than the new length, then it's also set to length(the end of the file).
    Upon successful completion, a value of 0 is returned.
    It returns -1 on failure: when the file descriptor fildes is invalid or the requested length is larger than the file size.
int fs_truncate(int fildes, off_t length){
	if(fildes >= 0 && length >= 0){
		write the extra data to a text file
		truncate the size of the file to length in size
		file pointer is set to length(which is the end of the file)
		update it in its FAT entry
		update its information in the meta-information file
		return 0
	}
	else{
		return -1;
	}
}

My Test Program:
It will have its own my_test_program.c file. (For organization purposes)
I will write a small program that demonstrates the creation of the virtual disk and file system.
To demonstrate the file system implementation:
  It is required to develop a program that uses all of the functions in the file system.
This program will mount the virtual disk and then demonstrate copying a file from the real OS file system to my file system and copying a file from my file system to the OS file system by re-running it and then showing the directory and virtual disk contents.
