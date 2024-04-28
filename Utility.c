#pragma once
#include <string.h>
#include "Card.h"
#include "Utility.h"
#include <stdio.h>
#include <stdlib.h>

bool stringsAreEqual(char* firstString, char* secondString)
{
    return strcmp(firstString,secondString) == 0;
}

void runCommand(Pile* deck, Pile *coloumns[], Pile *foundations[], STATE* state, COMMAND command)
{
    // response is currently not used
    char* response[50];
    char* argument = "";
    if (*state == NODECK || *state == STARTUP)
    {
        if (command == LOADDECK)
        {
            loadDeckFromFile(deck,argument, response);
            populateColoumns(state,deck,coloumns);
            *state = STARTUP;
        }
        if (command == QUITGAME)
        {
            exit(0);
        }
    }
    if (*state == STARTUP)
    {
        if (command == SHOWDECK)
        {
            showDeck(coloumns, response);

        }
        if (command == SPLITDECK)
        {
            splitDeck(deck,coloumns,atoi(argument),response);
            populateColoumns(state,deck,coloumns);
        }
        if (command == SHUFFLEDECK)
        {
            shuffleDeck(deck,coloumns,response);
            populateColoumns(state,deck,coloumns);

        }
        if (command == SAVEDECK)
        {
            saveDeckFromColoumnsToFile(coloumns,argument,response);
            populateColoumns(state,deck,coloumns);
        }
        if (command == STARTGAME)
        {
            *state = PLAY;
            saveDeckFromColoumnsToFile(coloumns,"temp/temp",NULL);
            populateColoumns(state,deck,coloumns);
            response[0] = "Let the games begin...";
        }

    }
    else if (*state == PLAY)
    {
        if (command == QUITGAME)
        {
            *state = STARTUP;
            loadDeckFromFile(deck,"temp/temp",response);
            populateColoumns(state,deck,coloumns);
        }
    }
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
    for (int i = 0; i < 52; i++)
    {
        bool addedToPrint = false;
        for (int k = 0; k < 7; k++)
        {
            Card* cardToPrint = coloumns[k]->firstCard;
            if (k != 0)
            {
                printf("\t");
            }
            //if (cardToPrint == NULL && i <= 7) printf("\t");
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

        if (addedToPrint || i <=8) printf("\n");
    }
    printf("\n");
}

void populateColoumns (STATE* state, Pile* deck, Pile *coloumns[])
{
    int amountOfCardsInColoumns[7] = {1,6,7,8,9,10,11};
    // one is subtracted from the facedownCard array, as we manually insert the first card into each array.
    int faceDownCardsInColoumn[7] = {0,0,1,2,3,4,5};
    Card* cardToAddToColoumn;
    cardToAddToColoumn = deck->firstCard;
    for (int i = 0; i < 7; i++)
    {
        coloumns[i]->size = 0;
        coloumns[i]->firstCard = cardToAddToColoumn;
        coloumns[i]->lastCard = cardToAddToColoumn;
        coloumns[i]->size += 1;
        if (*state == PLAY)
        {
            if (i == 0) cardToAddToColoumn->faceUp = true;
            else cardToAddToColoumn->faceUp = false;
        }
        cardToAddToColoumn = cardToAddToColoumn->nextCard;
    }
    int i = -1;
    while (cardToAddToColoumn != NULL)
    {
        i++;
        int coloumnToInsertTo = i % 7;

        if (*state == PLAY)
        {
            if (coloumns[coloumnToInsertTo]->size >= amountOfCardsInColoumns[coloumnToInsertTo])
            {
                continue;
            }
            if (faceDownCardsInColoumn[coloumnToInsertTo] != 0)
            {
                faceDownCardsInColoumn[coloumnToInsertTo] --;
                cardToAddToColoumn->faceUp = false;
            }
            else cardToAddToColoumn->faceUp = true;
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

void printUI (Pile *coloumns[], Pile *foundations[], STATE *state, char * command, char * response[])
{
    printBoard(coloumns,foundations,state);
    printf("Last Command > %s\n",command);
    printf("Response > ");
    if (*response != NULL) printf("%s",*response);
    printf("\n");
    printf("Input > ");
}

bool hasDeckBeenLoaded (Pile **coloumns)
{
    for (int i = 0; i < 7; i ++)
    {
        if (coloumns[i]->size == 0) return false;
    }
    return true;
}

COMMAND getInputFromTerminal()
{
    //TODO insert old handleinput function here
}

