//
// Created by Elias Aggergaard Larsen on 11/04/2024.
//
#include <stddef.h>
#include "Card.h"
#include "Utility.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>
#include <time.h>

// Malloc with sizeof Pile on every position in a Pile Arr;
void mallocPileArr (Pile** pile, int size)
{
    for (int i = 0; i < size; i++)
    {
        pile[i] = malloc(sizeof (Pile));
        pile[i]->size = 0;
        pile[i]->firstCard = NULL;
        pile[i]->lastCard = NULL;
    }
}

void saveDeckFromColoumnsToFile(Pile* coloumns[], char *fileName, char* response[])
{
    char fileToLoad[50];
    strcpy(fileToLoad, "../");
    strcat(fileToLoad,"Decks/");
    if (*fileName != '\0')
    {
        strcat(fileToLoad,fileName);
    }
    else
    {
        strcat(fileToLoad,"cards");
    }
    strcat(fileToLoad,".txt");
    FILE* filePTR = getFilePointer(fileToLoad,"w");
    if (filePTR == NULL)
    {
        if (response != NULL) *response = "File not found";
        return;
    }

    Card* iterationCard = linkColoumnsToSingleLinkedList(coloumns);
    while (iterationCard != NULL)
    {
        fprintf(filePTR,"%c%c\n", getCharFromCardNumber(iterationCard->number),iterationCard->suit);
        iterationCard = iterationCard->nextCard;
    }
    fclose(filePTR);
    if (response != NULL) response[0] = "OK";
}

void loadDeckFromFile (Pile *deck, char *fileName, char * response[])
{
    char fileToLoad[50];
    strcpy(fileToLoad, "../Decks/");
    if (*fileName != '\0')
    {
        strcat(fileToLoad,fileName);
    }
    else
    {
        strcat(fileToLoad,"unshuffledDeck");
    }
    strcat(fileToLoad,".txt");
    FILE* filePTR = getFilePointer(fileToLoad,"r");
    if (filePTR == NULL)
    {
        response[0] = "File not found";
        return;
    }
    Card* cards[52];
    for (int i = 0; i < 52; i++)
    {
        Card* anotherTempCard = malloc(sizeof (Card));
        cards[i] = anotherTempCard;
    }
    char readDeck[50];
    int i = 0;
    while (fgets(readDeck,50,filePTR))
    {
        int cardNumber = getIntFromCardLetter(readDeck[0]);
        if (cardNumber == -1)
        {
            cardNumber = readDeck[0] - '0';
        }
        char cardSuit = readDeck[1];
        cards[i]->number = cardNumber;
        cards[i]->suit = cardSuit;
        cards[i]->faceUp = false;
        cards[i]->nextCard = NULL;
        if (i == 0)
        {
            deck->firstCard = cards[i];
            deck->lastCard = cards[i];
        }
        else
        {
            deck->lastCard->nextCard = cards[i];
            deck->lastCard = cards[i];
        }
        i++;
    }
    // Check if deck is complete
    if (isDeckValid(*deck))
    {
        response[0] = "OK";
    }
    else
    {
        response[0] = "Invalid deck";
        // Free the memory allocated for the cards and stop the loading process
        for (int i = 0; i < 52; i++)
        {
            free(cards[i]);
        }
        deck->firstCard = NULL;
        deck->lastCard = NULL;
        return;
    }
}

void showDeck (Pile* coloumns[], char* response[])
{
    for (int i = 0; i < 7; i++)
    {
        Card* currentCard = coloumns[i]->firstCard;
        while (currentCard != NULL)
        {
            currentCard->faceUp = true;
            currentCard = currentCard->nextCard;
        }
    }

    if (hasDeckBeenLoaded(coloumns)) response[0] = "OK";
    else response[0] = "No deck has been loaded";
}

int getIntFromCardLetter (char letter)
{
    if (letter == 'A') return 1;
    if (letter == 'T') return 10;
    if (letter == 'J') return 11;
    if (letter == 'Q') return 12;
    if (letter == 'K') return 13;

    return letter- '0';
}

char getCharFromCardNumber (int cardNumber)
{
    if (cardNumber == 1) return 'A';
    if (cardNumber == 10) return 'T';
    if (cardNumber == 11) return 'J';
    if (cardNumber == 12) return 'Q';
    if (cardNumber == 13) return 'K';
    return (char) cardNumber + '0';

}

void splitDeck (Pile* deck, Pile * coloumns[],int toSplitAt, char* response[])
{
    if (toSplitAt == 0)
    {
        srand(time(NULL));
        toSplitAt = (rand()+1)%52;
    }
    if (toSplitAt < 2)
    {
        toSplitAt = 3;
    }
    if (toSplitAt > 52)
    {
        toSplitAt = 51;
    }
    deck->firstCard = NULL;
    deck->lastCard = NULL;
    deck->size = 52;
    deck->firstCard = linkColoumnsToSingleLinkedList(coloumns);
    deck->lastCard = deck->firstCard;
    Pile* firstPile = malloc(sizeof(Pile));
    Pile* secondPile = malloc(sizeof(Pile));

    firstPile->firstCard = deck->firstCard;
    firstPile->lastCard = deck->firstCard;
    for (int i = 0; i < toSplitAt; i++)
    {
        Card* iterationCard = firstPile->lastCard->nextCard;
        if (iterationCard == NULL) break;
        if (i == (toSplitAt - 2))
        {
            secondPile->firstCard = firstPile->lastCard->nextCard;
            secondPile->lastCard = firstPile->lastCard->nextCard;
            firstPile->lastCard->nextCard = NULL;
            break;
        }
        firstPile->lastCard->nextCard = iterationCard;
        firstPile->lastCard = iterationCard;
    }
    while (true)
    {
        Card* iterationCard = secondPile->lastCard->nextCard;
        if (iterationCard == NULL) break;
        secondPile->lastCard->nextCard = iterationCard;
        secondPile->lastCard = iterationCard;
    }
    int i = 0;
    while (firstPile->firstCard != NULL || secondPile->firstCard != NULL)
    {
        Card* iterationCard;
        if (i % 2 == 0 && firstPile->firstCard != NULL)
        {
            iterationCard = firstPile->firstCard;
            firstPile->firstCard = firstPile->firstCard->nextCard;
        }
        else if (secondPile->firstCard != NULL)
        {
            iterationCard = secondPile->firstCard;
            secondPile->firstCard = secondPile->firstCard->nextCard;
        }
        deck->lastCard->nextCard = iterationCard;
        deck->lastCard = iterationCard;
        i++;
        if (i > 52) break;
    }
    if (hasDeckBeenLoaded(coloumns)) response[0] = "Deck has been split";
    else response[0] = "No deck has been loaded";
}

void shuffleDeck(Pile* deck, Pile * coloumns[], char* response[]){
    if (!hasDeckBeenLoaded(coloumns))
    {
        response[0] = "No deck has been loaded";
        return;
    }
    srand(time(NULL));

    deck->firstCard =NULL;
    deck->lastCard = NULL;
    deck->size = 52;
    deck->firstCard = linkColoumnsToSingleLinkedList(coloumns);
    deck->lastCard = deck->firstCard;
    Pile* shuffledDeck = malloc(sizeof(Pile));
    shuffledDeck->firstCard = NULL;
    shuffledDeck->lastCard = NULL;
    shuffledDeck->size = 0;

    while (deck->size > 0) {

        int index = rand() % deck->size;


        Card* prevCard = NULL;
        Card* currentCard = deck->firstCard;
        for (int i = 0; i < index; i++) {
            prevCard = currentCard;
            currentCard = currentCard->nextCard;
        }


        if (prevCard == NULL) {
            deck->firstCard = currentCard->nextCard;
        } else {
            prevCard->nextCard = currentCard->nextCard;
        }
        if (currentCard == deck->lastCard) {
            deck->lastCard = prevCard;
        }
        deck->size--;


        if (shuffledDeck->firstCard == NULL) {
            shuffledDeck->firstCard = currentCard;
            shuffledDeck->lastCard = currentCard;
        } else {
            shuffledDeck->lastCard->nextCard = currentCard;
            shuffledDeck->lastCard = currentCard;
        }
        currentCard->nextCard = NULL;
        shuffledDeck->size++;
    }


    *deck = *shuffledDeck;
    free(shuffledDeck);
    response[0] = "OK";
}

bool LegalMove(Pile* coloumns[], Card* cardToMove, int destIndex)
{
    if (cardToMove==NULL) return false;
    if(coloumns[destIndex]->size == 0) {
        if(cardToMove->number != 13) {
            return false;
        }
        return true;
    }
    if (cardToMove->suit == coloumns[destIndex]->lastCard->suit) {
        return false;
    }
    if (cardToMove->number+1!=coloumns[destIndex]->lastCard->number) {
        return false;
    }
    return true;
}

Card* getLegalMove(Pile* coloumns[], int sourceIndex, int destIndex)
{
    // Check if the source column is empty
    if (coloumns[sourceIndex]->size == 0) {
        printf("Source column is empty. No card to move.\n");
        return NULL;
    }

    // Start from the bottom card
    Card* currentCard = coloumns[sourceIndex]->lastCard;


        // Check if the move is legal
        if (LegalMove(coloumns, currentCard, destIndex)) {
            // If the move is legal, return the card
            return currentCard;
        }



    // If no legal move is found, return NULL
    return NULL;
}

bool LegalMoveFoundation(Pile* foundation, Card* cardToMove) {
    // Check if the foundation is empty
    if (foundation->size == 0) {
        // If the foundation is empty, the card number has to be 1
        return cardToMove->number == 1;
    } else {
        // If the foundation is not empty, the card being put in the foundation
        // has to be 1 higher in number and of the same suit as the last card in the foundation
        return cardToMove->number == foundation->lastCard->number + 1 && cardToMove->suit == foundation->lastCard->suit;
    }
}

int cardToIndex(Card card) {


    int number = card.number;


    int suit;
    switch (card.suit) {
        case 'C': suit = 0; break;
        case 'D': suit = 1; break;
        case 'H': suit = 2; break;
        case 'S': suit = 3; break;
        default:
            printf("Error: invalid suit character\n");
            return -1;
    }

    return (number - 1) + suit * 13;
}

bool isDeckValid(Pile deck) {

    bool cardsFound[52] = {false};
    char card[3];
    Card* currentCard = deck.firstCard;
    while (currentCard != NULL)
    {
        int index = cardToIndex(*currentCard);
        if (cardsFound[index])
        {
            return false;
        }
        cardsFound[index] = true;
        currentCard = currentCard->nextCard;
    }

    for (int i = 0; i < 52; i++) {
        if (!cardsFound[i]) {
            return false;
        }
    }

    return true;
}