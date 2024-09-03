#include "Strategy_Processing.hpp"
#include "Stats.hpp"

#define NUMBER_OF_DECKS 8

int Play(Card_Shoe shoe, Absent_Map pool, Hand my_hand, int dealer_card){
      Hand dealer_hand;
      shoe.Permutate();
      while(my_hand.High_Total() < 12 || (my_hand.High_Total() < 18 && my_hand.Is_Soft())){
            my_hand.Add(shoe.Deal());
            pool.Add(my_hand.cards.back());
            if(my_hand.High_Total() > 21){
                  return -1;
            }
      }
      while(dealer_hand.High_Total() < 16){
            dealer_hand.Add(shoe.Deal());
      }

      int dealer_total = dealer_hand.High_Total();
      int my_total = my_hand.High_Total();
      if(dealer_total > 21){ // dealer busted
            return 1;
      }
      if(my_total == dealer_total){ // push
            return 0;
      }
      if(my_total < dealer_total){ // player loss
            return -1;
      } // player win
      return 1;
}


int main(){
      Absent_Map test_map(NUMBER_OF_DECKS);
      //Computed_Strategy_Chart Chart;
      //Chart.Configure(test_map);
      //Chart.Print_All();
      Card_Shoe shoe(NUMBER_OF_DECKS);
      Hand my_hand;
      my_hand.Add(3);
      my_hand.Add(3);
      test_map.Add(3);
      test_map.Add(3);
      test_map.Add(6);
      shoe.New_Shoe(test_map);
      
      int ans;
      int pushes = 0;
      int wins = 0;
      int losses = 0;
      for(int i = 0; i < 1000000; i++){
            ans = Play(shoe, test_map, my_hand, 6);
            if(ans == -1){
                  losses++;
            }else if(ans == 0){
                  pushes++;
            }else{
                  wins++;
            }
      }
      cout << "Wins: " << wins << endl;
      cout << "Losses: " << losses << endl;
      cout << "Pushes: " << pushes << endl;
      double EV = (double)(wins * 2 + pushes - losses * 2) / (double)(wins + pushes + losses);
      cout << "Expected Value: " << EV * 1.0 << endl;
      return 0;
}