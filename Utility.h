#pragma once
#include "StateMachine.h"
#include "raylib.h"
#include "Commands.h"
#include "Card.h"
typedef struct
{
    int x;
    int y;
    int height;
    int width;
    char* text;
    COMMAND commandToExecute;
    STATE state;
} Button;

bool stringsAreEqual(char* firstString, char* secondsString);

void runCommand(Pile* deck, Pile *coloumns[], Pile *foundations[], STATE* state, COMMAND command);

void printBoard(Pile* coloumns[], Pile* foundations[], STATE* state);

void printUI(Pile* coloumns[], Pile* foundations[], STATE* state, char[], char*[]);

void populateColoumns(STATE* state, Pile* deck, Pile* coloumns[]);

Card* linkColoumnsToSingleLinkedList(Pile* coloumns[]);

FILE* getFilePointer(char* fileName, char* readMode);

bool hasDeckBeenLoaded(Pile* coloumns[]);

void clearFoundations(Pile* foundations[]);

COMMAND getInputFromTerminal(Pile* deck, Pile* coloumns[], Pile* foundations[], STATE* state, char inputStr[],char* response[]);

void moveCardBetweenColoumns(Pile* coloumns[], int sourceIndex, int destIndex, Card* cardToMove);

void moveBottomCardToFoundation(Pile* coloumn, Pile* foundation);

Card* getCardFromString(Pile* coloumn, char cardString[]);
