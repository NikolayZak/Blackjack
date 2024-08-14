#include "Blackjack.hpp"

#define NUMBER_OF_DECKS 1
#define NUMBER_OF_PLAYERS 5
#define CREDIT_PER_PLAYER 0



int main(){
    Blackjack BJ(NUMBER_OF_DECKS, NUMBER_OF_PLAYERS, CREDIT_PER_PLAYER);
    Absent_Map test_map(NUMBER_OF_DECKS);
    Hand test_hand;
    test_hand.Add(10);
    test_hand.Add(2);

    test_map.Add(10);
    test_map.Add(2);
    test_map.Add(4);

    Move choice = BJ.Best_Move(test_map, test_hand, 1);
    cout << choice.stand_EV << endl;
    return 0;
}