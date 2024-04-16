#include <string.h>
#include "Card.h"
#include "Utility.h"
#include <stdio.h>

bool stringsAreEqual(char* firstString, char* secondString)
{
    return strcmp(firstString,secondString) == 0;
}

void handleInput(Pile* deck, Pile* coloumns[], Pile* foundations[], STATE* state)
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

    char* commandToExectute = &command[0];
    if (*state == STARTUP)
    {
        if (stringsAreEqual(commandToExectute,"LD"))
        {
            // TODO LoadFromDeck()
            loadDeckFromFile(deck,argument);
        }
        if (stringsAreEqual(commandToExectute,"SW"))
        {
            // TODO SW

        }
        if (stringsAreEqual(commandToExectute,"SL"))
        {
            // TODO SD

        }
        if (stringsAreEqual(commandToExectute,"SR"))
        {
            // TODO SR

        }
        if (stringsAreEqual(commandToExectute,"SD"))
        {
            // TODO SD

        }
        if (stringsAreEqual(commandToExectute,"QQ"))
        {
            // TODO QQ

        }
        if (stringsAreEqual(commandToExectute,"P"))
        {
            *state = PLAY;
            return;
        }
    }
    else if (*state == PLAY)
    {
        if (stringsAreEqual(commandToExectute,"Q"))
        {
            *state = STARTUP;
            return;
        }
    }
}