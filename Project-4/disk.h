#ifndef _DISK_H_
#define _DISK_H_

/******************************************************************************/
#define DISK_BLOCKS  16384      /* number of blocks on the disk                */
#define BLOCK_SIZE   4096      /* block size on "disk" - I changed it to 8192 bytes since I need it for the implementation of my file system                        */

/******************************************************************************/
int make_disk(char *name);     /* create an empty, virtual disk file          */
int open_disk(char *name);     /* open a virtual disk (file)                  */
int close_disk();              /* close a previously opened disk (file)       */

int block_write(int block, char *buf);
                               /* write a block of size BLOCK_SIZE to disk    */
int block_read(int block, char *buf);
                               /* read a block of size BLOCK_SIZE from disk   */
/******************************************************************************/

#endif
