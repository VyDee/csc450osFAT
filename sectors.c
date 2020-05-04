#include <stdio.h>
#include "sectors.h"

int sectorCount(int numByte)
{
    if ((numByte % 512) != 0)
    {
        return (numByte / 512) + 1;
    }
    else
    {
        return numByte / 512;
    }
}

int blockCount (int sector)
{
    if ((sector % 8) != 0)
    {
        return (sector / 8) + 1;
    }
    else
    {
        return sector / 8;
    }
}