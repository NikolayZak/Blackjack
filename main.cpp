#include "Blackjack.hpp"
#include "Stats.hpp"

#define NUMBER_OF_DECKS 1

int main(){
      Blackjack BJ;
      Absent_Map test_map(NUMBER_OF_DECKS);


      Hand my_hand;
      my_hand.Add(8);
      my_hand.Add(8);
      test_map.Add(8);
      test_map.Add(8);
      test_map.Add(8);
      BJ.Stats(test_map, my_hand, 8);
}