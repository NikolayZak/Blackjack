#include "Blackjack.hpp"

#define NUMBER_OF_DECKS 8
#define BET_SIZE 10



/*
Wins
1.    Stephen's Hand: 10 9 
      Dealer's Card: 9 | Tom's Total: 22

2.    Stephen's Hand: 4 1 6
      Dealer's Card: 10 | Tom's Total: 18

3.    Stephen's Hand: 3 6 10 
      Dealer's Card: 1 | Tom's Total: 18
*/
int main(){
    Blackjack BJ;
    Absent_Map test_map(NUMBER_OF_DECKS);
    Hand test_hand;
    // Assume 1 deck
    for(int i = 0; i < NUMBER_OF_DECKS; i++){
        test_map.Add(2);
        test_map.Add(5);
        test_map.Add(3);
        test_map.Add(3);
    }

    Simulation_Results Sim = BJ.Simulate(NUMBER_OF_DECKS, test_map, 10, BET_SIZE);
    cout << "Rounds Played: " << Sim.rounds_played << endl;
    cout << "Bet Size: " << BET_SIZE << endl;
    cout << "Player Balance: " << Sim.player_balance << endl;
    cout << "Player Wins: " << Sim.player_wins << endl;
    cout << "Player Losses: " << Sim.player_losses << endl;
    cout << "Player Pushes: " << Sim.player_pushes << endl;
    cout << "Player Blackjacks: " << Sim.player_blackjack << endl;
    cout << "Player Doubles: " << Sim.player_doubles << endl;
    cout << "Player Splits: " << Sim.player_splits << endl;
    return 0;
}
