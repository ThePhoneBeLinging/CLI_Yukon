#include "StateMachine.h"
#include "raylib.h"

bool stringsAreEqual(char* firstString, char* secondsString);

void handleInput(Pile* deck, Pile* coloumns[], Pile* foundations[], STATE* state, char inputStr[],Texture2D* textures[13][4]);

void printBoard(Pile* coloumns[], Pile* foundations[], STATE* state);

void printUI(Pile* coloumns[], Pile* foundations[], STATE* state, char[], char*[]);

void populateColoumns(STATE* state, Pile* deck, Pile* coloumns[]);

Card* linkColoumnsToSingleLinkedList(Pile* coloumns[]);

FILE* getFilePointer(char* fileName, char* readMode);

bool hasDeckBeenLoaded(Pile* coloumns[]);

void drawFrame(Pile* coloumns[], Pile* foundations[], STATE* state,Texture2D* textures[13][4]);

void initializeTextures(Texture2D* textures[13][4]);
Texture2D cardToTexture(Card card, Texture2D textures[13][4]);