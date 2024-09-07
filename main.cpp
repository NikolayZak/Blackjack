#include "Strategy_Processing.hpp"
#include "Stats.hpp"

#define NUMBER_OF_DECKS 4

void print(const vector<int> &stack){
      cout << endl;
      for(int i = 0; i < (int)stack.size(); i++){
            cout << stack[i] << " ";
      }
      cout << endl;
}

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






      /* deal out and compute every permutation of 1 card missing
      int max_cards = 3;
      int next_card;
      bool loop = true;
      vector<int> stack;
      stack.push_back(1);
      while(loop){
            if((int)stack.size() < max_cards){
                  test_map.Add(stack.back());
                  stack.push_back(stack.back());
            }
            if((int)stack.size() == max_cards){
                  print(stack);
                  Chart.Configure(test_map);
                  Chart.Print_All();

                  while(stack.back() == 10){
                        test_map.Remove(10);
                        stack.pop_back();
                  }
                  if(stack.size() == 0){
                        loop = false;
                        break;
                  }
                  next_card = stack.back() + 1;
                  test_map.Remove(stack.back());
                  test_map.Add(next_card);
                  stack.pop_back();
                  stack.push_back(next_card);
            }
      }
      */
      
      return 0;
}