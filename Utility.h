#include "StateMachine.h"
#include "raylib.h"
#include "Commands.h"
typedef struct
{
    int x;
    int y;
    int height;
    int width;
    char* text;
    COMMAND commandToExecute;
} Button;

bool stringsAreEqual(char* firstString, char* secondsString);

void handleInput(Pile* deck, Pile* coloumns[], Pile* foundations[], STATE* state, COMMAND command);

void printBoard(Pile* coloumns[], Pile* foundations[], STATE* state);

void printUI(Pile* coloumns[], Pile* foundations[], STATE* state, char[], char*[]);

void populateColoumns(STATE* state, Pile* deck, Pile* coloumns[]);

Card* linkColoumnsToSingleLinkedList(Pile* coloumns[]);

FILE* getFilePointer(char* fileName, char* readMode);

bool hasDeckBeenLoaded(Pile* coloumns[]);

void drawFrame(Pile* deck, Pile* coloumns[], Pile* foundations[], STATE* state,Texture2D* textures[13][4],Texture2D faceDownCard, Button* buttons[]);
void initializeTextures(Texture2D* textures[13][4]);
Texture2D cardToTexture(Card card, Texture2D* textures[13][4]);
void createButtons(Button* buttons[], int amountOfButtons);