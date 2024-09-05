#include "Strategy_Processing.hpp"
#include "Stats.hpp"

#define NUMBER_OF_DECKS 8



int main(){
      Absent_Map test_map(NUMBER_OF_DECKS);
      Computed_Strategy_Chart Chart;

      // deal out and compute every permutation of 3 cards
      for(int i = 0; i < 3; i++){

      }

      Chart.Configure(test_map);
      Chart.Print_All();
      
      return 0;
}