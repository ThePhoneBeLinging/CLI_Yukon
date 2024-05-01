#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Card.h"
#include "Utility.h"
#include "raylib.h"
#include "GUI.h"

int main (void)
{
    int screenHeight = 800;
    int screenWidth = 1400;
    char inputStr[50] = {};
    char* response[50] = {0};
    bool hasWon = false;


    while (! stringsAreEqual(inputStr,"CLI") && ! stringsAreEqual(inputStr,"GUI"))
    {
        printf("For GUI type 'GUI'\nFor CLI type 'CLI'\n");
        scanf("%s",inputStr);
    }
    // Set the current phase to the startup phase
    STATE state = NODECK;
    Pile deck;
    int AMOUNT_OF_COLOUMNS = 8;
    int AMOUNT_OF_FOUNDATIONS = 4;
    Pile* coloumns[AMOUNT_OF_COLOUMNS];
    Pile* foundations[AMOUNT_OF_FOUNDATIONS];
    mallocPileArr(foundations, AMOUNT_OF_FOUNDATIONS);
    mallocPileArr(coloumns,AMOUNT_OF_COLOUMNS);

    if (stringsAreEqual(inputStr,"GUI"))
    {
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
        int amountOfButtons = 10;
        Button* buttons[amountOfButtons];
        createButtons(buttons,amountOfButtons);
        int coloumnOfSelectedItems = -1;
        bool takenFromColoumn = true;
        int drawLine = 0;
        char argument[50]= {0};
        int positionOfLine = 0;
        char terminalText[26][50] = {0};
        COMMAND commandToExecute = INVALID_COMMAND;
        strcpy(terminalText[0],"Welcome To Yukon - Solitarie");
        while (!WindowShouldClose())
        {
            drawFrame(&deck,coloumns,foundations,&state,textures,faceDownCard,buttons,amountOfButtons,&coloumnOfSelectedItems,&takenFromColoumn,terminalText, &drawLine,&positionOfLine, argument, &commandToExecute);
            for (int i = 0; i < 7; i++)
            {
                if (coloumns[i]->size != 0) break;
                if (i == 6 && state == PLAY) hasWon = true;
            }
            if (hasWon) break;
        }
        CloseWindow();
    }
    else if (stringsAreEqual(inputStr,"CLI"))
    {
        while (true)
        {
            char argument[50]= {0};
            COMMAND command = getInputFromTerminal(&deck, coloumns, foundations, &state, inputStr, response, argument);
            if (command == INVALID_COMMAND)
            {
                *response = "Invalid Command";
                continue;
            }
            if (command != MOVEDCARD) *response = runCommand(&deck,coloumns,foundations,&state,command,argument);
            for (int i = 0; i < 7; i++)
            {
                if (coloumns[i]->size != 0) break;
                if (i == 6 && state == PLAY) hasWon = true;
            }
            if (hasWon) break;

        }
    }
    if (hasWon)
    {
        printf("CONGRATULATIONS, YOU HAVE WON");
        scanf("");
    }

    return 0;
}
