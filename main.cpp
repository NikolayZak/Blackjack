#include "Blackjack.hpp"

#define NUMBER_OF_DECKS 1
#define NUMBER_OF_PLAYERS 5
#define CREDIT_PER_PLAYER 0




int main(){
    
}




/*
int main(){
    Blackjack BJ(NUMBER_OF_DECKS, NUMBER_OF_PLAYERS, CREDIT_PER_PLAYER);
    Absent_Map test_map(NUMBER_OF_DECKS);
    Hand test_hand;
    test_hand.Add(10);
    test_hand.Add(10);

    test_map.Add(10);
    test_map.Add(10);
    test_map.Add(6);

    Move choice = BJ.Best_Move(test_map, test_hand, 6);
    cout << "Stand EV: " << choice.stand_EV << endl;
    cout << "Hit EV: " << choice.hit_EV << endl;
    cout << "Double EV: " << choice.double_EV << endl;
    if(choice.split_EV != -10){
        cout << "Split EV: " << choice.split_EV << endl;
    }
    return 0;
}
*/