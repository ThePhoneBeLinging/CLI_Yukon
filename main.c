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
    Texture2D faceDownCard = LoadTexture("../PNG-cards-1.3/unshown.png");
    faceDownCard.height = 100;
    faceDownCard.width = faceDownCard.height * 0.7159090909;

    //Remember to update the for loop in drawFrame as well (not optimal, but oh well)
    int amountOfButtons = 1;
    Button* buttons[amountOfButtons];
    createButtons(buttons,amountOfButtons);

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
        drawFrame(&deck,coloumns,foundations,&state,textures,faceDownCard,buttons);
    }
    CloseWindow();
    return 0;
}
