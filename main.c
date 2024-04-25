#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Card.h"
#include "Utility.h"
#include "raylib.h"

int main (void)
{
    int screenHeight = 800;
    int screenWidth = 1400;
    InitWindow(screenWidth, screenHeight, "Yukon - Solitaire");
    SetTargetFPS(60);
    Texture2D* textures[13][4];
    for (int i = 0; i < 13; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            textures[i][k] = malloc(sizeof(Texture2D));
        }
    }
    initializeTextures(textures);

    // Set the current phase to the startup phase
    STATE state = FIRSTPRINT;
    Pile deck;
    int AMOUNT_OF_COLOUMNS = 7;
    int AMOUNT_OF_FOUNDATIONS = 4;
    Pile* coloumns[AMOUNT_OF_COLOUMNS];
    Pile* foundations[AMOUNT_OF_FOUNDATIONS];
    mallocPileArr(foundations, AMOUNT_OF_FOUNDATIONS);
    mallocPileArr(coloumns,AMOUNT_OF_COLOUMNS);


    char inputStr[50] = {};
    while (!WindowShouldClose())
    {
        handleInput(&deck,coloumns,foundations,&state,inputStr,textures);
    }
    CloseWindow();
    return 0;
}
