#include "Blackjack.hpp"
#include <chrono>

#define NUMBER_OF_DECKS 8
#define BET_SIZE 10


int main(){
    Blackjack BJ;
    Absent_Map test_map(NUMBER_OF_DECKS);
    Hand test_hand;
    // Scales to 1 deck
    for(int i = 0; i < NUMBER_OF_DECKS; i++){
    }

    auto start = chrono::high_resolution_clock::now();
    Simulation_Results Sim = BJ.Simulate(NUMBER_OF_DECKS, test_map, 100, BET_SIZE);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "Time Elapsed: " << duration << endl;
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
