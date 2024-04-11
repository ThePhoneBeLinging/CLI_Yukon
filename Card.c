//
// Created by Elias Aggergaard Larsen on 11/04/2024.
//
#include <stddef.h>
#include "Card.h"
#include <stdlib.h>

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