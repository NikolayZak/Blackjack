#ifndef CARD_SHOE
#define CARD_SHOE


#include "Absent_Map.hpp"

using namespace std;

class Card_Shoe{
   private:
    vector<int> deck;
    int shoe_index;

   public:
    void Reshuffle();
    void Permutate();
    int Deal();
    bool Half();
    void New_Shoe(const Absent_Map &map);
    Card_Shoe(const int number_of_decks);
    ~Card_Shoe();
};

Card_Shoe::Card_Shoe(const int number_of_decks){
    shoe_index = 0;
    for(int i = 0; i < number_of_decks; i++){ // for every deck
        for(int j = 1; j < 10; j++){    // for every number (except 10s)
            for(int k = 0; k < 4; k++){ // for every suit
                deck.push_back(j);
            }
        }
        for(int j = 0; j < 16; j++){ // sixteen 10's for every deck
            deck.push_back(10);
        }
    }
    Reshuffle();
}

Card_Shoe::~Card_Shoe(){
    deck.clear();
}

void Card_Shoe::Reshuffle(){
    shoe_index = 0;
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
}

void Card_Shoe::Permutate(){
    random_device rd;
    mt19937 g(rd());
    shuffle(deck.begin() + shoe_index, deck.end(), g);
}

int Card_Shoe::Deal(){
    int ans = deck[shoe_index];
    shoe_index++;
    return ans;
}

bool Card_Shoe::Half(){
    if(shoe_index + 1 > (int)deck.size() / 2){
        return true;
    }
    return false;
}

void Card_Shoe::New_Shoe(const Absent_Map &used){
    int duplicates = (int)deck.size()/13;
    int num_missing;
    int counter;
    shoe_index = 0;
    int tmp;
    for(int i = 1; i < 11; i++){
        num_missing = duplicates - used.Count(i);
        for(int j = 0; j < num_missing; j++){
            counter = shoe_index;
            while(deck[counter] != i){
                counter++;
            }
            // swap to front
            tmp = deck[counter];
            deck.erase(deck.begin() + counter);
            deck.emplace(deck.begin(), tmp);
            shoe_index++;
        }
    }
    Permutate();
}

#endif