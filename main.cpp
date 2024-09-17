#include "Strategy_Processing.hpp"
#include "Stats.hpp"

#define NUMBER_OF_DECKS 4

int main(){
      Absent_Map test_map(NUMBER_OF_DECKS);
      Computed_Strategy_Chart Chart;
      
      // For now: computing each chart of 1 card missing at a time
      for(int i = 1; i < 3; i++){
            auto start = chrono::system_clock::now();

            test_map.Add(i);

            test_map.Print();
            Chart.Configure(test_map);
            Chart.Print_All();

            test_map.Remove(i);

            auto end = chrono::system_clock::now();
            auto elapsed_seconds = chrono::duration_cast<chrono::seconds>(end - start);
            cout << "Time Elapsed: " << elapsed_seconds.count() / 60 << " minutes " << elapsed_seconds.count() % 60 << " seconds\n";
      }

      
      return 0;
}