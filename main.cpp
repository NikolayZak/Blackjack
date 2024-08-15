#include "Blackjack.hpp"
#include "Stats.hpp"

#define NUMBER_OF_DECKS 8
#define CONFIDENCE_LEVEL 0.95

// TEST_RUNS MUST BE DIVISIBLE BY 2!!!
#define TEST_RUNS 10000

int main(){
      // Setup
      Blackjack BJ;
      Absent_Map test_map(NUMBER_OF_DECKS);
      auto sides = findClosestRectangleSides(TEST_RUNS);
      vector<double> data;


      // Simulations
      auto start = chrono::high_resolution_clock::now();
      for(int i = 0; i < sides.first; i++){
            data.push_back(BJ.Pre_Win_Chance(NUMBER_OF_DECKS, test_map, sides.second));
      }
      auto end = chrono::high_resolution_clock::now();

      // printing
      auto confidence_interval = calculate_confidence_interval(data, CONFIDENCE_LEVEL);
      double variance = (confidence_interval.second - confidence_interval.first) / 2;
      double average_return = confidence_interval.first + variance;
      chrono::duration<double> duration = end - start;
      cout << "Time Elapsed: " << (int)duration.count() / 60 << " Minutes " << (int)duration.count() % 60 << " Seconds\n";
      cout << "Number of Rounds: " << sides.second * sides.first << endl;
      cout << "Rate of Return With " << 100 * CONFIDENCE_LEVEL << "% " << "Confidence: " << confidence_interval.first << " to " << confidence_interval.second << endl;
      cout << "Average Return: " << average_return << " With Varience of: " << variance << endl;
      return 0;
}
 