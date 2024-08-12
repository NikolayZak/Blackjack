#ifndef ABSENT_MAP
#define ABSENT_MAP

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cmath>

class Absent_Map{
   private:
    int remaining_cards[10];
    int cards;
    int decks;
    int duplicates;

   public:
    void Add(int drawn);
    void Clear();
    double Probability(int Theta);
    int Count(int Theta) const;
    int Cards_Left();
    Absent_Map(int number_of_decks);
    ~Absent_Map();
};

Absent_Map::Absent_Map(int number_of_decks){
    decks = number_of_decks;
    cards = number_of_decks * 52;
    duplicates = number_of_decks * 4;
    for(int i = 0; i < 9; i++){
        remaining_cards[i] = duplicates;
    }
    remaining_cards[9] = duplicates * 4; // 10's
}

Absent_Map::~Absent_Map(){
    //nothing
}

void Absent_Map::Add(int drawn){
    remaining_cards[drawn]--;
    cards--;
}

void Absent_Map::Clear(){
    cards = decks * 52;
    for(int i = 0; i < 9; i++){
        remaining_cards[i] = duplicates;
    }
    remaining_cards[9] = duplicates * 4; // 10's
}

double Absent_Map::Probability(int Theta){
    return remaining_cards[Theta - 1]/cards;
}

int Absent_Map::Count(int Theta) const {
    return remaining_cards[Theta - 1];
}

int Absent_Map::Cards_Left(){
    return cards;
}

#endif