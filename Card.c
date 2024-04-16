//
// Created by Elias Aggergaard Larsen on 11/04/2024.
//
#include <stddef.h>
#include "Card.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

//Add Card at the bottom of a Pile
void pushToPile(Pile* pile, Card* card)
{
    pile->size += 1;
    if (pile->firstCard == NULL)
    {
        pile->firstCard = card;
        pile->lastCard = card;
        return;
    }
    pile->lastCard->nextCard = card;
    pile->lastCard = card;
}

// Malloc with sizeof Pile on every position in a Pile Arr;
void mallocPileArr (Pile** pile, int size)
{
    for (int i = 0; i < size; i++)
    {
        pile[i] = malloc(sizeof (Pile));
    }
}

void loadDeckFromFile (Pile *deck, char *fileName)
{
    char fileToLoad[50];
    strcpy(fileToLoad, "../");
    strcat(fileToLoad,fileName);
    strcat(fileToLoad,".txt");
    for (int i = 0; i < 50; i++)
    {
    }
    FILE* filePTR;
    filePTR = fopen(&fileToLoad[0],"r");
    if (filePTR == NULL)
    {
        printf("DEAN");
        return;
    }
    Card* cards[52];
    for (int i = 0; i < 52; i++)
    {
        Card* anotherTempCard = malloc(sizeof (Card));
        cards[i] = anotherTempCard;
    }
    char readDeck[50];
    int i = 0;
    while (fgets(readDeck,50,filePTR))
    {
        printf("%s",readDeck);
        int cardNumber = getIntFromCardLetter(readDeck[0]);
        if (cardNumber == -1)
        {
            cardNumber = readDeck[0] - '0';
        }
        char cardSuit = readDeck[1];
        cards[i]->number = cardNumber;
        cards[i]->suit = cardSuit;
        cards[i]->faceUp = false;
        cards[i]->nextCard = NULL;
        if (i == 0)
        {
            deck->firstCard = cards[i];
            deck->lastCard = cards[i];
        }
        else
        {
            deck->lastCard->nextCard = cards[i];
            deck->lastCard = cards[i];
        }
        i++;
    }
}

int getIntFromCardLetter (char letter)
{
    if (letter == 'A') return 1;
    if (letter == 'T') return 10;
    if (letter == 'J') return 11;
    if (letter == 'Q') return 12;
    if (letter == 'K') return 13;
    return -1;
}

