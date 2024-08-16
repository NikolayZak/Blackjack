#include "Blackjack.hpp"

Blackjack::Blackjack(){
    split = false;
}

Blackjack::~Blackjack(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 2; j++){
            Hashed_hands[i][j].clear();
            Hashed_ans[i][j].clear();
        }
    }
}

double Blackjack::Risk_Of_Ruin(double winRate, double lossRate, double averageWin, double averageLoss, double maxRiskPercent, double tradingCapital) {
    double riskOfRuin = pow(lossRate / winRate, tradingCapital / (averageLoss * maxRiskPercent));
    return riskOfRuin;
}

// ***********************************************************************   BEST MOVE   **************************************************************************

void Blackjack::Stand_Rec(Absent_Map a_map, int my_total, Hand dealer_hand, EV_Results &tally){
    // base case: Dealer has 17 or more
    int dealer_total = dealer_hand.High_Total();
    if(dealer_total > 16){                                // Dealer has a hand
        if(dealer_total > 21 || dealer_total < my_total){ // Dealer Lose: Bust || Dealer Short
            tally.win_P += tally.prob_hand;
        }else if(dealer_total > my_total){                // Dealer Win
            tally.loss_P += tally.prob_hand;
        }else{                                            // Push
            tally.push_P += tally.prob_hand;
        }
        return;
    } // implied else (Dealer needs to pickup)

    int card_dups;
    double original_prob = tally.prob_hand;
    for(int i = 1; i < 11; i++){ // go through the map
        card_dups = a_map.Count(i);
        if(card_dups == 0){
            continue; // no copies
        }
        
        // iterate 1 hand
        tally.prob_hand *= a_map.Probability(i);
        dealer_hand.Add(i);
        a_map.Add(i);
        Stand_Rec(a_map, my_total, dealer_hand, tally);
        dealer_hand.Remove_Last();
        a_map.Remove(i);
        tally.prob_hand = original_prob;
    }
}

double Blackjack::Stand_EV(Absent_Map a_map, Hand my_hand, int dealer_card){
    EV_Results ans;
    Hand dealer_hand;
    dealer_hand.Add(dealer_card);
    int my_total = my_hand.High_Total();
    Stand_Rec(a_map, my_total, dealer_hand, ans);
    double WR = ans.win_P / (ans.loss_P + ans.win_P + ans.push_P);
    double EV = (WR * 2) - 1;  //(payout)
    return EV;
}

double Blackjack::Hit_EV(Absent_Map a_map, Hand my_hand, int dealer_card){
    EV_Results ans;
    Hand dealer_hand;
    dealer_hand.Add(dealer_card);
    int card_dups;
    int my_total = my_hand.High_Total();

    for(int i = 1; i < 11; i++){ // iterate each possible hit
        card_dups = a_map.Count(i);
        if(card_dups == 0){
            continue; // no copies
        }
        ans.prob_hand *= a_map.Probability(i);
        my_hand.Add(i);
        a_map.Add(i);
        my_total = my_hand.High_Total();
        if(my_total > 21){ // check for bust
            ans.loss_P += ans.prob_hand;
        }else{
            Stand_Rec(a_map, my_total, dealer_hand, ans);
        }
        my_hand.Remove_Last();
        a_map.Remove(i);
        ans.prob_hand = 1; // reset the probability
    }
    double WR = ans.win_P / (ans.loss_P + ans.win_P + ans.push_P);
    double EV = (WR * 2) - 1;  //(payout)
    return EV;
}

void Blackjack::Split_Rec(Absent_Map a_map, Hand my_hand, int dealer_card, double Multiplier, double &Expected_Value){
    double Hit_EV_ = Hit_EV(a_map, my_hand, dealer_card);
    double Stand_EV_ = Stand_EV(a_map, my_hand, dealer_card);

    // base case: Hit_EV < Stand_EV
    if(Hit_EV_ < Stand_EV_){
        Expected_Value += Stand_EV_ * Multiplier;
        return;
    }

    // case recursion
    int card_dups;
    int my_total;
    double old_multy = Multiplier;
    for(int i = 1; i < 11; i++){
        card_dups = a_map.Count(i);
        if(card_dups == 0){
            continue; // no copies
        }
        Multiplier *= a_map.Probability(i);
        my_hand.Add(i);
        a_map.Add(i);
        my_total = my_hand.High_Total();
        if(my_total < 22){ // check it's not busted
            Split_Rec(a_map, my_hand, dealer_card, Multiplier, Expected_Value);
        }
        my_hand.Remove_Last();
        a_map.Remove(i);
        Multiplier = old_multy;
    }
}

double Blackjack::Split_EV(Absent_Map a_map, Hand my_hand, int dealer_card){
    double ans;
    my_hand.Remove_Last();
    double multiplier = 1;
    Split_Rec(a_map, my_hand, dealer_card, multiplier, ans);
    return ans * 2;
}

Move Blackjack::Best_Move(const Absent_Map &a_map, const Hand &my_hand, const int &dealer_card){
    // check hashed
    for(int i = 0; i < (int)Hashed_hands[dealer_card - 1][my_hand.Ace].size(); i++){
        if(Hashed_hands[dealer_card - 1][my_hand.Ace][i].total == my_hand.High_Total()){
            if(Hashed_hands[dealer_card - 1][my_hand.Ace][i].map == a_map){
                return Hashed_ans[dealer_card - 1][my_hand.Ace][i];
            }
        }
    }
    

    Move ans;
    ans.stand_EV = Stand_EV(a_map, my_hand, dealer_card);
    ans.hit_EV = Hit_EV(a_map, my_hand, dealer_card);
    ans.double_EV = ans.hit_EV * 2;
    if(my_hand.Can_Split() && !split){
        ans.split_EV = Split_EV(a_map, my_hand, dealer_card);
    }else{
        ans.split_EV = -10;
    }

    double largest = ans.stand_EV;
    ans.best = 's';  // Start by assuming 's' is the largest

    if (ans.hit_EV + 0.00000001 >= largest) { // add this number so hit is actually larger than stand on low numbers
        largest = ans.hit_EV;
        ans.best = 'h';
    }
    // check to see if a split has occured
    if(split){
        return ans;
    }
    if (ans.double_EV > largest) {
        largest = ans.double_EV;
        ans.best = 'd';
    }
    if (ans.split_EV > largest) {
        ans.best = 'p';
    }

    // Hash answer
    Hashed_Query new_hash(a_map, my_hand.High_Total());
    Hashed_hands[dealer_card - 1][my_hand.Ace].push_back(new_hash);
    Hashed_ans[dealer_card - 1][my_hand.Ace].push_back(ans);

    return ans;
}

void Blackjack::Play(Card_Shoe sim_shoe, Absent_Map sim_map, Simulation_Results &tally, int Bet_Amount){
    Dealer Tom;
    Player Stephen(0);
    tally.rounds_played++;
    Stephen.Deal_In(sim_shoe, Bet_Amount);
    Tom.Deal_In(sim_shoe);
    sim_map.Add(Stephen.hands[0].cards[0]);
    sim_map.Add(Stephen.hands[0].cards[1]);
    sim_map.Add(Tom.Dealer_Card());
    Move choice = Best_Move(sim_map, Stephen.hands[0], Tom.Dealer_Card());
    while(choice.best != 's'){
        if(choice.best == 'd'){ // Double
            Stephen.Double(sim_shoe);
            tally.player_doubles++;
            choice.best = 's'; // end turn
        }else if(choice.best == 'h'){ // Hit
            Stephen.Hit(sim_shoe, 0);
            sim_map.Add(Stephen.hands[0].cards.back());
            if(Stephen.hands[0].High_Total() > 21){
                choice.best = 's'; // bust -> end turn
            }else{ // keep going
                choice = Best_Move(sim_map, Stephen.hands[0], Tom.Dealer_Card());
            }
        }else if(choice.best == 'p'){ // split
            split = true;
            tally.player_splits++;
            Stephen.Split(sim_shoe);
            sim_map.Add(Stephen.hands[0].cards.back());
            sim_map.Add(Stephen.hands[1].cards.back());
            choice = Best_Move(sim_map, Stephen.hands[0], Tom.Dealer_Card()); // keep going on the first hand
        }
    }

    // case split
    if(split){
        choice = Best_Move(sim_map, Stephen.hands[1], Tom.Dealer_Card());
        while(choice.best != 's'){
            // Implied Hit
            Stephen.Hit(sim_shoe, 1);
            sim_map.Add(Stephen.hands[1].cards.back());
            if(Stephen.hands[1].High_Total() > 21){
                choice.best = 's'; // end turn
            }else{
                choice = Best_Move(sim_map, Stephen.hands[1], Tom.Dealer_Card());
            }
        }
    }

    // blackjack not counted as a win
    if(Stephen.blackjack){
        tally.player_blackjack++;
        Stephen.Win();
        tally.player_balance += Stephen.credit;
        return;
    }

    // Evaluate
    Tom.Call(sim_shoe);
    int D_total = Tom.Total();
    int S_total = Stephen.hands[0].High_Total();

        // Player Bust
    if(S_total > 21){
        Stephen.Lose();
        tally.player_losses++;
        // Dealer Bust
    }else if(D_total > 21){
        Stephen.Win();
        tally.player_wins++;
        // Dealer Win
    }else if(D_total > S_total){
        Stephen.Lose();
        tally.player_losses++;
        // Player Win
    }else if(S_total > D_total){
        Stephen.Win();
        tally.player_wins++;
        // Push
    }else{
        tally.player_pushes++;
    }

    // split evaluation
    if(split){
        S_total = Stephen.hands[1].High_Total();
            if(S_total > 21){
            Stephen.Lose();
            tally.player_losses++;
            // Dealer Bust
        }else if(D_total > 21){
            Stephen.Win();
            tally.player_wins++;
            // Dealer Win
        }else if(D_total > S_total){
            Stephen.Lose();
            tally.player_losses++;
            // Player Win
        }else if(S_total < D_total){
            Stephen.Win();
            tally.player_wins++;
            // Push
        }else{
            tally.player_pushes++;
        }
    }
    tally.player_balance += Stephen.credit;
    split = false;
}

Simulation_Results Blackjack::Simulate(int num_decks, Absent_Map Card_Count, int n, int Bet_Amount){
    auto start = chrono::high_resolution_clock::now();
    Simulation_Results ans;
    ans.bet_size = Bet_Amount;
    Card_Shoe Shoe(num_decks);
    for(int i = 0; i < n; i++){
        Shoe.New_Shoe(Card_Count);
        Play(Shoe, Card_Count, ans, Bet_Amount);
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    ans.time = duration.count();
    return ans;
}


double Blackjack::Pre_Win_Chance(int num_decks, Absent_Map Card_Count, int n){
    Simulation_Results ans = Simulate(num_decks, Card_Count, n, 10);
    double total_games = ans.player_blackjack + ans.player_wins + ans.player_losses; // pushes don't count LOL
    double Win_Chance = (((double)ans.player_wins / total_games) * 2 + ((double)ans.player_blackjack / total_games) * 2.5);
    return Win_Chance;
}