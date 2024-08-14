#include "Blackjack.hpp"

#define NUMBER_OF_DECKS 1
#define NUMBER_OF_PLAYERS 5
#define CREDIT_PER_PLAYER 0






int main(){
    Blackjack BJ;
    Absent_Map test_map(NUMBER_OF_DECKS);
    Hand test_hand;
    test_hand.Add(2);
    test_hand.Add(2);

    test_map.Add(2);
    test_map.Add(2);
    test_map.Add(2);
    test_map.Add(2);
    test_map.Add(3);
    test_map.Add(3);
    test_map.Add(6);

    Move choice = BJ.Best_Move(test_map, test_hand, 6);
    cout << "Stand EV: " << choice.stand_EV << endl;
    cout << "Hit EV: " << choice.hit_EV << endl;
    cout << "Double EV: " << choice.double_EV << endl;
    if(choice.split_EV != -10){
        cout << "Split EV: " << choice.split_EV << endl;
    }
    cout << "Best Move: " << choice.best << endl;
    return 0;
}
