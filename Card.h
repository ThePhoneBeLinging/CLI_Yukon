//
// Created by Elias Aggergaard Larsen on 11/04/2024.
//
typedef struct{
    struct Card* nextCard;
    int cardNumber;
    char suit;
    bool faceUp;

} Card;

typedef struct{
    struct Card* firstCard;
    struct Card* lastCard;
} pile;