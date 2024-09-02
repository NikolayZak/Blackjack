#include "Strategy_Processing.hpp"
#include "Stats.hpp"

#define NUMBER_OF_DECKS 1

int main(){
      Absent_Map test_map(NUMBER_OF_DECKS);
      Computed_Strategy_Chart Chart;
      Chart.Configure(test_map);
      Chart.Print_All();

      return 0;
}