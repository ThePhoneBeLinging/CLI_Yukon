#include <stdio.h>
#include <stdlib.h>
#include "Card.h"
int main (void)
{
    Pile* c1;
    c1 = malloc(sizeof(Pile));
    printf("Hello, World!\n");
    Card *card;
    card = malloc(sizeof(Card));
    pushToPile(c1,card);
    printf("%d",c1->firstCard->number);

    return 0;
}
