#include <stdio.h>
#pragma once
//
// Created by Elias Aggergaard Larsen on 11/04/2024.
//
typedef struct Card Card;
struct Card{
    Card* nextCard;
    int number;
    char suit;
    bool faceUp;

};

typedef struct{
    Card* firstCard;
    Card* lastCard;
    int size;
} Pile;

void mallocPileArr (Pile** pile, int size);
void loadDeckFromFile(Pile* deck, char fileName[], char* response[]);
void showDeck(Pile* coloumn[], char* response[]);
int getIntFromCardLetter(char letter);
char getCharFromCardNumber(int cardNumber);
void splitDeck(Pile* deck,Pile* coloumns[], int toSplitAt, char* response[]);
FILE* getFilePointer(char* fileName, char* readMode);
void shuffleDeck(Pile* deck, Pile * coloumns[], char* responsee[]);
void saveDeckFromColoumnsToFile(Pile* coloumns[], char fileName[],char* response[]);
bool LegalMove(Pile* coloumns[], Card* cardToMove, int destIndex);
Card* getLegalMove(Pile* coloumns[], int sourceIndex, int destIndex);
bool LegalMoveFoundation(Pile* foundation, Card* cardToMove);
bool isDeckValid(char* filename);
int cardToIndex(char* card);