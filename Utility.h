#include "StateMachine.h"

bool stringsAreEqual(char* firstString, char* secondsString);

void handleInput(Pile* deck, Pile* coloumns[], Pile* foundations[], STATE* state, char inputStr[]);

void printBoard(Pile* coloumns[], Pile* foundations[], STATE* state);

void printUI(Pile* coloumns[], Pile* foundations[], STATE* state, char[], char*[]);

void populateColoumns(STATE* state, Pile* deck, Pile* coloumns[]);

Card* linkColoumnsToSingleLinkedList(Pile* coloumns[]);

FILE* getFilePointer(char* fileName, char* readMode);

bool hasDeckBeenLoaded(Pile* coloumns[]);

void moveCardBetweenColoumns(Pile* coloumns[], int sourceIndex, int destIndex);