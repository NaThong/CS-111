# NAME: Jeffrey Chan, Jiwan Kang
# EMAIL: jeffschan97@gmail.com, jiwan226@gmail.com
# ID: 004-611-638, 704-624-623

import sys

def printDuplicateLines(blockNumber, inodeNumber, layer):
    offset = 0
    if layer == 'INDIRECT BLOCK':
        offset = 12
    if layer == 'DOUBLE INDIRECT BLOCK':
        offset = 12 + 256
    if layer == 'TRIPPLE INDIRECT BLOCK':
        offset = 12 + 256 + (256 * 256)

    sys.stdout.write('DUPLICATE ' + layer + ' ' + str(blockNumber) + ' IN INODE ' + str(inodeNumber) + ' AT OFFSET ' + str(offset) + '\n')

def handleBlock(blockNumber, inodeNumber, layer, offset, startNumber, endNumber):
    # handle invalid block
    if blockNumber < 0 or blockNumber > endNumber - 1:
        sys.stdout.write('INVALID ' + layer + ' ' + str(blockNumber) + ' IN INODE ' + str(inodeNumber) + ' AT OFFSET ' + str(offset) + '\n')
        return False
    # handle reserved block
    elif blockNumber > 0 and blockNumber < startNumber:
        sys.stdout.write('RESERVED ' + layer + ' ' + str(blockNumber) + ' IN INODE ' + str(inodeNumber) + ' AT OFFSET ' + str(offset) + '\n')
        return False

    return True

def blockAudit(file):
    blockMap = {} # map to hold block information

    for line in file:
        splitLine = line.split(',')

        # superblock info
        if splitLine[0] == 'SUPERBLOCK':
            inodeSize = int(splitLine[4])
            blockSize = int(splitLine[3])

        # group info
        if splitLine[0] == 'GROUP':
            numBlocks = int(splitLine[2])
            numInodes = int(splitLine[3])
            inodeTableOffset = int(splitLine[8].split('\n')[0])
            firstNonReservedBlockNum = inodeTableOffset + (inodeSize * numInodes / blockSize)

        # map blocks on the free list into blockMap
        if splitLine[0] == 'BFREE':
            blockNumber = int(splitLine[1].split('\n')[0])
            blockMap[blockNumber] = 'free'

        # handle inode lines by iterating through its block pointers
        if splitLine[0] == 'INODE':
            inodeNumber = splitLine[1]
            for num in range(12, 27):
                blockNumber = int(splitLine[num].split('\n')[0])
                if blockNumber != 0 and num < 24 and  blockNumber not in blockMap:
                    if handleBlock(blockNumber, inodeNumber, 'BLOCK', 0, firstNonReservedBlockNum, numBlocks):
                        blockMap[blockNumber] = (inodeNumber, 'BLOCK')
                elif blockNumber != 0 and num == 24 and blockNumber not in blockMap:
                    if handleBlock(blockNumber, inodeNumber, 'INDIRECT BLOCK', 12, firstNonReservedBlockNum, numBlocks):
                        blockMap[blockNumber] = (inodeNumber, 'INDIRECT BLOCK')
                elif blockNumber != 0 and num == 25 and blockNumber not in blockMap:
                    if handleBlock(blockNumber, inodeNumber, 'DOUBLE INDIRECT BLOCK', 268, firstNonReservedBlockNum, numBlocks):
                        blockMap[blockNumber] = (inodeNumber, 'DOUBLE INDIRECT BLOCK')
                elif blockNumber != 0 and num == 26 and blockNumber not in blockMap:
                    if handleBlock(blockNumber, inodeNumber, 'TRIPPLE INDIRECT BLOCK', 65804, firstNonReservedBlockNum, numBlocks):
                        blockMap[blockNumber] = (inodeNumber, 'TRIPPLE INDIRECT BLOCK')
                elif blockNumber != 0 and blockMap[blockNumber] == 'free':
                    sys.stdout.write('ALLOCATED BLOCK ' + str(blockNumber) + ' ON FREELIST\n')
                elif blockNumber != 0 and blockMap[blockNumber] != 'free':
                    layer = 'BLOCK'
                    if num == 24: layer = 'INDIRECT BLOCK';
                    if num == 25: layer = 'DOUBLE INDIRECT BLOCK';
                    if num == 26: layer = 'TRIPPLE INDIRECT BLOCK';
                    printDuplicateLines(blockNumber, inodeNumber, layer)
                    printDuplicateLines(blockNumber, blockMap[blockNumber][0], blockMap[blockNumber][1])

        # handle indirect lines
        if splitLine[0] == 'INDIRECT':
            blockNumber = int(splitLine[5].split('\n')[0])
            layerNumber = int(splitLine[2])
            inodeNumber = int(splitLine[1])
            offset = int(splitLine[3])
            layer = 'BLOCK'
            if layerNumber == 1: layer = 'INDIRECT BLOCK';
            if layerNumber == 2: layer = 'DOUBLE INDIRECT BLOCK';
            if layerNumber == 3: layer = 'TRIPPLE INDIRECT BLOCK';
            # handle allocated blocks that are also on the free list
            if blockNumber in blockMap:
                if blockMap[blockNumber] == 'free':
                    sys.stdout.write('ALLOCATED BLOCK ' + str(blockNumber) + ' ON FREELIST\n')
                    continue
            # handle duplicate blocks
            if blockNumber in blockMap:
                if blockMap[blockNumber] != 'free':
                    printDuplicateLines(blockNumber, inodeNumber, layer)
                    printDuplicateLines(blockNumber, blockMap[blockNumber][0], blockMap[blockNumber][1])
                    continue
            if (handleBlock(blockNumber, inodeNumber, layer, offset, firstNonReservedBlockNum, numBlocks)):
                blockMap[blockNumber] = (inodeNumber, layer)

    # check for unreferenced blocks (not on free list or inodes)
    for num in range(firstNonReservedBlockNum, numBlocks):
        if num not in blockMap:
            sys.stdout.write('UNREFERENCED BLOCK ' + str(num) + '\n')

    file.seek(0) # reset to beginning of file when done

def inodeAudit(file):
    inodeMap = {} # map to hold all inodes

    # go through all lines and map to free or allocated, while checking for error
    for line in file:
        splitLine = line.split(',')
        inodeNumber = int(splitLine[1])
        if splitLine[0] == 'SUPERBLOCK':
            numInodes = int(splitLine[2])
            firstNonReservedInode = int(splitLine[7].split('\n')[0])
        elif splitLine[0] == 'IFREE':
            inodeMap[inodeNumber] = 'free'
        elif splitLine[0] == 'INODE' and inodeNumber not in inodeMap:
            inodeMap[inodeNumber] = 'allocated'
        elif splitLine[0] == 'INODE':
            sys.stdout.write('ALLOCATED INODE ' + str(inodeNumber) + ' ON FREELIST\n')
            inodeMap[inodeNumber] = 'allocated'

    # check to see if any inodes not on free list or allocated
    for num in range(firstNonReservedInode, numInodes + 1):
        if num not in inodeMap:
            sys.stdout.write('UNALLOCATED INODE ' + str(num) + ' NOT ON FREELIST\n')


    file.seek(0) # reset to beginning of file when done
    return inodeMap

def directoryAudit(file, inodeMap):
    inodeLinkMap = {} # maps inodes to how many directories should refer to it
    inodeRunningTotal = {} # maps inodes to how many directories actually refer to it
    childToParentMap = {} # maps directories to parent directories
    freeInodes = inodeMap
    nonFreeInodes = []
    for key in freeInodes:
        if freeInodes[key] != 'free':
            nonFreeInodes.append(key)
    for key in nonFreeInodes:
        del freeInodes[key]

    # go through INODE lines and set them up in the maps
    for line in file:
        splitLine = line.split(',')
        if splitLine[0] == 'INODE':
            inodeNumber = int(splitLine[1])
            linkCount = int(splitLine[6])
            inodeLinkMap[inodeNumber] = linkCount
            inodeRunningTotal[inodeNumber] = 0

    file.seek(0) # start at beginning of file to look for DIRENTS

    # handle DIRENT lines
    for line in file:
        splitLine = line.split(',')
        if splitLine[0] == 'DIRENT':
            referencedInode = int(splitLine[3])
            parentInodeNumber = int(splitLine[1])
            directoryName = splitLine[6].split('\n')[0]
            # DIRENT refers to unallocated inode
            if referencedInode in freeInodes:
                sys.stdout.write('DIRECTORY INODE ' + str(parentInodeNumber)
                    + ' NAME ' + directoryName + ' UNALLOCATED INODE '
                    + str(referencedInode) + '\n')
            # DIRENT refers to invalid inode cause not in free list or inodeLinkMap
            elif referencedInode not in freeInodes and referencedInode not in inodeLinkMap:
                sys.stdout.write('DIRECTORY INODE ' + str(parentInodeNumber)
                    + ' NAME ' + directoryName + ' INVALID INODE '
                    + str(referencedInode) + '\n')
            # increment running total in inodeRunningTotal
            # take note of parent directory information along the way
            else:
                inodeRunningTotal[referencedInode] += 1
                if referencedInode not in childToParentMap:
                    childToParentMap[referencedInode] = parentInodeNumber

        # makes sure '.' and '..' refer to the correct directories
        if splitLine[0] == 'DIRENT':
            referencedInode = int(splitLine[3])
            parentInodeNumber = int(splitLine[1])
            directoryName = splitLine[6].split('\n')[0]
            if (directoryName == "'.'" and (referencedInode != parentInodeNumber)):
                sys.stdout.write('DIRECTORY INODE ' + str(parentInodeNumber)
                    + ' NAME ' + directoryName + ' LINK TO INODE ' + str(referencedInode)
                    + ' SHOULD BE ' + str(parentInodeNumber) + '\n')
            elif (directoryName == "'..'"):
                grandparentNumber = childToParentMap[parentInodeNumber]
                if(referencedInode != grandparentNumber):
                    sys.stdout.write('DIRECTORY INODE ' + str(parentInodeNumber)
                    + ' NAME ' + directoryName + ' LINK TO INODE ' + str(referencedInode)
                    + ' SHOULD BE ' + str(grandparentNumber) + '\n')

    # go through and check if link counts are consistent
    for inodeNumber in inodeLinkMap:
        if inodeRunningTotal[inodeNumber] != inodeLinkMap[inodeNumber]:
            sys.stdout.write('INODE ' + str(inodeNumber) + ' HAS '
                + str(inodeRunningTotal[inodeNumber]) + ' LINKS BUT LINKCOUNT IS '
                + str(inodeLinkMap[inodeNumber]) + '\n')

    file.seek(0) # reset to beginning of file when done

def runAudits(summary):
    # tries to open file, then runs block, inode, and directory audits on it
    try:
        file = open(summary)
        blockAudit(file)
        inodeMap = inodeAudit(file)
        directoryAudit(file, inodeMap)
        file.close()

    except IOError as e:
        sys.stderr.write('error: unable to open file\n')
        exit(1)

def main():
    # requires at least one argument
    if len(sys.argv) == 1:
        sys.stderr.write('error: not enough arguments\n')
        exit()

    # parse through arguments and run audits
    for argument in sys.argv:
        if argument == 'lab3b.py': continue; # dont process python script lol
        runAudits(argument)

if __name__ == '__main__':
    main()
