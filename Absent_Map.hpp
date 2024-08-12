#ifndef ABSENT_MAP
#define ABSENT_MAP

#include "General.hpp"

class Absent_Map{
   private:
    int remaining_cards[13];
    int cards;
    int decks;
    int duplicates;

   public:
    void Add(Card_Value drawn);
    void Clear();
    double Probability(Card_Value Theta);
    double TenProbability();
    int Count(Card_Value Theta) const;
    int Cards_Left();
    Absent_Map(int number_of_decks);
    ~Absent_Map();
};

Absent_Map::Absent_Map(int number_of_decks){
    decks = number_of_decks;
    cards = number_of_decks * 52;
    duplicates = number_of_decks * 4;
    for(int i = 0; i < 13; i++){
        remaining_cards[i] = duplicates;
    }
}

Absent_Map::~Absent_Map(){
    //nothing
}

void Absent_Map::Add(Card_Value drawn){
    remaining_cards[drawn]--;
    cards--;
}

void Absent_Map::Clear(){
    cards = decks * 52;
    for(int i = 0; i < 13; i++){
        remaining_cards[i] = duplicates;
    }
}

double Absent_Map::Probability(Card_Value Theta){
    return remaining_cards[Theta]/cards;
}

double Absent_Map::TenProbability(){
    int total_tens = remaining_cards[12] + remaining_cards[11] + remaining_cards[10] + remaining_cards[9];
    return total_tens/cards;
}

int Absent_Map::Count(Card_Value Theta) const {
    return remaining_cards[Theta];
}

int Absent_Map::Cards_Left(){
    return cards;
}

#endif