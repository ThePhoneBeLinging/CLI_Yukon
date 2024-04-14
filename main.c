#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Card.h"
#include "main.h"
int main (void)
{
    Pile deck;
    int AMOUNT_OF_COLOUMNS = 7;
    int AMOUNT_OF_FOUNDATIONS = 4;
    Pile* coloumns[AMOUNT_OF_COLOUMNS];
    Pile* foundations[AMOUNT_OF_FOUNDATIONS];
    mallocPileArr(foundations, AMOUNT_OF_FOUNDATIONS);
    mallocPileArr(coloumns,AMOUNT_OF_COLOUMNS);

    printf("Hello, World!\n");
    handleInput(&deck,coloumns,foundations);
    return 0;
}

void handleInput(Pile* deck, Pile* coloumns[], Pile* foundations[])
{
    char inputStr[50];
    printf("Board should be shown and show last message and output \n");
    //The scanf function adds all given characters to the char arr, until a next line is given.
    scanf("%[^\n]s",inputStr);
    char command[50];
    char argument[50];
    int indexOfFirstSpace = 0;
    int workingIndex = 0;
    for (int i = indexOfFirstSpace; i < 50; i++)
    {
        if (inputStr[i] == ' ')
        {
            if (indexOfFirstSpace != 0) break;
            indexOfFirstSpace = i + 1;
            workingIndex = 0;
            continue;
        }
        if (indexOfFirstSpace == 0)
        {
            command[workingIndex] = inputStr[i];
        }
        else
        {
            argument[workingIndex] = inputStr[i];
        }
        workingIndex++;
    }
    //Command is now an array of char*, containing all letters before the first space
    //Argument is now an array of char*, specifying whatever came after the first space in inputStr.
    // Use strcmp to compare strings in future switch


}
