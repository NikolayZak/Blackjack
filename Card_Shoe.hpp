#ifndef CARD_SHOE
#define CARD_SHOE

#include "General.hpp"
#include "Absent_Map.hpp"

class Card_Shoe{
   private:
    vector<Card> deck;
    int shoe_index;

   public:
    void Reshuffle();
    void Permutate();
    Card Deal();
    bool Half();
    void New_Shoe(const Absent_Map &map);
    Card_Shoe(const int number_of_decks);
    ~Card_Shoe();
};

Card_Shoe::Card_Shoe(const int number_of_decks){
    shoe_index = 0;
    Card tmp;
    for(int i = 0; i < number_of_decks; i++){ // for every deck
        for(int j = 0; j < 13; j++){    // for every value
            tmp.value = (Card_Value)j;
            for(int k = 0; k < 4; k++){ // for every suit
                tmp.suit = (Card_Suit)k;
                deck.push_back(tmp);
            }
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

Card Card_Shoe::Deal(){
    Card ans = deck[shoe_index];
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
    Card tmp;
    for(int i = 0; i < 13; i++){
        num_missing = duplicates - used.Count((Card_Value)i);
        for(int j = 0; j < num_missing; j++){
            counter = shoe_index;
            while(deck[counter].value != Card_Value(i)){
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