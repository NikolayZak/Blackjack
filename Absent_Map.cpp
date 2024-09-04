#include "Absent_Map.hpp"

Absent_Map::Absent_Map(int number_of_decks){
    if(number_of_decks > 8){
        std::cout << "COMPRESSION WILL FAIL" << std::endl;
    }
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

// MUST BE COMPUTED BEFORE ADDING TO POOL
double Absent_Map::Probability(int Theta) const{
    return (double)remaining_cards[Theta - 1]/(double)cards;
}

int Absent_Map::Count(int Theta) const {
    return remaining_cards[Theta - 1];
}

int Absent_Map::Cards_Left() const{
    return cards;
}

// Assuming each element from 0-8 in the map is BETWEEN 0-63 (6 bits) and the last element 9 is between 0-255 (8 bits)
uint64_t Absent_Map::Compressed_Map() const{
    uint64_t ans = 0;
    for(int i = 0; i < 8; i++){
        ans |= uint64_t(remaining_cards[i] & 0x1F) << (i * 6);
    }
    ans |= uint64_t(remaining_cards[8] & 0xFF) << 48;
}

bool Absent_Map::operator==(const Absent_Map &other) const{
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