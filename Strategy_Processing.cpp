#include "Strategy_Processing.hpp"
// TODO
// Add many layers of hashing




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

// Precondition: current is a Hand with 2 elements
// Assuming Pool is after the deal
double Computed_Strategy_Chart::Compute_Weight(Absent_Map pool, const Hand &current){
    int tmp;
    int hand_total = current.High_Total();
    // case splits
    if(current.Can_Split()){
        if(current.Ace()){ // case Ace
            tmp = 1;
        }else{
            tmp = hand_total / 2;
        }
        pool.Remove(tmp);
        pool.Remove(tmp);
        return pool.Probability(tmp, tmp);
    }

    // case soft hand
    if(current.Ace()){ // case Hand: Ace + card x
        tmp = hand_total - 11;
        pool.Remove(1);
        pool.Remove(tmp);
        return pool.Probability(1, tmp);
    }

    // compute the number of ways to get the hard_total
    double ans = 0;
    for(int i = 2; i < ((hand_total / 2) + 1); i++){
        tmp = hand_total - i;
        if(tmp != i && tmp < 11){
            pool.Remove(i);
            pool.Remove(tmp);
            ans += pool.Probability(i, tmp);
            pool.Add(i);
            pool.Add(tmp);
        }
    }
    return ans;
}


void Computed_Strategy_Chart::Configure(const Absent_Map &pool){
    Hand current;
    
    // compute the hard totals
    for(int i = 5; i < 11; i++){ // for every hard total < 11
        current.Add(i);
        for(int j = 1; j < 11; j++){ // for every dealer card
            hard_chart[i-5][j-1] = W_Move(BJ.Best_Move(pool, current, j), Compute_Weight(pool, current));
        }
        current.Clear();
    }

    current.Add(6);
    current.Add(5); // for hard total = 11
    for(int j = 1; j < 11; j++){ // for every dealer card
        hard_chart[6][j-1] = W_Move(BJ.Best_Move(pool, current, j), Compute_Weight(pool, current));
    }
    current.Clear();

    current.Add(10);
    for(int i = 12; i < 18; i++){ // for every hard total > 11
        current.Add(i-10);
        for(int j = 1; j < 11; j++){ // for every dealer card
            hard_chart[i-5][j-1] = W_Move(BJ.Best_Move(pool, current, j), Compute_Weight(pool, current));
        }
        current.Remove(i-10);
    }
    current.Clear();
    

    // compute the soft totals
    current.Add(1);
    for(int i = 13; i < 21; i++){ // for every soft total
        current.Add(i-11);
        for(int j = 1; j < 11; j++){ // for every dealer card
            soft_chart[i-13][j-1] = W_Move(BJ.Best_Move(pool, current, j), Compute_Weight(pool, current));
        }
        current.Remove(i-11);
    }
    current.Clear();

    // compute the split cards
    for(int i = 1; i < 11; i++){ // for every split card
        current.Add(i);
        current.Add(i);
        for(int j = 1; j < 11; j++){ // for every dealer card
            split_chart[i-1][j-1] = W_Move(BJ.Best_Move(pool, current, j), Compute_Weight(pool, current));
        }
        current.Clear();
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
    cout << endl;
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
    cout << endl;
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

void Computed_Strategy_Chart::Print_Hard_Weight(){
    cout << "  Hard Total\n";

    for(int i = 5; i < 18; i++){ // hard total
        cout << "      " << i;
        if(i > 9){
            cout << "     | ";
        }else{
            cout << "      | ";
        }
        cout << hard_chart[i-5][0].weight << endl;
    }
    cout << endl;
}

void Computed_Strategy_Chart::Print_Soft_Weight(){
    cout << "  Soft Total\n";

    for(int i = 13; i < 21; i++){ // soft total
        cout << "      " << i;
        if(i > 9){
            cout << "     | ";
        }else{
            cout << "      | ";
        }
        cout << soft_chart[i-13][0].weight << endl;
    }
    cout << endl;
}

void Computed_Strategy_Chart::Print_Split_Weight(){
    cout << " Split Cards\n";

    for(int i = 2; i < 11; i++){ // soft total
        if(i == 10){
            cout << "    " << i << "," << i << "    | ";
        }else{
            cout << "     " << i << "," << i << "     | ";
        }
        cout << split_chart[i-1][0].weight << endl;
    }
    
    cout << "     A,A     | ";
    cout << split_chart[0][0].weight << endl;
}

void Computed_Strategy_Chart::Print_All(){
    Print_Hard();
    Print_Hard_Weight();
    Print_Soft();
    Print_Soft_Weight();
    Print_Split();
    Print_Split_Weight();

    double ans = 0;
    for(int i = 0; i < 13; i++){
        ans += hard_chart[i][0].weight;
    }

    for(int i = 0; i < 8; i++){
        ans += soft_chart[i][0].weight;
    }

    for(int i = 0; i < 10; i++){
        ans += split_chart[i][0].weight;
    }
    cout << endl << ans << endl;
}