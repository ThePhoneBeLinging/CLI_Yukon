#include "StateMachine.h"

bool stringsAreEqual(char* firstString, char* secondsString);

void handleInput(Pile* deck, Pile* coloumns[], Pile* foundations[], STATE* state);

void printBoard(Pile* coloumns[], Pile* foundations[], STATE* state);

void populateColoumns(Pile* deck, Pile* coloumns[]);

Card* linkColoumnsToSingleLinkedList(Pile* coloumns[]);