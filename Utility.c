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

char* runCommand(Pile* deck, Pile *coloumns[], Pile *foundations[], STATE* state, COMMAND command, char argument[])
{
    // response is currently not used
    char* response[50] = {0};
    if (*state == NODECK || *state == STARTUP)
    {
        if (command == LOADDECK)
        {
            loadDeckFromFile(deck,argument, response);
            if (stringsAreEqual(*response,"File not found")) return response[0];
            if(isDeckValid(*deck,response))
            {
                populateColoumns(state, deck, coloumns);
                *state = STARTUP;
            }
            else
            {
                return *response;
            }
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
            response[0] = "OK";
        }

    }
    else if (*state == PLAY)
    {
        if (command == QUITGAME)
        {
            *state = STARTUP;
            clearFoundations(foundations);
            coloumns[7]->size = 0;
            coloumns[7]->firstCard = NULL;
            coloumns[7]->lastCard = NULL;
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
    printf("LAST Command: %s\n",command);
    printf("Message: ");
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

COMMAND getInputFromTerminal (Pile *deck, Pile **coloumns, Pile **foundations, STATE *state, char *inputStr, char **response,char argument[])
{

    //The scanf function adds all given characters to the char arr, until a next line is given.
    char command[50] = {0};
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

    if (stringsAreEqual(commandToExectute,"LD")) {return LOADDECK;}
    if (stringsAreEqual(commandToExectute,"SW")) return SHOWDECK;
    if (stringsAreEqual(commandToExectute,"SI")) return SPLITDECK;
    if (stringsAreEqual(commandToExectute,"SR")) return SHUFFLEDECK;
    if (stringsAreEqual(commandToExectute,"SD")) return SAVEDECK;
    if (stringsAreEqual(commandToExectute,"P")) return STARTGAME;
    if (stringsAreEqual(commandToExectute, "Q")) return QUITGAME;
    if (stringsAreEqual(commandToExectute,"QQ")) return EXIT;
        // Extract the source and destination indices
        int sourceIndex;
        int destIndex;

        // Call the function to move the card if the move is legal
        if ((command[0] == 'C' ||command[0]=='F')&& command[2] == '-' && command[3] == '>' && (command[4] == 'C'||command[4] == 'F')) {
            // Extract the source and destination indices
            sourceIndex = command[1] - '1'; // Subtract '1' to convert from char to int and adjust for 0-indexing
            destIndex = command[5] - '1'; // Subtract '1' to convert from char to int and adjust for 0-indexing

            if (command[4] == 'C') {
                if(command[0]=='C') {
                    Card *cardToMove = getLegalMove(coloumns, sourceIndex, destIndex);
                    if (cardToMove == NULL) {
                        response[0] = "Illegal move";
                    } else {
                        moveCardBetweenColoumns(coloumns, sourceIndex, destIndex, cardToMove);
                        turnOverLastCard(coloumns[sourceIndex]);
                        *response = "OK";                    }
                }else{
                    Card *cardToMove = foundations[sourceIndex]->lastCard;
                    if(cardToMove==NULL){
                        response[0] = "Illegal move";
                }else if(LegalMove(coloumns, cardToMove, destIndex)){
                    moveCardFoundation(coloumns[destIndex], foundations[sourceIndex], false);
                    turnOverLastCard(coloumns[destIndex]);
                    *response = "OK";
                }
                }
            } else if (command[4] == 'F') {
                if (LegalMoveFoundation(foundations[destIndex], coloumns[sourceIndex]->lastCard)) {
                    moveCardFoundation(coloumns[sourceIndex], foundations[destIndex], true);
                    turnOverLastCard(coloumns[sourceIndex]);
                    *response = "OK";
                } else {
                    response[0] = "Illegal move";
                }
            }
        }else if(command[0]=='C'&&command[2]==':'&&command[5]=='-'&&command[6]=='>'&&(command[7]=='C'||command[7]=='F')){
            sourceIndex = command[1] - '1'; // Subtract '1' to convert from char to int and adjust for 0-indexing
            destIndex = command[8] - '1'; // Subtract '1' to convert from char to int and adjust for 0-indexing
            char str[2]={command[3],command[4]};
            Card *cardToMove = getCardFromString(coloumns[sourceIndex], &str);
            if(command[7]=='C'){
                if(LegalMove(coloumns, cardToMove, destIndex)){
                    moveCardBetweenColoumns(coloumns, sourceIndex, destIndex, cardToMove);
                    turnOverLastCard(coloumns[sourceIndex]);
                    *response = "OK";                } else {
                    response[0] = "Illegal move";
                }
            } else if (command[7] == 'F') {
                if(LegalMoveFoundation(foundations[destIndex], cardToMove)) {
                    moveCardFoundation(coloumns[sourceIndex], foundations[destIndex], true);
                    turnOverLastCard(coloumns[sourceIndex]);
                    *response = "OK";
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

void moveCardFoundation(Pile* coloumn, Pile* foundation, bool toFoundation) {
    if(toFoundation){
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
    } else {
        // Check if the foundation is empty
        if (foundation->size == 0) {
            printf("Foundation is empty. No card to move.\n");
            return;
        }

        // Get the last card from the foundation
        Card* cardToMove = foundation->lastCard;

        // Remove the last card from the foundation
        if (foundation->size == 1) {
            // If the foundation has only one card
            foundation->firstCard = NULL;
            foundation->lastCard = NULL;
        } else {
            // If the foundation has more than one card
            Card* currentCard = foundation->firstCard;
            while (currentCard->nextCard != cardToMove) {
                currentCard = currentCard->nextCard;
            }
            currentCard->nextCard = NULL;
            foundation->lastCard = currentCard;
        }
        foundation->size--;

        // Add the card to the column pile
        if (coloumn->size == 0) {
            // If the column pile is empty
            coloumn->firstCard = cardToMove;
            coloumn->lastCard = cardToMove;
        } else {
            // If the column pile is not empty
            coloumn->lastCard->nextCard = cardToMove;
            coloumn->lastCard = cardToMove;
        }
        coloumn->size++;
        cardToMove->nextCard = NULL;

        // If the last card in the source foundation is facedown, turn it face up
        if (foundation->lastCard != NULL && !foundation->lastCard->faceUp) {
            foundation->lastCard->faceUp = true;
        }
    }
}

void turnOverLastCard (Pile *coloumn)
{
    if (coloumn->lastCard != NULL) coloumn->lastCard->faceUp = true;
}

void moveTerminalOneLineUp (char terminalText[26][50], int* positionOfLine)
{
    for (int i = 1; i < 26; i++)
    {
        if (i == 25)
        {
            for (int k = 0; k < 50;  k++)
            {
                if (terminalText[i][k] == '|') terminalText[i][k] = '\0';
            }
        }

        strcpy(terminalText[(i-1)],terminalText[i]);
    }
    for (int i = 0; i < 50; i++)
    {
        terminalText[25][i] = '\0';
    }
    *positionOfLine = 0;
}
