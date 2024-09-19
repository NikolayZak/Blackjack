#include "Strategy_Processing.hpp"

// Need to compute weights based off the dealer card aswell

Computed_Strategy_Chart::Computed_Strategy_Chart(){
    EV = -1;
}

Computed_Strategy_Chart::~Computed_Strategy_Chart(){
    // nothing
}


// Precondition: current is a Hand with 2 elements
// Assuming Pool is after the deal
// Consequence splits are not considered hard total weights (aka 4,4 will not be considered in the weight of hard 8)
double Computed_Strategy_Chart::Compute_Weight(const Absent_Map &pool, const Hand &current, int dealer_card){
    int tmp;
    int hand_total = current.High_Total();
    // case splits
    if(current.Can_Split()){
        if(current.Ace()){ // case Ace
            tmp = 1;
        }else{
            tmp = hand_total / 2;
        }
        return pool.Probability(tmp, tmp, dealer_card);
    }

    // case soft hand
    if(current.Ace()){ // case Hand: Ace + card x
        tmp = hand_total - 11;
        return pool.Probability(1, tmp, dealer_card);
    }

    // compute the number of ways to get the hard_total
    double ans = 0;
    for(int i = 2; i < ((hand_total / 2) + 1); i++){
        tmp = hand_total - i;
        if(tmp != i && tmp < 11){
            ans += pool.Probability(i, tmp, dealer_card);
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
            hard_chart[i-5][j-1] = W_Move(BJ.Best_Move(pool, current, j), Compute_Weight(pool, current, j));
        }
        current.Clear();
    }

    current.Add(6);
    current.Add(5); // for hard total = 11
    for(int j = 1; j < 11; j++){ // for every dealer card
        hard_chart[6][j-1] = W_Move(BJ.Best_Move(pool, current, j), Compute_Weight(pool, current, j));
    }
    current.Clear();

    current.Add(10);
    for(int i = 12; i < 20; i++){ // for every hard total > 11
        current.Add(i-10);
        for(int j = 1; j < 11; j++){ // for every dealer card
            hard_chart[i-5][j-1] = W_Move(BJ.Best_Move(pool, current, j), Compute_Weight(pool, current, j));
        }
        current.Remove(i-10);
    }
    current.Clear();
    

    // compute the soft totals
    current.Add(1);
    for(int i = 13; i < 22; i++){ // for every soft total
        current.Add(i-11);
        for(int j = 1; j < 11; j++){ // for every dealer card
            soft_chart[i-13][j-1] = W_Move(BJ.Best_Move(pool, current, j), Compute_Weight(pool, current, j));
        }
        current.Remove(i-11);
    }
    current.Clear();

    // compute the split cards
    for(int i = 1; i < 11; i++){ // for every split card
        current.Add(i);
        current.Add(i);
        for(int j = 1; j < 11; j++){ // for every dealer card
            split_chart[i-1][j-1] = W_Move(BJ.Best_Move(pool, current, j), Compute_Weight(pool, current, j));
        }
        current.Clear();
    }

    // Computing the total ev
    EV = 0;
    for(int i = 0; i < 15; i++){ // hard chart evs
        for(int j = 0; j < 10; j++){
            EV += hard_chart[i][j].weight * hard_chart[i][j].EV;
        }
    }
    for(int i = 0; i < 9; i++){ // soft chart evs
        for(int j = 0; j < 10; j++){
            EV += soft_chart[i][j].weight * soft_chart[i][j].EV;
        }
    }
    for(int i = 0; i < 10; i++){ // split chart evs
        for(int j = 0; j < 10; j++){
            EV += split_chart[i][j].weight * split_chart[i][j].EV;
        }
    }
}

void Computed_Strategy_Chart::Print_Hard(){
    cout << "                         Dealer Upcard\n";
    cout << "  Hard Total | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |\n";

    for(int i = 5; i < 20; i++){ // hard total
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

    for(int i = 13; i < 22; i++){ // soft total
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

// Precondition MUST BE CONFIGURED
void Computed_Strategy_Chart::Print_All(){
    Print_Hard();
    Print_Soft();
    Print_Split();
    cout << endl << "Total EV: " << EV << endl << endl;
}