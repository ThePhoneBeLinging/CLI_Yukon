#pragma once
#include "Utility.h"
#include "Card.h"
#include <string.h>
#include <stdlib.h>
void drawFrame(Pile* deck, Pile* coloumns[], Pile* foundations[], STATE* state,Texture2D* textures[13][4],Texture2D faceDownCard, Button* buttons[], int amountOfButtons);

void initializeTextures(Texture2D* textures[13][4]);
Texture2D cardToTexture(Card card, Texture2D* textures[13][4]);
void createButtons(Button* buttons[], int amountOfButtons);
