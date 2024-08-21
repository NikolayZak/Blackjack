#ifndef BLACKJACK
#define BLACKJACK
// blackjack setup
// Nikolay Zakirov, 2024-08-05
#include "Library.hpp"

// EV(Game) = P(Hand[i]) * P(D_Card[j]) * Max_EV(Hand[i], D_Card[j]) + P(Blackjack) * 2.5
class Blackjack{
    private:
    // shhh private stuff
    void Stand_Rec(Absent_Map pool, int my_total, Hand Dealer_Hand, double &multiplier, Move &ans);
    Move Stand_EV(const Absent_Map &pool, const Hand &current, int Dealer_Card);
    void Hit_Rec(Absent_Map pool, Hand current, int Dealer_Card, Move &ans);
    Move Hit_EV(const Absent_Map &pool, const Hand &current, int Dealer_Card);
    void Double_Rec(Absent_Map pool, Hand current, int Dealer_Card, Move &ans);
    Move Double_EV(const Absent_Map &pool, const Hand &current, int Dealer_Card);
    void Split_Rec(Absent_Map pool, Hand current, int Dealer_Card, Move &ans);
    Move Split_EV(const Absent_Map &pool, const Hand &current, int Dealer_Card);

    public:
    double BJ_EV(Absent_Map &pool);
    vector<Move> Choice_EV(const Absent_Map &pool, const Hand &current, int Dealer_Card);
    double Game_EV(const Absent_Map &pool);
};

double Blackjack::BJ_EV(Absent_Map &pool){
    return pool.Probability(10) * pool.Probability(1) * 2.5;
}

void Blackjack::Stand_Rec(Absent_Map pool, int my_total, Hand Dealer_Hand, double &multiplier, Move &ans){
    // base case: Dealer has 17 or more
    int dealer_total = Dealer_Hand.High_Total();
    if(dealer_total > 16){                                // Dealer has a hand
        if(dealer_total > 21 || dealer_total < my_total){ // Dealer Lose: Bust || Dealer Short
            ans.EV += multiplier;
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
}

vector<Move> Blackjack::Choice_EV(const Absent_Map &pool, const Hand &current, int Dealer_Card){
    Move Stand = Stand_EV(pool, current, Dealer_Card);
    Move Hit = Hit_EV(pool, current, Dealer_Card);
    Move Double = Double_EV(pool, current, Dealer_Card);
    Move Split = Split_EV(pool, current, Dealer_Card);
    vector<Move> ans;
    ans.push_back(Stand);
    ans.push_back(Hit);
    ans.push_back(Double);
    ans.push_back(Split);
    return ans;
}

double Blackjack::Game_EV(const Absent_Map &pool){
    int D_Card, first_card, second_card;
    vector<Move> tmp;
    vector<double> ans;
    Hand my_hand;
    for(D_Card = 1; D_Card < 11; D_Card++){
        for(first_card = 1; first_card < 11; first_card++){
            my_hand.Add(first_card);
            second_card = first_card;
            while(second_card <= 11){
                my_hand.Add(second_card);
                tmp = Choice_EV(pool, my_hand, D_Card);
                ans.push_back(tmp[0].EV); // best case
                my_hand.Remove_Last();
                second_card++;
            }
            my_hand.Remove_Last();
        }
    }
    // return average EV
    double total = accumulate(ans.begin(), ans.end(), 0.0);
    return total / ans.size();
}


/*
class Blackjack{
   private:
    bool split;
    vector<Hashed_Query> Hashed_hands[10][2];
    vector<Move> Hashed_ans[10][2];
    double Risk_Of_Ruin(double winRate, double lossRate, double averageWin, double averageLoss, double maxRiskPercent, double tradingCapital); // good
    void Stand_Rec(Absent_Map a_map, int my_total, Hand dealer_hand, EV_Results &tally);
    void Split_Rec(Absent_Map a_map, Hand my_hand, int dealer_card, double Multiplier, double &Expected_Value);

    double Stand_EV(Absent_Map a_map, Hand my_hand, int dealer_card);
    double Hit_EV(Absent_Map a_map, Hand my_hand, int dealer_card);
    double Split_EV(Absent_Map a_map, Hand my_hand, int dealer_card);
    void Play(Card_Shoe sim_shoe, Absent_Map sim_map, Simulation_Results &tally, int Bet_Amount);

   public:
    Blackjack();
    ~Blackjack();
    Move Best_Move(const Absent_Map &a_map, const Hand &my_hand, const int &dealer_card);
    Simulation_Results Simulate(int num_decks, Absent_Map Card_Count, int n, int Bet_Amount);
    double Pre_Win_Chance(int num_decks, Absent_Map Card_Count, int n);
};

// ****************************************************************************************************************************************************************


double Blackjack::Bust_Chance(Hand my_hand){
    if(my_hand.Low_Total() > 13){// check for odds of not busting and invert it
        int safe_cards = 0;
        int test_card = 1;
        while(test_card + my_hand.Low_Total() < 22){
            safe_cards += Used_Cards.Count(test_card);
            test_card++;
        }
        return 1 - (safe_cards / Used_Cards.Cards_Left());
    }else{// check for odds of busting
        int bust_cards = 0;
        int test_card = 10;
        while(test_card + my_hand.Low_Total() > 21){
            bust_cards += Used_Cards.Count(test_card);
            test_card--;
        }
        return bust_cards / Used_Cards.Cards_Left();
    }
}

// monte carlo sim
double Blackjack::PreWin_Chance(){

}

double Blackjack::Natural_Blackjack_Chance(){
    return Used_Cards.Probability(10) * Used_Cards.Probability(1);
}

void Blackjack::Check_Split(int player_num){
    Hand my_hand = players[player_num].Cards(0);
    Bust_Chance(my_hand);
}

// Doubles as much as possible
bool Blackjack::Check_Double(int player_num, int hand_num){

}

void Blackjack::Player_Call(int player_num, int hand_num){

}

void Blackjack::Payout_Winners(Simulation_Results &tally){

}

void Blackjack::Play_Round(Simulation_Results &tally){
    // reshuffle if needed
    if(Shoe.Half()){
        Shoe.Reshuffle();
        Used_Cards.Clear();
        tally.shoe_shuffles++;
    }

    // check odds, bet accordingly, deal in each player and the dealer
    for(int i = 0; i < (int)players.size(); i++){
        double P_win = PreWin_Chance();
        double best_wager;
        if(P_win >= 0.5){
            double P_loss = 1 - P_win; 
            double P_BJ = Natural_Blackjack_Chance();
            double player_balance = (double)players[i].Balance();
            best_wager = 2; // start at 2 so the minimum bet is 1 dollar

            double average_win = (P_BJ/P_win) * 2.5 * best_wager + (1 - (P_BJ/P_win)) * 2 * best_wager;
            double average_loss = best_wager;
            double RoR = Risk_Of_Ruin(P_win, P_loss, average_win, average_loss, 1, player_balance);

            while(RoR < 0.02){
                best_wager++;
                average_win = (P_BJ/P_win) * 2.5 * best_wager + (1 - (P_BJ/P_win)) * 2 * best_wager;
                average_loss = best_wager;
                RoR = Risk_Of_Ruin(P_win, P_loss, average_win, average_loss, 1, player_balance);
            }
            best_wager--;
        }else{
            best_wager = 0;
        }
        players[i].Deal_In(Shoe, best_wager);
    }
    Tom.Deal_In(Shoe);

    // each player's turn
    bool hand_split;
    for(int i = 0; i < (int)players.size(); i++){
        // check splits
        Check_Split(i);
        tally.player_splits += players[i].Hands_In_Play() - 1;
        for(int j = 0; j < players[i].Hands_In_Play(); j++){ // for every hand
            // check doubles, call if no doubles
            if(Check_Double(i, j)){
                tally.player_doubles++;
            }else{
                Player_Call(i, j);
            }
        }
    }

    // dealer calls
    Tom.Call(Shoe);
    Payout_Winners(tally);
}

Simulation_Results Blackjack::Play(int num_games, int num_players, int money_per_player){
    //setup
    Simulation_Results ans;
    ans.rounds_played = 0;
    ans.house_balance = 0;
    ans.player_pushes = 0;
    ans.player_wins = 0;
    ans.player_loses = 0;
    ans.player_splits = 0;
    ans.player_doubles = 0;
    ans.shoe_shuffles = 0;
    players.clear();
    Tom.Clear();
    Shoe.Reshuffle();
    Used_Cards.Clear();

    for(int i = 0; i < num_players; i++){
        Player new_player(money_per_player);
        players.push_back(new_player);
    }
    for(int i = 0; i < num_games; i++){
        Play_Round(ans);
    }
    return ans;
}
*/

#endif