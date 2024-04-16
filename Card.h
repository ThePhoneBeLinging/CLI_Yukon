//
// Created by Elias Aggergaard Larsen on 11/04/2024.
//
typedef struct Card Card;
struct Card{
    Card* nextCard;
    int number;
    char suit;
    bool faceUp;

};

typedef struct{
    Card* firstCard;
    Card* lastCard;
    int size;
} Pile;

void pushToPile(Pile* pile, Card* card);
void mallocPileArr (Pile** pile, int size);
void loadDeckFromFile(Pile* deck, char fileName[]);
int getIntFromCardLetter(char letter);
char getCharFromCardNumber(int cardNumber);