#include "GUI.h"
void drawFrame (Pile* deck, Pile *coloumns[], Pile *foundations[], STATE *state,Texture2D* textures[13][4], Texture2D faceDownCard, Button* buttons[], int amountOfButtons, int* coloumnOfSelectedItems, bool* takenFromColoumn, char terminalText[26][50], int* drawLine, int* positionOfLine, char argument[], COMMAND* commandToExectue)
{
    if (*positionOfLine > 49)
    {
        *positionOfLine = 49;
    }
    if (*positionOfLine < 0)
    {
        *positionOfLine = 0;
    }
    // Print board
    BeginDrawing();
    ClearBackground(GetColor(0x34A249FF));
    int x = 5;
    int y = 5;
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
                else DrawTexture(cardToTexture(*foundations[foundationsDrawn]->lastCard,textures),x,y,WHITE);
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
        // Draw terminal box
        x = 850;
        y = 30;
    DrawRectangle(x,y,500,400, ColorAlpha(BLACK,0.7f));
    x += 5;
    if (*drawLine == 40)
    {
        *drawLine = 0;
        if (terminalText[25][*positionOfLine] == '\0' && *commandToExectue != INVALID_COMMAND)
        {
            terminalText[25][*positionOfLine] = '|';
        }
        else
        {
            terminalText[25][*positionOfLine] = '\0';
        }
    }
    else
    {
        *drawLine = *drawLine + 1;
    }
    for (int i = 0; i < 26; i++)
    {
        DrawText(terminalText[i],x,y,15,WHITE);
        y+= 15;
    }
    if (IsKeyPressed(KEY_ENTER))
    {
        moveTerminalOneLineUp(terminalText,positionOfLine);
        if (*commandToExectue != INVALID_COMMAND)
        {
            strcpy(terminalText[25],runCommand(deck,coloumns,foundations,state,*commandToExectue,terminalText[24]));
            moveTerminalOneLineUp(terminalText,positionOfLine);
            *commandToExectue = INVALID_COMMAND;
        }

    }


    char pressedChar = GetCharPressed();

    while (pressedChar != 0)
    {
        terminalText[25][*positionOfLine] = pressedChar;
         pressedChar = GetCharPressed();
         *positionOfLine += 1;
    }
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        terminalText[25][*positionOfLine] = '\0';
        *positionOfLine -= 1;
    }





        x = 5;
        y += 25;
        //Draw coloumn 7 on cursor
        Card* drawOnCursor = coloumns[7]->firstCard;
        int xToDrawOnCursor = GetMouseX() - (faceDownCard.width / 2);
        int yToDrawOnCursor = GetMouseY() - (faceDownCard.height / 2);
        while(drawOnCursor != NULL)
        {
            DrawTexture(cardToTexture(*drawOnCursor,textures),xToDrawOnCursor,yToDrawOnCursor,WHITE);
            drawOnCursor = drawOnCursor->nextCard;
            yToDrawOnCursor += 25;
        }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && *state == PLAY)
    {
        int coloumnOfCard = (GetMouseX() + 5) / 100;
        int positionOfCardInColoumn = (GetMouseY() - 30) / 25;

        if (coloumnOfCard > 6)
        {
            positionOfCardInColoumn /= 4;
            moveCardFoundation(coloumns[7],foundations[positionOfCardInColoumn],false);
            *coloumnOfSelectedItems = positionOfCardInColoumn;
            *takenFromColoumn = false;
        }
        else
        {
            if (coloumns[coloumnOfCard]->size + 2 >= positionOfCardInColoumn)
            {
                if (coloumns[coloumnOfCard]->size <= positionOfCardInColoumn)
                {
                    positionOfCardInColoumn = coloumns[coloumnOfCard]->size;
                }
                Card *cardToTake = coloumns[coloumnOfCard]->firstCard;
                for (int i = 0; i < positionOfCardInColoumn; i ++)
                {
                    if (cardToTake == NULL || cardToTake->nextCard == NULL) break;
                    cardToTake = cardToTake->nextCard;
                }
                if (cardToTake->faceUp)
                {
                    moveCardBetweenColoumns(coloumns, coloumnOfCard, 7, cardToTake);
                    *coloumnOfSelectedItems = coloumnOfCard;
                    *takenFromColoumn = true;
                }

            }
        }
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && *state == PLAY)
    {
        if (*coloumnOfSelectedItems != -1)
        {
            int coloumnOfCard = (GetMouseX() + 5) / 100;
            if (coloumnOfCard > 6)
            {
                int positionOfCardInColoumn = (GetMouseY() - 30) / 25;
                positionOfCardInColoumn /= 4;
                if (coloumns[7]->size == 1)
                {
                    if (LegalMoveFoundation(foundations[positionOfCardInColoumn],coloumns[7]->firstCard))
                    {
                        moveCardFoundation(coloumns[coloumnOfCard], foundations[positionOfCardInColoumn], true);
                        turnOverLastCard(coloumns[coloumnOfCard]);
                    }
                }
            }
            else if (LegalMove(coloumns,coloumns[7]->firstCard,coloumnOfCard))
            {
                turnOverLastCard(coloumns[*coloumnOfSelectedItems]);
                *coloumnOfSelectedItems = coloumnOfCard;
            }
            if (*takenFromColoumn) moveCardBetweenColoumns(coloumns,7,*coloumnOfSelectedItems,coloumns[7]->firstCard);
            else moveCardFoundation(coloumns[7],foundations[*coloumnOfSelectedItems],true);
            *coloumnOfSelectedItems = -1;
        }
    }
    //DrawButtons:
    //For now we use rectangles with text, could change to images easily in the future;

    for (int i = 0; i < amountOfButtons; i++)
    {
        if (buttons[i]->state != *state && buttons[i]->state != ALWAYS) continue;
        DrawRectangle(buttons[i]->x,buttons[i]->y,buttons[i]->width,buttons[i]->height, GetColor(0x2B7B3BEF
        ));
        // Calculate the position of the text to center it
        int textWidth = MeasureText(buttons[i]->text, 20);
        int textX = buttons[i]->x + (buttons[i]->width - textWidth) / 2;
        int textY = buttons[i]->y + (buttons[i]->height - 20) / 2; // assuming the height of the text is approximately 15

        DrawText(buttons[i]->text, textX,textY,20,RED);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && buttons[i]->x < GetMouseX() && buttons[i]->x + buttons[i]->width > GetMouseX() && buttons[i]->y < GetMouseY() && buttons[i]->y + buttons[i]->height > GetMouseY())
        {
            *commandToExectue = buttons[i]->commandToExecute;
            if (*commandToExectue == LOADDECK || *commandToExectue == SAVEDECK)
            {
                strcpy(terminalText[25],"Please enter a filename:");
                moveTerminalOneLineUp(terminalText,positionOfLine);
            }
            else if (*commandToExectue == SPLITDECK)
            {
                strcpy(terminalText[25],"Please enter an integer");
                moveTerminalOneLineUp(terminalText,positionOfLine);
            }
            else
            {
                runCommand(deck, coloumns, foundations, state, *commandToExectue,argument);
                *commandToExectue = INVALID_COMMAND;
            }
        }
    }

    EndDrawing();

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

void createButtons (Button *buttons[], int amountOfButtons)
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
    buttons[0]->state = NODECK;

    buttons[9]->height = buttonHeight;
    buttons[9]->width = buttonWidth;
    buttons[9]->text = "Load";
    buttons[9]->commandToExecute = LOADDECK;
    buttons[9]->x = x;
    buttons[9]->y = y;
    buttons[9]->state = STARTUP;

    x += buttonWidth + 50;

    buttons[1]->height = buttonHeight;
    buttons[1]->width = buttonWidth;
    buttons[1]->text = "Show";
    buttons[1]->commandToExecute = SHOWDECK;
    buttons[1]->x = x;
    buttons[1]->y = y;
    buttons[1]->state = STARTUP;

    x += buttonWidth + 50;

    buttons[2]->height = buttonHeight;
    buttons[2]->width = buttonWidth;
    buttons[2]->text = "Split";
    buttons[2]->commandToExecute = SPLITDECK;
    buttons[2]->x = x;
    buttons[2]->y = y;
    buttons[2]->state = STARTUP;

    x += buttonWidth + 50;

    buttons[3]->height = buttonHeight;
    buttons[3]->width = buttonWidth;
    buttons[3]->text = "Shuffle";
    buttons[3]->commandToExecute = SHUFFLEDECK;
    buttons[3]->x = x;
    buttons[3]->y = y;
    buttons[3]->state = STARTUP;

    x += buttonWidth + 50;

    buttons[4]->height = buttonHeight;
    buttons[4]->width = buttonWidth;
    buttons[4]->text = "Save";
    buttons[4]->commandToExecute = SAVEDECK;
    buttons[4]->x = x;
    buttons[4]->y = y;
    buttons[4]->state = STARTUP;

    x += buttonWidth + 50;

    buttons[5]->height = buttonHeight;
    buttons[5]->width = buttonWidth;
    buttons[5]->text = "Start";
    buttons[5]->commandToExecute = STARTGAME;
    buttons[5]->x = x;
    buttons[5]->y = y;
    buttons[5]->state = STARTUP;

    x += buttonWidth + 50;

    buttons[6]->height = buttonHeight;
    buttons[6]->width = buttonWidth;
    buttons[6]->text = "Quit";
    buttons[6]->commandToExecute = EXIT;
    buttons[6]->x = x;
    buttons[6]->y = y;
    buttons[6]->state = NODECK;

    buttons[7]->height = buttonHeight;
    buttons[7]->width = buttonWidth;
    buttons[7]->text = "Quit";
    buttons[7]->commandToExecute = EXIT;
    buttons[7]->x = x;
    buttons[7]->y = y;
    buttons[7]->state = STARTUP;

    buttons[8]->height = buttonHeight;
    buttons[8]->width = buttonWidth;
    buttons[8]->text = "Quit";
    buttons[8]->commandToExecute = QUITGAME;
    buttons[8]->x = x;
    buttons[8]->y = y;
    buttons[8]->state = PLAY;


}