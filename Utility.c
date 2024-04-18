#include <string.h>
#include "Card.h"
#include "Utility.h"
#include <stdio.h>
#include <stdlib.h>

bool stringsAreEqual(char* firstString, char* secondString)
{
    return strcmp(firstString,secondString) == 0;
}

void handleInput(Pile* deck, Pile* coloumns[], Pile* foundations[], STATE* state)
{
    char inputStr[50];
    printf("Board should be shown and show last message and output \n");
    //The scanf function adds all given characters to the char arr, until a next line is given.
    scanf(" %[^\n]s",inputStr);
    char command[50];
    char argument[50];
    char response[50];
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
            loadDeckFromFile(deck,argument);
            populateColoumns(deck,coloumns);
        }
        if (stringsAreEqual(commandToExectute,"SW"))
        {
            showDeck(coloumns);

        }
        if (stringsAreEqual(commandToExectute,"SI"))
        {
            // TODO SL
            splitDeck(deck,coloumns,atoi(argument));
            populateColoumns(deck,coloumns);
        }
        if (stringsAreEqual(commandToExectute,"SR"))
        {
            // TODO SR

        }
        if (stringsAreEqual(commandToExectute,"SD"))
        {
            // TODO SD
            saveDeckFromColoumnsToFile(coloumns,argument);
            populateColoumns(deck,coloumns);
        }
        if (stringsAreEqual(commandToExectute,"QQ"))
        {
            exit(0);

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
    printBoard(coloumns,foundations,state);
}

void printBoard(Pile* coloumns[], Pile* foundations[], STATE* state)
{
    // Print Coloumn names
    for (int i = 1; i < 8; i++)
    {
        printf("C%d\t",i);
    }
    printf("\n\n");
    int foundationsDrawn = 0;
    for (int i = 0; i < 25; i++)
    {
        bool addedToPrint = false;
        for (int k = 0; k < 7; k++)
        {
            Card* cardToPrint = coloumns[k]->firstCard;
            if (k != 0 && coloumns[k]->size > i)
            {
                printf("\t");
            }
            if (coloumns[k]->size <= i) continue;
            for (int j = 0; j < i; j++)
            {
                if (cardToPrint->nextCard == NULL) break;
                cardToPrint = cardToPrint->nextCard;
            }
            if (cardToPrint->faceUp) printf("%c%c", getCharFromCardNumber(cardToPrint->number), cardToPrint->suit);
            else printf("[]");

            addedToPrint = true;
        }
        if (i % 2 == 0 && foundationsDrawn < 4)
        {
            printf("\t\t");
            if (foundations[foundationsDrawn]->lastCard == NULL) printf("[]");
            else printf("%c%c", getCharFromCardNumber(foundations[foundationsDrawn]->lastCard->number),foundations[foundationsDrawn]->lastCard->suit);
            printf("\tF%d",foundationsDrawn+1);
            addedToPrint = true;
            foundationsDrawn++;
        }

        if (addedToPrint) printf("\n");
    }
}

void populateColoumns (Pile* deck, Pile *coloumns[])
{
    Card* cardToAddToColoumn;
    cardToAddToColoumn = deck->firstCard;
    for (int i = 0; i < 7; i++)
    {
        coloumns[i]->size = 0;
        coloumns[i]->firstCard = cardToAddToColoumn;
        coloumns[i]->lastCard = cardToAddToColoumn;
        coloumns[i]->size += 1;
        cardToAddToColoumn = cardToAddToColoumn->nextCard;
    }
    for (int i = 0; i < 52; i++)
    {
        if (cardToAddToColoumn == NULL) break;
        coloumns[i % 7]->lastCard->nextCard = cardToAddToColoumn;
        coloumns[i%7]->lastCard = cardToAddToColoumn;
        coloumns[i%7]->size += 1;
        cardToAddToColoumn = cardToAddToColoumn->nextCard;
    }
    for (int i = 0; i < 7; i++)
    {
        coloumns[i]->lastCard->nextCard = NULL;
    }
}

Card* linkColoumnsToSingleLinkedList (Pile *coloumns[])
{
    Card* firstCard = coloumns[0]->firstCard;
    for (int k = 0; k < 25; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            if (coloumns[i]->firstCard == NULL) break;
            Card* temp = coloumns[i]->firstCard;
            coloumns[i]->firstCard = coloumns[i]->firstCard->nextCard;
            temp->nextCard = coloumns[(i+1)%7]->firstCard;

        }
    }
    return firstCard;
}
FILE* getFilePointer(char* fileName, char* readMode)
{
    FILE* filePTR = malloc(sizeof (FILE));
    filePTR = fopen(&fileName[0],readMode);
    return filePTR;
}
