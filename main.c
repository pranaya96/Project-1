#include <stdio.h>
#include <stdlib.h>

int main(){
    unsigned long fileLen;
    char *buff;
    FILE *filePtr;
    filePtr = fopen("practice_project_test_file_1", "rb");
    if (filePtr==NULL){
        printf("Error opening file!");
        exit(1);
    }
    
    fseek(filePtr,0, SEEK_END); //Get the length of file
    fileLen=ftell(filePtr);
    printf("the length of the entire file is: %lu\n",fileLen);
    fseek(filePtr, 0, SEEK_SET);



    //Allocate memory
	buff =(char *)malloc(fileLen+1);

	//Read file contents into buffer
	fread(buff, fileLen, 1, filePtr);
	fclose(filePtr);
    int n;
    for(n =0; n< fileLen ; n++ ){
        printf("%u\n",buff[n]);
    }
	//Do what ever with buffer

    //free(buff);
    return 0;

}