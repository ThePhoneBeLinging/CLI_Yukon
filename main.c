#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Card.h"
#include "Utility.h"
int main (void)
{
    // Set the current phase to the startup phase
    STATE state = STARTUP;
    Pile deck;
    int AMOUNT_OF_COLOUMNS = 7;
    int AMOUNT_OF_FOUNDATIONS = 4;
    Pile* coloumns[AMOUNT_OF_COLOUMNS];
    Pile* foundations[AMOUNT_OF_FOUNDATIONS];
    mallocPileArr(foundations, AMOUNT_OF_FOUNDATIONS);
    mallocPileArr(coloumns,AMOUNT_OF_COLOUMNS);
    while (true){
    handleInput(&deck,coloumns,foundations,&state);
    }
    return 0;
}
