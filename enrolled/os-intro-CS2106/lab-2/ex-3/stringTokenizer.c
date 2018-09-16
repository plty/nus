#include <stdio.h>
#include <stdlib.h>
#include <string.h>     //for string comparison etc

char** readTokens( int maxTokenNum, int maxTokenSize, int* readTokenNum )
//Assumptions:
//  - the input line is < 120 characters
//  - the tokens are separated by " " and ended by newline
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - should use the freeTokenArray to free memory after use!
{
    char input[120];
    char** tokenStrArr;
    char* tStart;   //start of token
    int i;      

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum );

    //Nullify all entries
    for (i = 0; i < maxTokenNum; i++){
        tokenStrArr[i] = NULL;
    }

    fgets(input, 120, stdin);

    tStart = strtok(input, " \n"); 

    i = 0;
    while (i < maxTokenNum && tStart){
        //Allocate space for token string
        tokenStrArr[i] = (char*) malloc(sizeof(char) * maxTokenSize );

        //Ensure at most 19 + null characters are copied
        strncpy(tokenStrArr[i], tStart, maxTokenSize);    

        //Add NULL terminator in the worst case
        tokenStrArr[i][maxTokenSize-1] = '\0';

        i++;
        tStart = strtok(NULL, " \n"); 
    }

    //return number of token read
    *readTokenNum = i;

    return tokenStrArr;
}

void freeTokenArray(char** strArr, int size)
{
    int i;

    //Free each string first
    for (i = 0; i < size; i++){
        if (strArr[i])
            free( strArr[i] );
        strArr[i] = NULL;
    }

    //Free the entire array
    free( strArr );

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}

int main()
{
    char **argsArr;
    int i, nToken;


    //Example usage
    printf("Example: reads at most 5 tokens of at most 19 characters each\n");

    argsArr = readTokens(5, 20, &nToken);
    printf("%d Tokens read:\n", nToken);

    for (i = 0; i < nToken; i++){
        printf("Arg %d \"%s\"\n", i, argsArr[i]);
    }
    
    //Free the memory space
    freeTokenArray(argsArr, 5);
    argsArr = NULL;     //Just a good habit

    return 0;

}
