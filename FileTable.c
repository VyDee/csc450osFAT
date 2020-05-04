#include <stdio.h>
#include "FileTable.h"
#include <string.h>

int row;
int column;

void initializeArray(int arr[row][column])
{
    for (int i = 0; i < row; i++)
    {
        for(int j = 0; j < column; j ++)
        {
            if (j == 0)
            {
                arr[i][j] = i;
            }
            else
            {
                arr[i][j] = 0;
            } 
        }
    }
}

void printFileTable(int arr[row][column])
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j ++)
        {
            printf("%d  | ", arr[i][j]);
            if (j == (column -1))
            {
                printf("\n");
            }
        }
    }
}

int returnEmptyBlock(int i, int arr[row][column])
{
    
    while(i < (row - 1))
    {
        int checkBlock = arr[i][1];
        if(checkBlock == 1)
        {
            i++;
        }
        else
        {
            return i;
        }
        
    }    
    return 1;
}

void getAllBlock (int startBlock, int arr[row][column])
{
    if((arr[startBlock][2]) != (-1))
    {
        printf("%d -> ", arr[startBlock][2]);
        getAllBlock(arr[startBlock][2], arr);
    }
    else
    {
        printf("-1 \n");
    }
}
void printAllBlock (int startBlock, int arr[row][column])
{
    printf("The block order is %d -> ", startBlock);
    getAllBlock(startBlock, arr);
}

void storeData (int* startBlock, int numBlock, int arr[row][column]) // block = 1
{
    int i = 1; 
    int temp; // temp = 
    i = returnEmptyBlock(i,arr); //i = 1 
    *startBlock = i;    
    arr[i][1] = 1; //ar[1][1] = 1
    temp = i; //temp = 1
    numBlock--; // 1

    while(numBlock >= 0)
    {
        if (numBlock == 0)
        {
            arr[temp][2]= -1;
            numBlock --;
        }
        else
        {
            i = returnEmptyBlock(i,arr); // i = 2
            arr[i][1] = 1; //2 1 = 
            arr[temp][2] = i;  // arr[1][2] = 2
            temp = i;        
            numBlock --;
            i++;
        }    
    }
    
}