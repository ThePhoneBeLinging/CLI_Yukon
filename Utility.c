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
    //The scanf function adds all given characters to the char arr, until a next line is given.

    char command[50] = {0};
    char argument[50]={0};
    char response[50] = {0};
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
            populateColoumns(state,deck,coloumns);
        }
        if (stringsAreEqual(commandToExectute,"SW"))
        {
            showDeck(coloumns);

        }
        if (stringsAreEqual(commandToExectute,"SI"))
        {
            // TODO SL
            splitDeck(deck,coloumns,atoi(argument));
            populateColoumns(state,deck,coloumns);
        }
        if (stringsAreEqual(commandToExectute,"SR"))
        {
            // TODO SR
            shuffleDeck(deck,coloumns);
            populateColoumns(state,deck,coloumns);

        }
        if (stringsAreEqual(commandToExectute,"SD"))
        {
            // TODO SD
            saveDeckFromColoumnsToFile(coloumns,argument);
            populateColoumns(state,deck,coloumns);
        }
        if (stringsAreEqual(commandToExectute,"QQ"))
        {
            exit(1);

        }
        if (stringsAreEqual(commandToExectute,"P"))
        {
            *state = PLAY;
            saveDeckFromColoumnsToFile(coloumns,"temp/temp");
            populateColoumns(state,deck,coloumns);
        }
    }
    else if (*state == PLAY)
    {
        if (stringsAreEqual(commandToExectute,"Q"))
        {
            *state = STARTUP;
            loadDeckFromFile(deck,"temp/temp");
            populateColoumns(state,deck,coloumns);
        }
    }
    printBoard(coloumns,foundations,state);
    printf("Last Command > %s\n",command);
    printf("Response > %s\n",response);
    printf("Input > ");
    scanf(" %[^\n]s",inputStr);

}

void printBoard(Pile* coloumns[], Pile* foundations[], STATE* state)
{
    printf("\n");
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
            if (cardToPrint == NULL && i <= 7) printf("\t");
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

        if (addedToPrint || i <=7) printf("\n");
    }
}

void populateColoumns (STATE* state, Pile* deck, Pile *coloumns[])
{
    int amountOfCardsInColoumns[7] = {1,6,7,8,9,10,11};
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
    int i = -1;
    while (cardToAddToColoumn != NULL)
    {
        i++;
        int coloumnToInsertTo = i % 7;
        if (*state == PLAY && coloumns[coloumnToInsertTo]->size >= amountOfCardsInColoumns[coloumnToInsertTo])
        {
            continue;
        }
        //printf("Current coloumn: %d Size of coloumn: %d\n",coloumnToInsertTo, coloumns[coloumnToInsertTo]->size);
        coloumns[coloumnToInsertTo]->lastCard->nextCard = cardToAddToColoumn;
        coloumns[coloumnToInsertTo]->lastCard = cardToAddToColoumn;
        coloumns[coloumnToInsertTo]->size += 1;
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
