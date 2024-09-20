#include "Strategy_Processing.hpp"

#define NUMBER_OF_DECKS 4

// Must never exceed NUMBER_OF_DECKS * 4
#define NUMBER_OF_POOL_ADDS 2

void compute_rec(Absent_Map current){
      Computed_Strategy_Chart Chart;
      if(current.Cards_Left() > NUMBER_OF_DECKS * 52 - NUMBER_OF_POOL_ADDS){
            for(int i = 1; i < 11; i++){
                  current.Add(i);
                  compute_rec(current);
                  current.Remove(i);
            }
      }
      auto start = chrono::system_clock::now();
      current.Print();
      Chart.Configure(current);
      Chart.Print_All();

      auto end = chrono::system_clock::now();
      auto elapsed_seconds = chrono::duration_cast<chrono::seconds>(end - start);
      cout << "Total Time Elapsed: " << elapsed_seconds.count() / 60 << " minutes " << elapsed_seconds.count() % 60 << " seconds\n";
}


int main(){
      Absent_Map test_map(NUMBER_OF_DECKS);
      compute_rec(test_map);
      
      return 0;
}