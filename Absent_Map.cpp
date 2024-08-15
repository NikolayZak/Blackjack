#include "Absent_Map.hpp"

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
    remaining_cards[drawn - 1]--;
    cards--;
}

void Absent_Map::Remove(int Theta){
    remaining_cards[Theta - 1]++;
    cards++;
}

void Absent_Map::Clear(){
    cards = decks * 52;
    for(int i = 0; i < 9; i++){
        remaining_cards[i] = duplicates;
    }
    remaining_cards[9] = duplicates * 4; // 10's
}

double Absent_Map::Probability(int Theta){
    return (double)remaining_cards[Theta - 1]/(double)cards;
}

int Absent_Map::Count(int Theta) const {
    return remaining_cards[Theta - 1];
}

int Absent_Map::Cards_Left(){
    return cards;
}