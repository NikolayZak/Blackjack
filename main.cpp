#include "Blackjack.hpp"


#define NUMBER_OF_DECKS 8
#define BET_SIZE 10





int main(){
      Blackjack BJ;
      Absent_Map test_map(NUMBER_OF_DECKS);
      Hand test_hand;

      // Scales to 1 deck
      for(int i = 0; i < NUMBER_OF_DECKS; i++){

      }
      for(int i = 0; i < 100; i++){

      }
      Simulation_Results Sim = BJ.Simulate(NUMBER_OF_DECKS, test_map, 100, BET_SIZE);
      Sim.Print();
      return 0;
}
