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
            populateColoumns(*deck,coloumns);
        }
        if (stringsAreEqual(commandToExectute,"SW"))
        {
            showDeck(coloumns);

        }
        if (stringsAreEqual(commandToExectute,"SL"))
        {
            // TODO SL
            splitDeck(deck,atoi(argument));
            populateColoumns(*deck,coloumns);
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
    for (int i = 0; i < 25; i++)
    {
        bool addedToPrint = false;
        for (int k = 0; k < 7; k++)
        {
            Card* cardToPrint = coloumns[k]->firstCard;
            if (coloumns[k]->size <= i) continue;
            for (int j = 0; j < i; j++)
            {
                if (cardToPrint->nextCard == NULL) break;
                cardToPrint = cardToPrint->nextCard;
            }
            if (cardToPrint->faceUp) printf("%c%c\t", getCharFromCardNumber(cardToPrint->number), cardToPrint->suit);
            else printf("[]\t");
            addedToPrint = true;
        }
        if (i < 4)
        {
            printf("\t");
            if (foundations[i]->lastCard == NULL) printf("[]\t");
            else printf("%c%c", getCharFromCardNumber(foundations[i]->lastCard->number),foundations[i]->lastCard->suit);
            printf("F%d",(i+1));
            addedToPrint = true;
        }

        if (addedToPrint) printf("\n");
    }
}

void populateColoumns (Pile deck, Pile *coloumns[])
{
    Pile* workingDeck = &deck;
    Card* cardToAddToColoumn;
    cardToAddToColoumn = workingDeck->firstCard;
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
