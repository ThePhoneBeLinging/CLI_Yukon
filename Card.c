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
        pile[i]->size = 0;
        pile[i]->firstCard = NULL;
        pile[i]->lastCard = NULL;
    }
}

void loadDeckFromFile (Pile *deck, char *fileName)
{
    char fileToLoad[50];
    strcpy(fileToLoad, "../");
    if (*fileName == ' ')
    {
        strcat(fileToLoad,"Decks/");
        strcat(fileToLoad,fileName);
    }
    else
    {
        strcat(fileToLoad,"unshuffledDeck");
    }
    strcat(fileToLoad,".txt");
    FILE* filePTR;
    filePTR = fopen(&fileToLoad[0],"r");
    if (filePTR == NULL)
    {
        printf("File not found");
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

void showDeck (Pile* coloumns[])
{
    for (int i = 0; i < 7; i++)
    {
        Card* currentCard = coloumns[i]->firstCard;
        while (currentCard != NULL)
        {
            currentCard->faceUp = true;
            currentCard = currentCard->nextCard;
        }
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

char getCharFromCardNumber (int cardNumber)
{
    if (cardNumber == 1) return 'A';
    if (cardNumber == 10) return 'T';
    if (cardNumber == 11) return 'J';
    if (cardNumber == 12) return 'Q';
    if (cardNumber == 13) return 'K';
    return (char) cardNumber + '0';

}

