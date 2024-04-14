#include <stdio.h>
#include <stdlib.h>
#include "Card.h"
#include "main.h"
int main (void)
{
    Pile* deck;
    deck = malloc(sizeof (Pile));
    int AMOUNT_OF_COLOUMNS = 7;
    int AMOUNT_OF_FOUNDATIONS = 4;
    Pile* coloumns[AMOUNT_OF_COLOUMNS];
    Pile* foundations[AMOUNT_OF_FOUNDATIONS];
    mallocPileArr(foundations, AMOUNT_OF_FOUNDATIONS);
    mallocPileArr(coloumns,AMOUNT_OF_COLOUMNS);

    printf("Hello, World!\n");
    handleInput();
    return 0;
}

void handleInput()
{
    char inputStr[50];
    char raraStr[50];
    printf("Board should be shown and show last message and output \n");
    scanf("%[^\n]s",inputStr);
    printf("%s",inputStr);
}
