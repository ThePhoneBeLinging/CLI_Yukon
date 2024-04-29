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

char* runCommand(Pile* deck, Pile *coloumns[], Pile *foundations[], STATE* state, COMMAND command)
{
    // response is currently not used
    char* response[50] = {0};
    char* argument = "";
    if (*state == NODECK || *state == STARTUP)
    {
        if (command == LOADDECK)
        {
            loadDeckFromFile(deck,argument, response);
            populateColoumns(state,deck,coloumns);
            *state = STARTUP;
        }
        if (command == EXIT)
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
            clearFoundations(foundations);
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
            clearFoundations(foundations);
            loadDeckFromFile(deck,"temp/temp",response);
            populateColoumns(state,deck,coloumns);
        }
    }
    else
    {
        response[0] = "Invalid Command";
    }
    return *response;
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
void clearFoundations(Pile* foundations[]) {
    for (int i = 0; i < 4; i++) {
        foundations[i]->size = 0;
        foundations[i]->firstCard = NULL;
        foundations[i]->lastCard = NULL;
    }
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

Card* getCardFromString(Pile* coloumn, char cardString[]){
    Card* currentCard = coloumn->firstCard;

    while (currentCard != NULL) {

        if (currentCard->number == getIntFromCardLetter(cardString[0]) && currentCard->suit == cardString[1]) {
            return currentCard;
        }
        currentCard = currentCard->nextCard;
    }
    return NULL;

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

void
printUI (Pile *coloumns[], Pile *foundations[], STATE *state, char * command, char * response[])
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

COMMAND getInputFromTerminal (Pile *deck, Pile **coloumns, Pile **foundations, STATE *state, char *inputStr, char **response)
{

    //The scanf function adds all given characters to the char arr, until a next line is given.
    char command[50] = {0};
    char argument[50]= {0};
    printUI(coloumns,foundations,state,command,response);
    scanf(" %[^\n]s",inputStr);
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

    if (stringsAreEqual(commandToExectute,"LD")) return LOADDECK;
    if (stringsAreEqual(commandToExectute,"SW")) return SHOWDECK;
    if (stringsAreEqual(commandToExectute,"SI")) return SPLITDECK;
    if (stringsAreEqual(commandToExectute,"SR")) return SHUFFLEDECK;
    if (stringsAreEqual(commandToExectute,"SD")) return SAVEDECK;
    if (stringsAreEqual(commandToExectute,"P")) return STARTGAME;
    if (stringsAreEqual(commandToExectute, "Q")) return QUITGAME;
    if (stringsAreEqual(commandToExectute,"QQ")) return EXIT;
        // Extract the source and destination indices
        int sourceIndex = command[1] - '1'; // Subtract '1' to convert from char to int and adjust for 0-indexing
        int destIndex = command[5] - '1'; // Subtract '1' to convert from char to int and adjust for 0-indexing

        // Call the function to move the card if the move is legal
        if (command[0] == 'C' && command[2] == '-' && command[3] == '>' && (command[4] == 'C'||command[4] == 'F')) {
            // Extract the source and destination indices
            int sourceIndex = command[1] - '1'; // Subtract '1' to convert from char to int and adjust for 0-indexing
            int destIndex = command[5] - '1'; // Subtract '1' to convert from char to int and adjust for 0-indexing
            if (command[4] == 'C') {
                Card *cardToMove = getLegalMove(coloumns, sourceIndex, destIndex);
                if (cardToMove == NULL) {
                    response[0] = "Illegal move";
                } else {
                    moveCardBetweenColoumns(coloumns, sourceIndex, destIndex, cardToMove);
                    turnOverLastCard(coloumns[sourceIndex]);
                    *response = TextFormat("Moved %c%c to C%d from C%d", getCharFromCardNumber(cardToMove->number),cardToMove->suit,destIndex+1,sourceIndex+1);
                }
            } else if (command[4] == 'F') {
                if (LegalMoveFoundation(foundations[destIndex], coloumns[sourceIndex]->lastCard)) {
                    moveBottomCardToFoundation(coloumns[sourceIndex], foundations[destIndex]);
                    turnOverLastCard(coloumns[sourceIndex]);
                    *response = TextFormat("Moved %c%c to F%d from C%d", getCharFromCardNumber(foundations[destIndex]->lastCard->number),foundations[destIndex]->lastCard->suit,destIndex+1,sourceIndex+1);

                } else {
                    response[0] = "Illegal move";
                }
            }
        }else if(command[0]=='C'&&command[2]==':'&&command[5]=='-'&&command[6]=='>'&&(command[7]=='C'||command[7]=='F')){
            int sourceIndex = command[1] - '1'; // Subtract '1' to convert from char to int and adjust for 0-indexing
            int destIndex = command[8] - '1'; // Subtract '1' to convert from char to int and adjust for 0-indexing
            char str[2]={command[3],command[4]};
            Card *cardToMove = getCardFromString(coloumns[sourceIndex], &str);
            if(command[7]=='C'){
                if(LegalMove(coloumns, cardToMove, destIndex)){
                    moveCardBetweenColoumns(coloumns, sourceIndex, destIndex, cardToMove);
                    turnOverLastCard(coloumns[sourceIndex]);
                    *response = TextFormat("Moved %c%c to C%d from C%d", getCharFromCardNumber(cardToMove->number),cardToMove->suit,destIndex+1,sourceIndex+1);
                } else {
                    response[0] = "Illegal move";
                }
            } else if (command[7] == 'F') {
                if(LegalMoveFoundation(foundations[destIndex], cardToMove)) {
                    moveBottomCardToFoundation(coloumns[sourceIndex], foundations[destIndex]);
                    turnOverLastCard(coloumns[sourceIndex]);
                    *response = TextFormat("Moved %c%c to F%d from C%d", getCharFromCardNumber(cardToMove->number),cardToMove->suit,destIndex+1,sourceIndex+1);

                } else {
                    response[0] = "Illegal move";
                }
            }
        }

        return MOVEDCARD;

    return INVALID_COMMAND;
}


void moveCardBetweenColoumns(Pile* coloumns[], int sourceIndex, int destIndex, Card* cardToMove) {
    // Check if the source column is empty
    if (coloumns[sourceIndex]->size == 0) {
        printf("Source column is empty. No card to move.\n");
        return;
    }

    // Find the card in the source column
    Card* currentCard = coloumns[sourceIndex]->firstCard;
    Card* prevCard = NULL;
    while (currentCard != cardToMove) {
        prevCard = currentCard;
        currentCard = currentCard->nextCard;
    }

    // If the card is not found in the source column
    if (currentCard == NULL) {
        printf("Card not found in source column.\n");
        return;
    }

    // Add the cards to the destination column
    if (coloumns[destIndex]->size == 0) {
        // If the destination column is empty
        coloumns[destIndex]->firstCard = currentCard;
        coloumns[destIndex]->lastCard = coloumns[sourceIndex]->lastCard;
    } else {
        // If the destination column is not empty
        coloumns[destIndex]->lastCard->nextCard = currentCard;
        coloumns[destIndex]->lastCard = coloumns[sourceIndex]->lastCard;
    }

    // Increase the size of the destination column by the number of moved cards
    Card* tempCard = currentCard;
    while (tempCard != NULL) {
        coloumns[destIndex]->size++;
        coloumns[sourceIndex]->size--;
        tempCard = tempCard->nextCard;
    }
    // If the card is not the last card in the source column
    if (prevCard != NULL) {
        prevCard->nextCard = NULL;
        coloumns[sourceIndex]->lastCard = prevCard;
    } else {
        coloumns[sourceIndex]->firstCard = NULL;
        coloumns[sourceIndex]->lastCard = NULL;
    }
}

void moveBottomCardToFoundation(Pile* coloumn, Pile* foundation) {
    // Check if the column is empty
    if (coloumn->size == 0) {
        printf("Column is empty. No card to move.\n");
        return;
    }

    // Get the last card from the column
    Card* cardToMove = coloumn->lastCard;

    // Remove the last card from the column
    if (coloumn->size == 1) {
        // If the column has only one card
        coloumn->firstCard = NULL;
        coloumn->lastCard = NULL;
    } else {
        // If the column has more than one card
        Card* currentCard = coloumn->firstCard;
        while (currentCard->nextCard != cardToMove) {
            currentCard = currentCard->nextCard;
        }
        currentCard->nextCard = NULL;
        coloumn->lastCard = currentCard;
    }
    coloumn->size--;

    // Add the card to the foundation pile
    if (foundation->size == 0) {
        // If the foundation pile is empty
        foundation->firstCard = cardToMove;
        foundation->lastCard = cardToMove;
    } else {
        // If the foundation pile is not empty
        foundation->lastCard->nextCard = cardToMove;
        foundation->lastCard = cardToMove;
    }
    foundation->size++;
    cardToMove->nextCard = NULL;

    // If the last card in the source column is facedown, turn it face up
    if (coloumn->lastCard != NULL && !coloumn->lastCard->faceUp) {
        coloumn->lastCard->faceUp = true;
    }
}

void turnOverLastCard (Pile *coloumn)
{
    if (coloumn->lastCard != NULL) coloumn->lastCard->faceUp = true;
}
