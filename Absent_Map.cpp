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

Absent_Map::Absent_Map(const Absent_Map &a_map){
    for(int i = 0; i < 10; i++){
        remaining_cards[i] = a_map.remaining_cards[i];
    }
    cards = a_map.cards;
    decks = a_map.decks;
    duplicates = a_map.duplicates;
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

bool Absent_Map::operator==(const Absent_Map &other){
    if(cards == other.cards){
        for(int i = 0; i < 10; i++){
            if(remaining_cards[i] != other.remaining_cards[i]){
                return false;
            }
        }
        return true;
    }
    return false;
}