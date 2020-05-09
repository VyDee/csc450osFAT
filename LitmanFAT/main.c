#include <stdio.h>
#include <stdlib.h>

struct file{
    int startingBlockInFAT;
    int sizeBytes;
};

struct fat_entry{
    int busy;
    int next;
};

struct file_allocation_table{
    int numberOfBlocks;
    struct fat_entry* file_blocks;
};

struct sector{
    int byteCount;
    char bytes[512];
};

struct block{
    int usedSectors;
    struct sector* sectors; //This is the base address of the first sector
};

struct hdd{
    int freeSectors;
    int totalSectors;
    int usedSectors;
    struct sector* sectors; //array of sectors because we don't know
                            // how many sectors are there
};
struct formatted_hdd{
    int numberOfBlocks;

    //base address of our collection of blocks that abstract
    //an existing collection of sectors
    struct block* blocks ;
    struct file* files_dir;
    struct file_allocation_table FAT;
};

//constructor for a fat_entry
struct fat_entry genfat_entry()
{
    struct fat_entry temp;
    temp.busy = 0;
    temp.next = -1;
    return temp;
}

struct file_allocation_table genFAT(struct formatted_hdd myHDD)
{
    struct file_allocation_table temp;
    temp.numberOfBlocks = myHDD.numberOfBlocks;
    temp.file_blocks = malloc(temp.numberOfBlocks*sizeof(struct fat_entry));

    for(int i = 0; i<temp.numberOfBlocks; i++)
    {
        temp.file_blocks[i]=genfat_entry();
    }

    return temp;
    
}

//constructor for a sector
struct sector genSector()
{
    struct sector temp;
    temp.byteCount = 0;
    return temp;
}
//constructor for blocks
struct block genBlock(struct sector* baseAddressOfSectorGroupOf8)
{
    struct block temp;
    temp.usedSectors = 0;
    temp.sectors = baseAddressOfSectorGroupOf8;
    return temp;

}
//constructor for hdd
struct hdd genHDD(int bytes)
{
    struct hdd temp;
    temp.totalSectors = (bytes/512);
    temp.usedSectors = 0;
    temp.freeSectors = temp.totalSectors - temp.usedSectors;
    temp.sectors = malloc(temp.totalSectors*sizeof(struct sector));

    //fill the HDD with sectors
    for(int i = 0; i <temp.totalSectors; i++)
    {
        temp.sectors[i] = genSector();
    }
    return temp;
}

struct file genFile()
{
    struct file temp;
    temp.sizeBytes = -1;
    temp.startingBlockInFAT = -1;
    return temp;
}

struct formatted_hdd formatHDD( struct hdd myHDD)
{
    struct formatted_hdd temp;
    temp.numberOfBlocks = myHDD.totalSectors/8;
    temp.blocks = malloc(temp.numberOfBlocks*sizeof(struct block));
    temp.files_dir = malloc(temp.numberOfBlocks*sizeof(struct file));
    temp.FAT = genFAT(temp);
    int postOfNextBaseSector = 0;
    for (int i = 0; i <temp.numberOfBlocks; i++)
    {
        temp.blocks[i]= genBlock(&myHDD.sectors[postOfNextBaseSector]);
        postOfNextBaseSector += 8;
    }
    for(int j = 0; j < temp.numberOfBlocks; j++)
    {
        temp.files_dir[j] = genFile();
    }
    return temp;
}

struct file getFile()
{
    printf("Enter the size of the files?");
    struct file theFile;
    scanf("%d",&theFile.sizeBytes);
    theFile.startingBlockInFAT = -1;
    return theFile;
}

int convertBytestoSectors (int numBytesToConvert)
{
    int temp = numBytesToConvert % 512;
    if (temp == 0)
    {
        return numBytesToConvert/512;
    }
    else
    {
        return (numBytesToConvert/512) + 1;
    }
}

int convertBytestoBlocks (int numBytesToConvert)
{
    int temp = (convertBytestoSectors(numBytesToConvert))%8;
    if (temp == 0)
    {
        return (convertBytestoSectors(numBytesToConvert))/8;
    }
    else
    {
        return (convertBytestoSectors(numBytesToConvert))/8 + 1;
    }
}

int checkAvailableFatEntry (struct formatted_hdd myHDD)
{
    int r = rand() % (myHDD.numberOfBlocks);

    if(myHDD.FAT.file_blocks[r].busy == 0)
    {
        return r;
    }
    else
    {
        checkAvailableFatEntry(myHDD);
    }
    
    // for(int i = 0; i <myHDD.numberOfBlocks; i++)
    // {
    //     // printf("***********************************************************************************\n");
    //     // printf("I am check block %d\n",i);
    //     // printf("This is block %d busy or not value -----> %d\n", i, myHDD.FAT.file_blocks[i].busy);
    //     // printf("***********************************************************************************\n");
    //     if(myHDD.FAT.file_blocks[i].busy == 0)
    //     {
    //         return i;
    //     }
    // }
    // return -1;
}

void FATLookUp (int startingBlock, struct formatted_hdd myHDD)
{
    printf("%d->",startingBlock);
    struct file_allocation_table myFAT = myHDD.FAT;
    int nextNode = myFAT.file_blocks[startingBlock].next;
    if (nextNode != (-1))
    {
        FATLookUp(nextNode,myHDD);
    }
    else
    {
        printf("-1\n");
    }
}

void storeFileIn_FAT_DirectoryTable_HDD(struct file myFile, struct formatted_hdd myHDD, struct hdd* orginalHDD)
{
    printf("---------------------------------------------------------------------\n");
    printf("|\n");
    int fileBlock = convertBytestoBlocks(myFile.sizeBytes);
    printf("The total fileBlock is ---------------> %d \n", fileBlock);
    int previousPos = 0;
    int j = 0;

    //store the file into the allocation table
    while(j < fileBlock)
    {
        int i = checkAvailableFatEntry(myHDD);
        // printf("the j int right now: %d\n", j);
        // printf("The next available block is %d \n", i);
        if(j==0)
        {
            myFile.startingBlockInFAT = i;
            printf("the starting block of the file on FAT is: %d\n",i);
        }
        else if (j != (fileBlock -1))
        {
            myHDD.FAT.file_blocks[previousPos].next = i;
            
        }
        else
        {
            myHDD.FAT.file_blocks[previousPos].next = i;
            myHDD.FAT.file_blocks[i].next = -1;
            printf("the ending block of the file on FAT is: %d \n\n", i);
        }
        previousPos =i;
        myHDD.FAT.file_blocks[i].busy = 1;
        j++; 
    }

    printf("FAT order is \n");
    FATLookUp(myFile.startingBlockInFAT,myHDD);
    //store file into the directory table
    int i = 0;
    while (myHDD.files_dir[i].sizeBytes != (-1))
    {
        i++;
    }
    myHDD.files_dir[i] = myFile;

    orginalHDD -> usedSectors = convertBytestoSectors(myFile.sizeBytes);
    orginalHDD -> freeSectors = orginalHDD->totalSectors-orginalHDD->usedSectors;
    printf("|\n");
    printf("---------------------------------------------------------------------\n");
}

int main (int argc, char** argv)
{
    struct hdd theHDD = genHDD(1*1024*1024*1024);
    struct formatted_hdd theFormattedHHD = formatHDD(theHDD); 
    // modifyHDD(&theHDD);
    struct file theFile = getFile();
    struct file theFile2 = getFile();
    storeFileIn_FAT_DirectoryTable_HDD(theFile,theFormattedHHD,&theHDD);
    printf("First run");
    printf("The file 1 starting is %d\n",theFormattedHHD.files_dir[0].startingBlockInFAT);
    printf("The file 1 has %d bytes\n",theFormattedHHD.files_dir[0].sizeBytes);
    printf("The HDD total size is %d\n",theHDD.totalSectors);
    printf("The HDD used size is %d\n",theHDD.usedSectors);
    printf("The HDD free size is %d\n",theHDD.freeSectors);
    // printf("_______________________________________________\n");
    // printf("_______________________________________________\n");

    storeFileIn_FAT_DirectoryTable_HDD(theFile2,theFormattedHHD,&theHDD);
    printf("Second run");
    printf("The file 2 starting block is %d\n",theFormattedHHD.files_dir[1].startingBlockInFAT);
    printf("The file 2 has %d bytes\n",theFormattedHHD.files_dir[1].sizeBytes);
    printf("The HDD total size is %d\n",theHDD.totalSectors);
    printf("The HDD used size is %d\n",theHDD.usedSectors);
    printf("The HDD free size is %d\n",theHDD.freeSectors);


    return 0;
}