#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Card.h"
#include "Utility.h"
#include "raylib.h"

int main (void)
{
    int screenHeight = 500;
    int screenWidth = 800;
    // Set the current phase to the startup phase
    STATE state = FIRSTPRINT;
    Pile deck;
    int AMOUNT_OF_COLOUMNS = 7;
    int AMOUNT_OF_FOUNDATIONS = 4;
    Pile* coloumns[AMOUNT_OF_COLOUMNS];
    Pile* foundations[AMOUNT_OF_FOUNDATIONS];
    mallocPileArr(foundations, AMOUNT_OF_FOUNDATIONS);
    mallocPileArr(coloumns,AMOUNT_OF_COLOUMNS);

    InitWindow(screenWidth, screenHeight, "Yukon - Solitaire");
    SetTargetFPS(60);
    char inputStr[50] = {};
    while (!WindowShouldClose())
    {
        handleInput(&deck,coloumns,foundations,&state,inputStr);
    }
    CloseWindow();
    return 0;
}
