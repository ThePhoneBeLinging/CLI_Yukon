#include <stdio.h>

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
void loadDeckFromFile(Pile* deck, char fileName[]);
void showDeck(Pile* coloumn[]);
int getIntFromCardLetter(char letter);
char getCharFromCardNumber(int cardNumber);
void splitDeck(Pile* deck,Pile* coloumns[], int toSplitAt);
FILE* getFilePointer(char* fileName, char* readMode);