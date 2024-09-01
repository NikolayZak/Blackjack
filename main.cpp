#include "Blackjack.hpp"
#include "Stats.hpp"

#define NUMBER_OF_DECKS 1

int main(){
      Blackjack BJ;
      Absent_Map test_map(NUMBER_OF_DECKS);


      Hand my_hand;
      my_hand.Add(10);
      my_hand.Add(6);
      test_map.Add(10);
      test_map.Add(10);
      test_map.Add(6);
      BJ.Stats(test_map, my_hand, 10);
}