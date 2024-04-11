//
// Created by Elias Aggergaard Larsen on 11/04/2024.
//
#include <stddef.h>
#include "Card.h"

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
