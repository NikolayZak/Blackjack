#include "Blackjack.hpp"


double Blackjack::BJ_EV(Absent_Map &pool){
    return pool.Probability(10) * pool.Probability(1) * 5; // blackjack pays 2.5, there are 2 ways to draw 2 cards
}

void Blackjack::Stand_Rec(Absent_Map pool, int my_total, Hand dealer_hand, double multiplier, double &EV){
    // assuming the player_total is less then 22
    // base case: Dealer has 17 or more: check for a player win
    int dealer_total = dealer_hand.High_Total();
    if(dealer_total > 16){                                // Dealer has a hand
        if(dealer_total > 21 || dealer_total < my_total){ // Dealer Lose: Bust || Dealer Short
            EV += multiplier * 2.0; // wins pay 2 times
        }else if(dealer_total == my_total){
            EV += multiplier; // push
        }
        // loss is assumed in EV = -1
        return;
    } // implied else (Dealer needs to pickup)

    int card_dups;
    double original_prob = multiplier; // save state
    for(int i = 1; i < 11; i++){ // go through the map
        card_dups = pool.Count(i);
        if(card_dups == 0){
            continue; // no copies
        }
        
        // iterate 1 hand
        dealer_hand.Add(i);
        multiplier *= pool.Probability(i);
        pool.Add(i);
        Stand_Rec(pool, my_total, dealer_hand, multiplier, EV);
        dealer_hand.Remove_Last();
        pool.Remove(i);
        multiplier = original_prob;
    }
}

// returns the expected value of a stand in this position
double Blackjack::Stand_EV(const Absent_Map &pool, const Hand &current, int dealer_card){
    double ans = -1.0;
    if(current.High_Total() > 21){
        return ans;
    }
    Hand D_Hand;
    D_Hand.Add(dealer_card);
    Stand_Rec(pool, current.High_Total(), D_Hand, 1.0, ans);
    return ans;
}

