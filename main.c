#include <stdio.h>
#include <stdlib.h>


#define nul '\0'
#define TRUE 1

int readTypeZero(char*, int);
int readTypeOne(char*, int);

int main(){
    int fileLen;
    char *buff;
    FILE *filePtr;
    filePtr = fopen("practice_project_test_file_1", "rb");
    if (filePtr==NULL){
        printf("Error opening file!");
        exit(1);
    }
    
    //Get the length of file
    fseek(filePtr,0, SEEK_END); 
    fileLen= ftell(filePtr); 
    printf("the length of the entire file is: %d\n",fileLen);
    fseek(filePtr, 0, SEEK_SET);


    //buffer to read the file content
    char primaryBuffer[fileLen];
	//Read file contents into buffer
    fread(primaryBuffer,1,fileLen,filePtr);
    int pointerPosition;
    pointerPosition = 0;
    while (pointerPosition < fileLen-1){
        if (primaryBuffer[pointerPosition] == 0) {
            printf("\nNums of Type0:\n");
            pointerPosition = readTypeZero(primaryBuffer, pointerPosition);
        }
        if (primaryBuffer[pointerPosition]== 1){
            printf("\nNums of Type1:\n");
            pointerPosition = readTypeOne(primaryBuffer, pointerPosition);
        }
    }

    printf(".....sucessful........");
    fclose(filePtr);
    return 0;
}


int readTypeZero(char* buffer, int currIndex)
{
    int amount; // amount of numbers 
    short tempBuffer[1]; // temporary Buffer to store individual number 
    amount = buffer[currIndex+1]; 
    printf("Amount:%d\t", amount);
    printf("current index:%d\n", currIndex);
    
    for(int i = 0; i < amount; i++){
        short swapped[1];
        memcpy(tempBuffer, (buffer+currIndex+2)+ i*2, 2);
        swapped[0] = (tempBuffer[0]>>8) | (tempBuffer[0]<<8); //change the byte order
        printf("%d\n", swapped[0]);
    }
    printf("\nPointer Position:%d\n", currIndex+2+(amount*2));
    return (currIndex+2+(amount*2));

}


int readTypeOne(char* buffer, int currIndex)
{
    char amount[4]; //array to store amount followed by nul terminator
    memcpy(amount, buffer+currIndex+1, 3); // copy 3 bytes from primary buffer to amount array
    amount[3] = nul ;//last index terminated by nul terminator
    printf("\n%s\n",amount);
    int numberAmount = atoi(amount); //change ascii to integer
    printf("Amount:%d\t", numberAmount);
    printf("------------------");
    char tempBuffer[6]; //5 bytes is needed at max for unsigned integers no more than 65535 plus nul terminator
    int currPosOfPointer = 4; // move pointer after reading the amount
    int i = 0;
    int bufferTracker=0;
    printf("current index:%d\n", currIndex);
    while (i < numberAmount){
        memcpy(tempBuffer+bufferTracker, buffer+currIndex+currPosOfPointer, 1);
        if(tempBuffer[bufferTracker]== 44){
            tempBuffer[bufferTracker] = nul;
            bufferTracker = 0;
            currPosOfPointer ++;
            printf("%d\n", atoi(tempBuffer));
            i++;
            continue;

        }
        else if (tempBuffer[bufferTracker] == 0 || tempBuffer[bufferTracker] ==1){
            tempBuffer[bufferTracker] = nul;
            i++;
            printf("%d\n", atoi(tempBuffer));
            //printf("\nPointer Position:%d\n",  currIndex + currPosOfPointer);
            return currIndex + currPosOfPointer;  
        }
        else{ 
            currPosOfPointer++;
            bufferTracker++;
            continue;
        }
    } 
    return currIndex + currPosOfPointer;//for the last number 

}

