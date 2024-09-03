#include "Strategy_Processing.hpp"
// TODO
// Add many layers of hashing
// Test to see if splitting on 3's and 4's is optimal



//                         Dealer Upcard
//  Hard Total| 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |
//      5     | H | H | H | H | H | H | H | H | H  | H |
//      6     | H | H | H | H | H | H | H | H | H  | H |
//      7     | H | H | H | H | H | H | H | H | H  | H |
//      8     | H | H | H | H | H | H | H | H | H  | H |
//      9     | H | D | D | D | D | H | H | H | H  | H |
//      10    | D | D | D | D | D | D | D | D | H  | H |
//      11    | D | D | D | D | D | D | D | D | D  | D |
//      12    | H | H | S | S | S | H | H | H | H  | H |
//      13    | S | S | S | S | S | H | H | H | H  | H |
//      14    | S | S | S | S | S | H | H | H | H  | H |
//      15    | S | S | S | S | S | H | H | H | H  | H |
//      16    | S | S | S | S | S | H | H | H | H  | H |
//      17    | S | S | S | S | S | S | S | S | S  | S |


//                         Dealer Upcard
//  Soft Total| 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |
//      13    | H | H | H | D | D | H | H | H | H  | H |
//      14    | H | H | H | D | D | H | H | H | H  | H |
//      15    | H | H | D | D | D | H | H | H | H  | H |
//      16    | H | H | D | D | D | H | H | H | H  | H |
//      17    | H | D | D | D | D | H | H | H | H  | H |
//      18    | S | D | D | D | D | S | S | H | H  | H |
//      19    | S | S | S | S | S | S | S | S | S  | S |
//      20    | S | S | S | S | S | S | S | S | S  | S |


//                         Dealer Upcard
// Split Cards| 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |
//     2,2    | H | H | P | P | P | P | H | H | H  | H |
//     3,3    | H | H | P | P | P | P | H | H | H  | H |
//     4,4    | H | H | H | H | H | H | H | H | H  | H |
//     5,5    | D | D | D | D | D | D | D | D | H  | H |
//     6,6    | H | P | P | P | P | H | H | H | H  | H |
//     7,7    | P | P | P | P | P | P | H | H | H  | H |
//     8,8    | P | P | P | P | P | P | P | P | P  | P |
//     9,9    | P | P | P | P | P | S | P | P | S  | S |
//    10,10   | S | S | S | S | S | S | S | S | S  | S |
//     A,A    | P | P | P | P | P | P | P | P | P  | P |



/*

                         Dealer Upcard
  Hard Total | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |
      5      | H | H | H | H | H | H | H | H | H  | H |
      6      | H | H | H | H | H | H | H | H | H  | H |
      7      | H | H | H | H | H | H | H | H | H  | H |
      8      | H | H | H | H | D | H | H | H | H  | H |
      9      | D | D | D | D | D | H | H | H | H  | H |
      10     | D | D | D | D | D | D | D | D | H  | H |
      11     | D | D | D | D | D | D | D | D | H  | H |
      12     | H | H | H | S | S | H | H | H | H  | H |
      13     | S | S | S | S | S | H | H | H | H  | H |
      14     | S | S | S | S | S | H | H | H | H  | H |
      15     | S | S | S | S | S | H | H | H | H  | H |
      16     | S | S | S | S | S | H | H | H | H  | H |
      17     | S | S | S | S | S | S | S | S | S  | S |

                         Dealer Upcard
  Soft Total | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |
      13     | H | H | H | D | D | H | H | H | H  | H |
      14     | H | H | H | D | D | H | H | H | H  | H |
      15     | H | H | H | D | D | H | H | H | H  | H |
      16     | H | H | D | D | D | H | H | H | H  | H |
      17     | H | D | D | D | D | H | H | H | H  | H |
      18     | S | D | D | D | D | S | S | H | H  | S |
      19     | S | S | S | S | S | S | S | S | S  | S |
      20     | S | S | S | S | S | S | S | S | S  | S |

                         Dealer Upcard
 Split Cards | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |
     2,2     | H | H | H | H | H | P | H | H | H  | H |
     3,3     | H | H | H | H | H | H | H | H | H  | P |
     4,4     | H | H | H | H | D | H | H | H | H  | H |
     5,5     | D | D | D | D | D | D | D | D | H  | H |
     6,6     | H | H | H | S | S | H | H | H | H  | H |
     7,7     | P | P | P | P | P | P | H | H | H  | H |
     8,8     | P | P | P | P | P | P | P | P | P  | P |
     9,9     | P | P | P | P | P | S | P | P | S  | S |
    10,10    | S | S | S | S | S | S | S | S | S  | S |
     A,A     | P | P | P | P | P | P | P | P | P  | P |

*/

void Computed_Strategy_Chart::Configure(const Absent_Map &pool){
    Blackjack BJ;
    Hand current;
    
    cout << "HARD\n";
    // compute the hard totals
    for(int i = 5; i < 18; i++){ // for every hard total
        for(int j = 1; j < 11; j++){ // for every dealer card
            current.Add(i);
            hard_chart[i-5][j-1] = BJ.Best_Move(pool, current, j);
            cout << i << " VS " << j << " : ";
            hard_chart[i-5][j-1].Print();
            current.Remove_Last();
        }
    }
    
    cout << "SOFT\n";
    // compute the soft totals
    current.Add(1);
    for(int i = 13; i < 21; i++){ // for every soft total
        for(int j = 1; j < 11; j++){ // for every dealer card
            current.Add(i-11);
            soft_chart[i-13][j-1] = BJ.Best_Move(pool, current, j);
            cout << i << " VS " << j << " : ";
            soft_chart[i-13][j-1].Print();
            current.Remove_Last();
        }
    }
    current.Remove_Last();

    cout << "SPLIT\n";
    // compute the split cards
    for(int i = 1; i < 11; i++){ // for every split card
        for(int j = 1; j < 11; j++){ // for every dealer card
            current.Add(i);
            current.Add(i);
            split_chart[i-1][j-1] = BJ.Best_Move(pool, current, j);
            cout << i << " VS " << j << " : ";
            split_chart[i-1][j-1].Print();
            current.Remove_Last();
            current.Remove_Last();
        }
    }
}

void Computed_Strategy_Chart::Print_Hard(){
    cout << "                         Dealer Upcard\n";
    cout << "  Hard Total | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |\n";

    for(int i = 5; i < 18; i++){ // hard total
        cout << "      " << i;
        if(i > 9){
            cout << "     | ";
        }else{
            cout << "      | ";
        }
        for(int j = 2; j < 11; j++){ // dealer card
            cout << hard_chart[i-5][j-1].name;
            if(j == 10){
                cout << "  | ";
            }else{
                cout << " | ";
            }
        }
        cout << hard_chart[i-5][0].name << " |\n";
    }
}

void Computed_Strategy_Chart::Print_Soft(){
    cout << "                         Dealer Upcard\n";
    cout << "  Soft Total | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |\n";

    for(int i = 13; i < 21; i++){ // soft total
        cout << "      " << i;
        if(i > 9){
            cout << "     | ";
        }else{
            cout << "      | ";
        }
        for(int j = 2; j < 11; j++){ // dealer card
            cout << soft_chart[i-13][j-1].name;
            if(j == 10){
                cout << "  | ";
            }else{
                cout << " | ";
            }
        }
        cout << soft_chart[i-13][0].name << " |\n";
    }
}

void Computed_Strategy_Chart::Print_Split(){
    cout << "                         Dealer Upcard\n";
    cout << " Split Cards | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |\n";

    for(int i = 2; i < 11; i++){ // soft total
        if(i == 10){
            cout << "    " << i << "," << i << "    | ";
        }else{
            cout << "     " << i << "," << i << "     | ";
        }
        for(int j = 2; j < 11; j++){ // dealer card
            cout << split_chart[i-1][j-1].name;
            if(j == 10){
                cout << "  | ";
            }else{
                cout << " | ";
            }
        }
        cout << split_chart[i-1][0].name << " |\n";
    }
    
    cout << "     A,A     | ";
    for(int j = 2; j < 11; j++){ // dealer card
        cout << split_chart[0][j-1].name;
        if(j == 10){
            cout << "  | ";
        }else{
            cout << " | ";
        }
    }
    cout << split_chart[0][0].name << " |\n";
}


void Computed_Strategy_Chart::Print_All(){
    Print_Hard();
    cout << endl;
    Print_Soft();
    cout << endl;
    Print_Split();
}