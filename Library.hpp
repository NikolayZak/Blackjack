#ifndef LIBRARY
#define LIBRARY

#include "Card_Shoe.hpp"

struct Hand {
    int cards;

    void Add(int card);
    int High_Total() const;
    bool Can_Split() const;
    bool Ace() const;
    void Clear();
    void Remove(int card);
    void Split();
    Hand();
    ~Hand();
};

struct Move{
    double EV;
    char name;
};

struct W_Move : Move{
    double weight;
    W_Move() : Move(), weight(0.0) {}
    W_Move(const Move& move, double w) : Move(move), weight(w) {}
};

#endif