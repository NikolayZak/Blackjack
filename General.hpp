#ifndef GENERAL
#define GENERAL


#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cmath>

using namespace std;

enum Card_Value{
    Ace,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
};

const string Card_Value_Str[] = {
    "Ace",
    "Two",
    "Three",
    "Four",
    "Five",
    "Six",
    "Seven",
    "Eight",
    "Nine",
    "Ten",
    "Jack",
    "Queen",
    "King"
};

const int Card_Value_Int[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10
};

enum Card_Suit{
    Diamonds,
    Clubs,
    Hearts,
    Spades
};

const string Card_Suit_Str[] = {
    "Diamonds",
    "Clubs",
    "Hearts",
    "Spades"
};

struct Card{
    Card_Value value;
    Card_Suit suit;
};

#endif