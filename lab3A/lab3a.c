// NAME: Jeffrey Chan, Jiwan Kang
// EMAIL: jeffschan97@gmail.com, jiwan226@gmail.com
// ID: 004-611-638, 704-624-623

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <inttypes.h>
#include <math.h>
#include <fcntl.h>
#include<time.h>
#include<sys/time.h>
#include "ext2_fs.h"

char* fileName;
int imageFD;
int outFD;
int groupCount;
int numValidDirectoryInodes = 0;

uint32_t i32;
uint16_t i16;
uint8_t i8;

const int SUPEROFF = 1024;
const int BLOCKSIZE = 1024;

struct ext2_super_block superSum;

struct ext2_group_desc* groupSum;

struct group_sum {
    int blockCount;
    int inodeCount;
    int freeblockCount;
    int freeinodeCount;
    int freeblockNum;
    int freeinodeNum;
    int blockinodeNum;
};

void formatTime(uint32_t timestamp, char* buf) {
	time_t epoch = timestamp;
	struct tm ts = *gmtime(&epoch);
	strftime(buf, 80, "%m/%d/%y %H:%M:%S", &ts);
}

void superSummary() {
    dprintf(outFD, "SUPERBLOCK,");

    pread(imageFD, &superSum, sizeof(struct ext2_super_block), SUPEROFF);

    //blockCount
	dprintf(outFD, "%d,", superSum.s_blocks_count);

	// inodeCount
	dprintf(outFD, "%d,", superSum.s_inodes_count);

	// blockSize
	dprintf(outFD, "%d,", 1024 << superSum.s_log_block_size);

	// inodeSize
	dprintf(outFD, "%d,", superSum.s_inode_size);

	// blockPG (blocks per group)
	dprintf(outFD, "%d,", superSum.s_blocks_per_group);

	// inodePG (inodes per group)
	dprintf(outFD, "%d,", superSum.s_inodes_per_group);

	// inodeNR (first non-reserved inode)
	dprintf(outFD, "%d\n", superSum.s_first_ino);

}

void groupSummary() {
	// Calculate number of groups
	groupCount = superSum.s_blocks_count / superSum.s_blocks_per_group + 1;

    //remaining blocks in last group
    int blockRem = superSum.s_blocks_count % superSum.s_blocks_per_group;
    //remaining blocks in last group
    int inodeRem = superSum.s_inodes_count % superSum.s_inodes_per_group;


	groupSum = malloc(groupCount*sizeof(struct ext2_group_desc));
    int STARTOFFSET = SUPEROFF + BLOCKSIZE;

	int i;
	for(i = 0; i < groupCount; i++) {
        //group number
        dprintf(outFD, "GROUP,%d,", i);

		// Number of blocks in group
		if(i != groupCount - 1 || blockRem == 0) {
			dprintf(outFD, "%d,", superSum.s_blocks_per_group);
		}
		else {
			dprintf(outFD, "%d,", blockRem);
		}

        // Number of inodes in group
		if(i != groupCount - 1 || inodeRem == 0) {
			dprintf(outFD, "%d,", superSum.s_inodes_per_group);
		}
		else {
			dprintf(outFD, "%d,", inodeRem);
		}

        pread(imageFD, &groupSum[i], sizeof(struct ext2_group_desc), STARTOFFSET + i*sizeof(struct ext2_group_desc));

		// Number of free blocks
		dprintf(outFD, "%d,", groupSum[i].bg_free_blocks_count);

        // Number of free inodes
		dprintf(outFD, "%d,", groupSum[i].bg_free_inodes_count);

		// Block number of free block bitmap for group
		dprintf(outFD, "%d,", groupSum[i].bg_block_bitmap);

        // Block number of free inode bitmap for group
		dprintf(outFD, "%d,", groupSum[i].bg_inode_bitmap);

		// Block number of first block of inodes in group
		dprintf(outFD, "%d\n", groupSum[i].bg_inode_table);
	}
}

void blockEntries() {
    int i;
    //for each group
	for(i = 0; i < groupCount; i++) {
		int j;
        //for each block in group
		for(j = 0; j < (1024 << superSum.s_log_block_size); j++) {
			pread(imageFD, &i8, 1, groupSum[i].bg_block_bitmap*(1024 << superSum.s_log_block_size) + j);

			int k;
			for(k = 0; k < 8; k++) {
				if((i8 & (1 << k)) == 0) {
					dprintf(outFD, "BFREE,%d\n", (i*superSum.s_blocks_per_group) + (j*8) + (k+1));
				}
			}
		}
    }
}

void inodeEntries() {
    int i;
    //for each group
	for(i = 0; i < groupCount; i++) {
		int j;
        //for each block in group
		for(j = 0; j < (1024 << superSum.s_log_block_size); j++) {
			pread(imageFD, &i8, 1, groupSum[i].bg_inode_bitmap*(1024 << superSum.s_log_block_size) + j);

			int k;
			for(k = 0; k < 8; k++) {
				if((i8 & (1 << k)) == 0) {
					dprintf(outFD, "IFREE,%d\n", (i*superSum.s_inodes_per_group) + (j*8) + (k+1));
				}
			}
		}
    }
}

void processDirectory(struct ext2_dir_entry directory, int parentInodeNumber, int logicalByteOffset) {
    if (directory.inode == 0) return;

    dprintf(outFD, "DIRENT,%d,%d,%d,%d,%d,'%s'\n", parentInodeNumber, logicalByteOffset, directory.inode,
            directory.rec_len, directory.name_len, directory.name);
}

void processIndirectory(struct ext2_dir_entry directory, int parentInodeNumber, int logicalByteOffset, int levelIndirect, int blockNum, int i){
    if (directory.inode == 0) return;

    dprintf(outFD, "INDIRECT,%d,%d,%d,%d,%d\n", parentInodeNumber, levelIndirect, logicalByteOffset, blockNum, blockNum + i);
}

void inodeSummary() {
    struct ext2_inode localInode;

    // iterate through each group
    int k = 0;
    for (k = 0; k < groupCount; k++) {
        int inodeTableOffset = SUPEROFF + 4*BLOCKSIZE; // offset at which inodeTable startds

        // go through all inodes in the table
        int m = 0;
        for (m = 0; m < superSum.s_inodes_count; m++) {
            pread(imageFD, &localInode, sizeof(struct ext2_inode), inodeTableOffset + m * sizeof(struct ext2_inode));
            if (localInode.i_mode != 0 && localInode.i_links_count != 0) {
        		// INODE and inode number
        		dprintf(outFD, "INODE,%d,", m + 1);

                // file type
                char fileType = '?';
                if (localInode.i_mode & 0xA000) fileType = 's';
                if (localInode.i_mode & 0x8000) fileType = 'f';
                if (localInode.i_mode & 0x4000) {
                    fileType = 'd';
                    numValidDirectoryInodes++;
                }
                dprintf(outFD, "%c,", fileType);

                // mode, ownver, group, link count
                dprintf(outFD, "%o,%d,%d,%d,", localInode.i_mode & 0xFFF,
                    localInode.i_uid, localInode.i_gid, localInode.i_links_count);

                // time of last inode change, modification time, time of last access
                char cTimeString[20], mTimeString[20], aTimeString[20];
                formatTime(localInode.i_ctime, cTimeString);
                formatTime(localInode.i_mtime, mTimeString);
                formatTime(localInode.i_atime, aTimeString);
                dprintf(outFD, "%s,%s,%s,", cTimeString, mTimeString,
                    aTimeString);

                // file size, number of blocks
                dprintf(outFD, "%d,%d", localInode.i_size,
                    localInode.i_blocks);

                int j;
                for (j = 0; j < EXT2_N_BLOCKS; j++)
                	dprintf(outFD, ",%d", localInode.i_block[j]);
                dprintf(outFD, "\n");

                // process DIRENTS
                struct ext2_dir_entry directory;
                if (fileType == 'd' || fileType == 'f') {
                    if(fileType == 'd') {
                        for (j = 0; j < EXT2_NDIR_BLOCKS; j++) {
                            if (localInode.i_block[j] == 0) break; // NULL - no more

                            int directoryEntryOffset = localInode.i_block[j] * BLOCKSIZE;
                            int currOffset = 0;

                            while (currOffset < BLOCKSIZE) {
                                pread(imageFD, &directory, sizeof(struct ext2_dir_entry), directoryEntryOffset + currOffset);

                                processDirectory(directory, m + 1, currOffset);
                                currOffset += directory.rec_len;
                            }
                        }
                    }
                    int* pointerArr = malloc(BLOCKSIZE);
                    int* doublePointerArr = malloc(BLOCKSIZE);
                    int* triplePointerArr = malloc(BLOCKSIZE);

                    if(localInode.i_block[12] > 0) {

                        pread(imageFD, pointerArr, BLOCKSIZE, localInode.i_block[12]*BLOCKSIZE);
                        int i;
                        for(i = 0; i < BLOCKSIZE/4; i++) {
                            if(pointerArr[i]==0)
                                break;

                            int directoryEntryOffset = pointerArr[i]*BLOCKSIZE;
                            int currOffset = 0;

                            while (currOffset < BLOCKSIZE) {
                                pread(imageFD, &directory, sizeof(struct ext2_dir_entry), directoryEntryOffset + currOffset);

                                processIndirectory(directory, m + 1, 12 + i, 1, localInode.i_block[12], i + 1);
                                currOffset += directory.rec_len;
                            }
                        }
                    }

                    // DOUBLE INDIRECT
                    if (localInode.i_block[13] > 0) {
                        // read first level pointer block
                        pread(imageFD, pointerArr, BLOCKSIZE, localInode.i_block[13] * BLOCKSIZE);

                        // go through pointers and read in second level blocks
                        int a;
                        for (a = 0; a < BLOCKSIZE / 4; a++) {
                            // read in second level pointer blocks
                            pread(imageFD, doublePointerArr, BLOCKSIZE, pointerArr[a] * BLOCKSIZE);

                            // go through pointers and process directories like before
                            int b;
                            for (b = 0; b < BLOCKSIZE / 4; b++) {
                                if (doublePointerArr[b] == 0) break; // null pointer

                                int directoryEntryOffset = doublePointerArr[b] * BLOCKSIZE; // start offset at this block
                                int currOffset = 0;
                                while (currOffset < BLOCKSIZE) {
                                    pread(imageFD, &directory, sizeof(struct ext2_dir_entry), directoryEntryOffset + currOffset);

                                    // later we'll check if we're passing everything in correctly
                                    processIndirectory(directory, m + 1, 13 + b, 2, localInode.i_block[13], b + 1);

                                    currOffset += directory.rec_len;
                                }
                            }
                        }
                    }

                    //TRIPLE INDIRECT
                    if (localInode.i_block[14] > 0) {
                        // read first level pointer block
                        pread(imageFD, pointerArr, BLOCKSIZE, localInode.i_block[14] * BLOCKSIZE);

                        // go through pointers and read in third level blocks
                        int a;
                        for(a = 0; a < BLOCKSIZE / 4; a++) {
                            //read in second level pointer blocks
                            pread(imageFD, doublePointerArr, BLOCKSIZE, pointerArr[a] * BLOCKSIZE);

                            // go through pointers and read in second level blocks
                            int b;
                            for (b = 0; b < BLOCKSIZE / 4; b++) {
                                // read in third level pointer blocks
                                pread(imageFD, triplePointerArr, BLOCKSIZE, doublePointerArr[b] * BLOCKSIZE);

                                // go through pointers and process directories like before
                                int c;
                                for (c = 0; c < BLOCKSIZE / 4; c++) {
                                    if (triplePointerArr[c] == 0) break; // null pointer

                                    int directoryEntryOffset = triplePointerArr[c] * BLOCKSIZE; // start offset at this block
                                    int currOffset = 0;
                                    while (currOffset < BLOCKSIZE) {
                                        pread(imageFD, &directory, sizeof(struct ext2_dir_entry), directoryEntryOffset + currOffset);

                                        // later we'll check if we're passing everything in correctly
                                        processIndirectory(directory, m + 1, 13 + c, 2, localInode.i_block[13], c + 1);

                                        currOffset += directory.rec_len;
                                    }
                                }
                            }
                        }
                    }
                }
    	    }
        }
    }
}

int main(int argc, char **argv) {
    if(argc != 2) {
		fprintf(stderr, "Error: Wrong argument combination.\n");
		exit(1);
	}
    else {
        int len = strlen(argv[1]) + 1;
		fileName = malloc(len);
		fileName = argv[1];
	}

    imageFD = open(fileName, O_RDONLY);
    if (imageFD == -1) {
        fprintf(stderr, "error: image file does not exist.\n");
        exit(1);
    }
    outFD = creat("summary.csv", S_IRWXU);

    superSummary();
    groupSummary();
    blockEntries();
    inodeEntries();
    inodeSummary();

    close(outFD);

    int fd = open("./summary.csv", O_RDONLY);
    char buf[1024];
    int len;
    while((len = read(fd, buf, 1024)) > 0) {
        write(1, buf, len);
    }
    close(fd);
}
