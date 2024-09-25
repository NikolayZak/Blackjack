#include "Strategy_Processing.hpp"


#define NUMBER_OF_DECKS 4

// Must never exceed NUMBER_OF_DECKS * 4
#define NUMBER_OF_POOL_ADDS 0

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
double simulate_double(Hand my_hand, int dealer_card, int iterations){
      int wins = 0;
      int losses = 0;
      int pushes = 0;
      int dhigh;
      int chigh;
      Hand dealer_hand;
      Hand current;
      Card_Shoe shoe(NUMBER_OF_DECKS);
      for(int i = 0; i < iterations; i++){
            shoe.Reshuffle();
            dealer_hand.Clear();
            dealer_hand.Add(dealer_card);
            current = my_hand;
            current.Add(shoe.Deal());

            chigh = current.High_Total();
            if(chigh > 21){
                  losses++;
            }

            // Hit on soft 17
            dealer_hand.Add(shoe.Deal());
            if(dealer_hand.Ace()){ // soft 17
                  dealer_hand.Add(shoe.Deal()); // hits on soft 17
            }

            while(dealer_hand.High_Total() < 17){
                  dealer_hand.Add(shoe.Deal());
            }

            dhigh = dealer_hand.High_Total();
            if(dhigh > 21 || chigh > dhigh){
                  wins++;
            }else if(chigh == dhigh){
                  pushes++;
            }else{
                  losses++;
            }
      }
      double EV = (double)(wins - losses)/50;
      return EV;
}

double simulate_hit(Hand my_hand, int dealer_card, int iterations){
      int wins = 0;
      int losses = 0;
      int pushes = 0;
      int dhigh;
      int chigh;
      bool loop;
      Hand dealer_hand;
      Hand current;
      Card_Shoe shoe(NUMBER_OF_DECKS);
      for(int i = 0; i < iterations; i++){
            shoe.Reshuffle();
            dealer_hand.Clear();
            dealer_hand.Add(dealer_card);
            current = my_hand;
            loop = true;
            while(loop){
                  current.Add(shoe.Deal());
                  if(current.High_Total() > 11){
                        loop = false;
                  }
            }

            chigh = current.High_Total();
            if(chigh > 21){
                  losses++;
            }

            // Hit on soft 17
            dealer_hand.Add(shoe.Deal());
            if(dealer_hand.Ace()){ // soft 17
                  dealer_hand.Add(shoe.Deal()); // hits on soft 17
            }

            while(dealer_hand.High_Total() < 17){
                  dealer_hand.Add(shoe.Deal());
            }

            dhigh = dealer_hand.High_Total();
            if(dhigh > 21 || chigh > dhigh){
                  wins++;
            }else if(chigh == dhigh){
                  pushes++;
            }else{
                  losses++;
            }
      }
      
      double EV = (double)(wins - losses)/100;
      return EV;
}

int main(){
      Absent_Map test_map(NUMBER_OF_DECKS);
      //compute_rec(test_map);
      Hand my_hand;
      my_hand.Add(8);
      cout << "Double EV: " << simulate_double(my_hand, 6, 10000000) << endl;
      cout << "Hit EV: " << simulate_hit(my_hand, 6, 10000000) << endl;
      
      return 0;
}