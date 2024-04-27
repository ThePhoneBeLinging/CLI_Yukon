#include <string.h>
#include "Card.h"
#include "Utility.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

bool stringsAreEqual(char* firstString, char* secondString)
{
    return strcmp(firstString,secondString) == 0;
}

void handleInput(Pile* deck, Pile* coloumns[], Pile* foundations[], STATE* state, COMMAND command)
{
    // response is currently not used
    char* response[50];
    char* argument = "";
    if (*state == NODECK || *state == STARTUP)
    {
        if (command == LOADDECK)
        {
            loadDeckFromFile(deck,argument, response);
            populateColoumns(state,deck,coloumns);
            *state = STARTUP;
        }
        else if (*state == NODECK)
        {
            response[0] = "Use 'LD' to load a deck";
        }
    }
    if (*state == STARTUP)
    {
        if (command == SHOWDECK)
        {
            showDeck(coloumns, response);

        }
        if (command == SPLITDECK)
        {
            splitDeck(deck,coloumns,atoi(argument),response);
            populateColoumns(state,deck,coloumns);
        }
        if (command == SHUFFLEDECK)
        {
            shuffleDeck(deck,coloumns,response);
            populateColoumns(state,deck,coloumns);

        }
        if (command == SAVEDECK)
        {
            saveDeckFromColoumnsToFile(coloumns,argument,response);
            populateColoumns(state,deck,coloumns);
        }
        if (command == STARTGAME)
        {
            *state = PLAY;
            saveDeckFromColoumnsToFile(coloumns,"temp/temp",NULL);
            populateColoumns(state,deck,coloumns);
            response[0] = "Let the games begin...";
        }
        if (command == QUITGAME)
        {
            exit(0);
        }
    }
    else if (*state == PLAY)
    {
        if (command == QUITGAME)
        {
            *state = STARTUP;
            loadDeckFromFile(deck,"temp/temp",response);
            populateColoumns(state,deck,coloumns);
        }
    }
}

void printBoard(Pile* coloumns[], Pile* foundations[], STATE* state)
{
    printf("\n");
    // Print Coloumn names
    for (int i = 1; i < 8; i++)
    {
        printf("C%d\t",i);
    }
    printf("\n\n");
    int foundationsDrawn = 0;
    for (int i = 0; i < 52; i++)
    {
        bool addedToPrint = false;
        for (int k = 0; k < 7; k++)
        {
            Card* cardToPrint = coloumns[k]->firstCard;
            if (k != 0)
            {
                printf("\t");
            }
            //if (cardToPrint == NULL && i <= 7) printf("\t");
            if (coloumns[k]->size <= i) continue;
            for (int j = 0; j < i; j++)
            {
                if (cardToPrint->nextCard == NULL) break;
                cardToPrint = cardToPrint->nextCard;
            }
            if (cardToPrint->faceUp) printf("%c%c", getCharFromCardNumber(cardToPrint->number), cardToPrint->suit);
            else printf("[]");

            addedToPrint = true;
        }
        if (i % 2 == 0 && foundationsDrawn < 4)
        {
            printf("\t\t");
            if (foundations[foundationsDrawn]->lastCard == NULL) printf("[]");
            else printf("%c%c", getCharFromCardNumber(foundations[foundationsDrawn]->lastCard->number),foundations[foundationsDrawn]->lastCard->suit);
            printf("\tF%d",foundationsDrawn+1);
            addedToPrint = true;
            foundationsDrawn++;
        }

        if (addedToPrint || i <=8) printf("\n");
    }
    printf("\n");
}

void populateColoumns (STATE* state, Pile* deck, Pile *coloumns[])
{
    int amountOfCardsInColoumns[7] = {1,6,7,8,9,10,11};
    // one is subtracted from the facedownCard array, as we manually insert the first card into each array.
    int faceDownCardsInColoumn[7] = {0,0,1,2,3,4,5};
    Card* cardToAddToColoumn;
    cardToAddToColoumn = deck->firstCard;
    for (int i = 0; i < 7; i++)
    {
        coloumns[i]->size = 0;
        coloumns[i]->firstCard = cardToAddToColoumn;
        coloumns[i]->lastCard = cardToAddToColoumn;
        coloumns[i]->size += 1;
        if (*state == PLAY)
        {
            if (i == 0) cardToAddToColoumn->faceUp = true;
            else cardToAddToColoumn->faceUp = false;
        }
        cardToAddToColoumn = cardToAddToColoumn->nextCard;
    }
    int i = -1;
    while (cardToAddToColoumn != NULL)
    {
        i++;
        int coloumnToInsertTo = i % 7;

        if (*state == PLAY)
        {
            if (coloumns[coloumnToInsertTo]->size >= amountOfCardsInColoumns[coloumnToInsertTo])
            {
                continue;
            }
            if (faceDownCardsInColoumn[coloumnToInsertTo] != 0)
            {
                faceDownCardsInColoumn[coloumnToInsertTo] --;
                cardToAddToColoumn->faceUp = false;
            }
            else cardToAddToColoumn->faceUp = true;
        }
        //printf("Current coloumn: %d Size of coloumn: %d\n",coloumnToInsertTo, coloumns[coloumnToInsertTo]->size);
        coloumns[coloumnToInsertTo]->lastCard->nextCard = cardToAddToColoumn;
        coloumns[coloumnToInsertTo]->lastCard = cardToAddToColoumn;
        coloumns[coloumnToInsertTo]->size += 1;
        cardToAddToColoumn = cardToAddToColoumn->nextCard;
    }
    for (int i = 0; i < 7; i++)
    {
        coloumns[i]->lastCard->nextCard = NULL;
    }
}

Card* linkColoumnsToSingleLinkedList (Pile *coloumns[])
{
    Card* firstCard = coloumns[0]->firstCard;
    for (int k = 0; k < 25; k++)
    {
        for (int i = 0; i < 7; i++)
        {

            if (coloumns[i]->firstCard == NULL) break;
            Card* temp = coloumns[i]->firstCard;
            coloumns[i]->firstCard = coloumns[i]->firstCard->nextCard;
            temp->nextCard = coloumns[(i+1)%7]->firstCard;

        }
    }
    return firstCard;
}
FILE* getFilePointer(char* fileName, char* readMode)
{
    FILE* filePTR = malloc(sizeof (FILE));
    filePTR = fopen(&fileName[0],readMode);
    return filePTR;
}

void printUI (Pile *coloumns[], Pile *foundations[], STATE *state, char * command, char * response[])
{
    printBoard(coloumns,foundations,state);
    printf("Last Command > %s\n",command);
    printf("Response > ");
    if (*response != NULL) printf("%s",*response);
    printf("\n");
    printf("Input > ");
}

bool hasDeckBeenLoaded (Pile **coloumns)
{
    for (int i = 0; i < 7; i ++)
    {
        if (coloumns[i]->size == 0) return false;
    }
    return true;
}

void drawFrame (Pile* deck, Pile *coloumns[], Pile *foundations[], STATE *state,Texture2D* textures[13][4], Texture2D faceDownCard, Button* buttons[], int amountOfButtons)
{
    // Print board
    BeginDrawing();
    ClearBackground(GetColor(0x34A249FF));
    int x = 5;
    int y = 0;
    if(hasDeckBeenLoaded(coloumns))
    {
        // Print Coloumn names
        for (int i = 1; i < 8; i++)
        {
            DrawText(TextFormat("C%d",i),x,y,15,BLACK);
            x += 100;
        }
        x = 5;
        y += 25;
        int foundationsDrawn = 0;
        for (int i = 0; i < 52; i++)
        {
            bool addedToPrint = false;
            for (int k = 0; k < 7; k++)
            {
                Card* cardToPrint = coloumns[k]->firstCard;
                if (k != 0)
                {
                    x += 100;
                }
                //if (cardToPrint == NULL && i <= 7) printf("\t");
                if (coloumns[k]->size <= i) continue;
                for (int j = 0; j < i; j++)
                {
                    if (cardToPrint->nextCard == NULL) break;
                    cardToPrint = cardToPrint->nextCard;
                }
                if (cardToPrint->faceUp) DrawTexture(cardToTexture(*cardToPrint,textures),x,y,WHITE);
                else DrawTexture(faceDownCard,x,y,WHITE);

                addedToPrint = true;
            }
            if (i % 4 == 0 && foundationsDrawn < 4)
            {
                x += 100;
                if (foundations[foundationsDrawn]->lastCard == NULL) DrawRectangleLines(x,y,faceDownCard.width,faceDownCard.height,BLACK);
                else DrawText(TextFormat("%c%c", getCharFromCardNumber(foundations[foundationsDrawn]->lastCard->number),foundations[foundationsDrawn]->lastCard->suit),x,y,15,BLACK);
                x += faceDownCard.width + 5;
                DrawText(TextFormat("F%d",foundationsDrawn+1),x,y,15,BLACK);
                addedToPrint = true;
                foundationsDrawn++;
            }

            if (addedToPrint || i <=16)
            {
                x = 5;
                y += 25;
            }
        }
        x = 5;
        y += 25;

    }

    //DrawButtons:
    //For now we use rectangles with text, could change to images easily in the future;

    for (int i = 0; i < amountOfButtons; i++)
    {
        DrawRectangle(buttons[i]->x,buttons[i]->y,buttons[i]->width,buttons[i]->height,BLACK);
        // Calculate the position of the text to center it
        int textWidth = MeasureText(buttons[i]->text, 15);
        int textX = buttons[i]->x + (buttons[i]->width - textWidth) / 2;
        int textY = buttons[i]->y + (buttons[i]->height - 15) / 2; // assuming the height of the text is approximately 15

        DrawText(buttons[i]->text, textX,textY,15,RED);
    }

    EndDrawing();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        for (int i = 0; i < amountOfButtons; i++)
        {
            //NO support for overlapping buttons
            if (buttons[i]->x < GetMouseX() && buttons[i]->x + buttons[i]->width > GetMouseX() && buttons[i]->y < GetMouseY() && buttons[i]->y + buttons[i]->height > GetMouseY())
            {
                printf("DEAJN");
                handleInput(deck,coloumns,foundations,state,buttons[i]->commandToExecute);
            }
        }
    }

}

void initializeTextures (Texture2D* textures[13][4])
{
    char fileToLoad[50];
    for (int i = 0; i < 13; i++)
    {
        for (int k = 0; k < 4; k++)
        {
            printf("%d,%d\n",i,k);
            strcpy(fileToLoad,"../PNG-cards-1.3/");

            switch (i)
            {
                case 0:
                    strcat(fileToLoad,"ace");
                    break;
                case 9:
                    strcat(fileToLoad,"10");
                    break;
                case 10:
                    strcat(fileToLoad,"jack");
                    break;
                case 11:
                    strcat(fileToLoad,"queen");
                    break;
                case 12:
                    strcat(fileToLoad,"king");
                    break;
                default:
                    strcat(fileToLoad, TextFormat("%d",i+1));
            }
            strcat(fileToLoad,"_of_");
            switch (k)
            {
                case 0:
                    strcat(fileToLoad,"clubs");
                    break;
                case 1:
                    strcat(fileToLoad,"diamonds");
                    break;
                case 2:
                    strcat(fileToLoad,"hearts");
                    break;
                case 3:
                    strcat(fileToLoad,"spades");
                    break;
            }
            strcat(fileToLoad,".png");
            Texture2D texture = LoadTexture(fileToLoad);
            *textures[i][k] = texture;
            if (texture.id == 0) k--;
        }
    }
}

Texture2D cardToTexture (Card card, Texture2D* textures[13][4])
{
    int suitNumber = -1;
    switch(card.suit)
    {
        case 'C':
            suitNumber = 0;
            break;
        case 'D':
            suitNumber = 1;
            break;
        case 'H':
            suitNumber = 2;
            break;
        case 'S':
            suitNumber = 3;
            break;
    }
    Texture2D textureToReturn = *textures[card.number - 1][suitNumber];
    textureToReturn.height = 100;
    textureToReturn.width = textureToReturn.height * 0.71;
    return textureToReturn;
}

void
createButtons (Button *buttons[], int amountOfButtons)
{
    for (int i = 0; i < amountOfButtons; i++)
    {
        buttons[i] = malloc(sizeof (Button));
    }
    int x = 0;
    int y = 700;
    int buttonHeight = 100;
    int buttonWidth = 150;

    buttons[0]->height = buttonHeight;
    buttons[0]->width = buttonWidth;
    buttons[0]->text = "Load";
    buttons[0]->commandToExecute = LOADDECK;
    buttons[0]->x = x;
    buttons[0]->y = y;

    x += buttonWidth + 50;

    buttons[1]->height = buttonHeight;
    buttons[1]->width = buttonWidth;
    buttons[1]->text = "Show";
    buttons[1]->commandToExecute = SHOWDECK;
    buttons[1]->x = x;
    buttons[1]->y = y;

    x += buttonWidth + 50;

    buttons[2]->height = buttonHeight;
    buttons[2]->width = buttonWidth;
    buttons[2]->text = "Split";
    buttons[2]->commandToExecute = SPLITDECK;
    buttons[2]->x = x;
    buttons[2]->y = y;

    x += buttonWidth + 50;

    buttons[3]->height = buttonHeight;
    buttons[3]->width = buttonWidth;
    buttons[3]->text = "Shuffle";
    buttons[3]->commandToExecute = SHUFFLEDECK;
    buttons[3]->x = x;
    buttons[3]->y = y;

    x += buttonWidth + 50;

    buttons[4]->height = buttonHeight;
    buttons[4]->width = buttonWidth;
    buttons[4]->text = "Save";
    buttons[4]->commandToExecute = SAVEDECK;
    buttons[4]->x = x;
    buttons[4]->y = y;

    x += buttonWidth + 50;

    buttons[5]->height = buttonHeight;
    buttons[5]->width = buttonWidth;
    buttons[5]->text = "Start";
    buttons[5]->commandToExecute = STARTGAME;
    buttons[5]->x = x;
    buttons[5]->y = y;

    x += buttonWidth + 50;

buttons[6]->height = buttonHeight;
    buttons[6]->width = buttonWidth;
    buttons[6]->text = "Quit";
    buttons[6]->commandToExecute = QUITGAME;
    buttons[6]->x = x;
    buttons[6]->y = y;
}
