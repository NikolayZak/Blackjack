#include "Blackjack.hpp"

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
    Card_Shoe Shoe(8);
    Player P1(1000);
    Dealer D1;
    D1.Deal_In(Shoe);
    P1.Deal_In(Shoe, 100);
    Print_Hand(P1.Cards(P1.Hands_In_Play() - 1));
    cout << D1.Call(Shoe) << endl;

    return 0;
}