#include "Blackjack.hpp"

#define NUMBER_OF_DECKS 8


void Print_Card(const Card &current){
    cout << Card_Value_Str[current.value] << " of " << Card_Suit_Str[current.suit] << " ";
}

void Print_Hand(const vector<Card>& hand){
    cout << "Player Hand: ";
    for(int i = 0; i < (int)hand.size(); i++){
        Print_Card(hand[i]);
    }
    cout << endl;
}

int main(){
    Absent_Map Card(NUMBER_OF_DECKS);
    Card_Shoe Shoe(NUMBER_OF_DECKS);
    Player Stephen(1000);
    Dealer Tom;
    Tom.Deal_In(Shoe);
    Stephen.Deal_In(Shoe, 100);
    Print_Hand(Stephen.Cards(0).cards);
    cout << Tom.Call(Shoe) << endl;
    
    return 0;
}