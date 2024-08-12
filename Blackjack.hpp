#ifndef BLACKJACK
#define BLACKJACK
// blackjack setup
// Nikolay Zakirov, 2024-08-05
#include "Card_Shoe.hpp"


class Hand {
    public:
    vector<int> cards;
    int Ace_Count;

    int Total() const {
        int total = 0;
        for (int card : cards) {
            total += card;
        }
        return total;
    }

    bool IsSoft() const {
        return Ace_Count > 0 && Total() + 10 <= 21;
    }

    bool CanSplit() const {
        return cards.size() == 2 && cards[0] == cards[1];
    }

    void Clear(){
        cards.clear();
        Ace_Count = 0;
    }

    Hand(){
        Ace_Count = 0;
    }

    ~Hand(){
        cards.clear();
    }
};

class Player{
   private:
    Hand hands[2];
    bool split;
    int credit;
    int wager;
    int lowest_balance;
    
   public:
    Player(int money);
    ~Player();
    void Hit(Card_Shoe &shoe, int hand_num);
    void Deal_In(Card_Shoe &shoe, int money);
    void Split(Card_Shoe &shoe);
    void Double(Card_Shoe &shoe);
    void Win();
    void Lose();
    int Hand_Total(int hand_num);
    const Hand& Cards(int hand_num);
    int Hands_In_Play();
    int Balance();
};

Player::Player(int money){
    credit = money;
}

Player::~Player(){
    for(int i = 0; i < 2; i++){
        hands[i].Clear();
    }
}

void Player::Hit(Card_Shoe &shoe, int hand_num){
    Card draw = shoe.Deal();
    if(draw.value == Ace){
        hands[hand_num].Ace = true;
    }
    hands[hand_num].total += Card_Value_Int[draw.value];
    hands[hand_num].cards.push_back(draw);
}

void Player::Deal_In(Card_Shoe &shoe, int money){
    split = false;
    hands[0].Clear();
    hands[1].Clear();
    wager = money;
    Hit(shoe, 0);
    Hit(shoe, 0);
    if(hands[0].Ace && hands[0].total == 11){
        hands[0].natural_blackjack = true;
        return;
    }
}

void Player::Split(Card_Shoe &shoe){
    Card descendant = hands[0].cards.back();
    hands[0].cards.pop_back();
    hands[1].cards.push_back(descendant);
    Hit(shoe, 0);
    Hit(shoe, 1);
    hands[0].natural_blackjack = false;
    hands[1].natural_blackjack = false;
    wager *= 2;
    split = true;
}

void Player::Double(Card_Shoe &shoe){
    Hit(shoe, 0);
    wager *= 2;
}

void Player::Win(){
    if(split){
        credit += wager / 2;
        return;
    }
    if(hands[0].natural_blackjack){
        credit += wager * 1.5;
    }
    credit += wager;
}

void Player::Lose(){
    if(split){
        credit -= wager / 2;
    }
    credit -= wager;
}

int Player::Hand_Total(int hand_num){
    if(hands[hand_num].Ace && hands[hand_num].total < 12){
        return hands[hand_num].total + 10;
    }
    return hands[hand_num].total;
}

const Hand& Player::Cards(int hand_num){
    return hands[hand_num];
}

int Player::Balance(){
    return credit;
}

class Dealer{
   private:
    Hand hand;

    void Hit(Card_Shoe &shoe);

   public:
    void Deal_In(Card_Shoe &shoe);
    void Call(Card_Shoe &shoe);
    Card Dealer_Card();
    const Hand& Cards();
    void Clear();
    int Total();
    Dealer();
    ~Dealer();
};

Dealer::Dealer(){
    //nothing
}

Dealer::~Dealer(){
    hand.Clear();
}

void Dealer::Hit(Card_Shoe &shoe){
    Card draw = shoe.Deal();
    if(draw.value == Ace){
        hand.Ace = true;
    }
    hand.cards.push_back(draw);
    return;
}

void Dealer::Deal_In(Card_Shoe &shoe){
    hand.Clear();
    Hit(shoe);
    Hit(shoe);
}

void Dealer::Call(Card_Shoe &shoe){
    while(hand.total < 17){
        if(hand.Ace && hand.total > 6 && hand.total < 12){
            return; // exeption (Ace makes it > 16)
        }
        Hit(shoe);
    }
}

int Dealer::Total(){
    if(hand.Ace && hand.total < 12){
        return hand.total + 10;
    }
    return hand.total;
}

Card Dealer::Dealer_Card(){
    return hand.cards.front();
}

void Dealer::Clear(){
    hand.Clear();
}

struct Simulation_Results{
    int rounds_played;
    int house_balance;
    int player_pushes;
    int player_wins;
    int player_loses;
    int player_splits;
    int player_doubles;
    int shoe_shuffles;
};

class Blackjack{
   private:
    vector<Player> players;
    Dealer Tom;
    Card_Shoe Shoe;
    Absent_Map Used_Cards;


    double Bust_Chance(Hand my_hand);
    double Win_Chance(Hand my_hand);
    double Risk_Of_Ruin(double winRate, double lossRate, double averageWin, double averageLoss, double maxRiskPercent, double tradingCapital); // good
    double PreWin_Chance(); // Monte carlo Sim 10,000
    double Natural_Blackjack_Chance();
    void Check_Split(int player_num);
    bool Check_Double(int player_num, int hand_num);
    void Player_Call(int player_num, int hand_num);
    void Payout_Winners(Simulation_Results &tally);
    void Play_Round(Simulation_Results &tally);

   public:
    Simulation_Results Play(int num_games, int num_players, int money_per_player);
};

double Blackjack::Bust_Chance(Hand my_hand){
    if(my_hand.total > 13){// check for odds of not busting and invert it
        double safe_chance = 0;
        Card_Value test_card = Ace;
        while(Card_Value_Int[test_card] + my_hand.total < 22){
            safe_chance += Used_Cards.Probability(test_card);
            test_card = (Card_Value)((int)test_card + 1);
        }
        return 1 - safe_chance;
    }else{// check for odds of busting
        double bust_chance = 0;
        Card_Value test_card = King;
        while(Card_Value_Int[test_card] + my_hand.total > 21){
            bust_chance += Used_Cards.Probability(test_card);
            test_card = (Card_Value)((int)test_card - 1);
        }
        return bust_chance;
    }
}

double Blackjack::Win_Chance(Hand my_hand){
    // compute the chance the dealer beats my_hand
    Card Dcard = Tom.Dealer_Card();
    bool DAce = false;
    if(Dcard.value == Ace){
        DAce = true;
    }
    int hand_total = Card_Value_Int[Dcard.value];
    
}

double Blackjack::Risk_Of_Ruin(double winRate, double lossRate, double averageWin, double averageLoss, double maxRiskPercent, double tradingCapital) {
    double riskOfRuin = pow(lossRate / winRate, tradingCapital / (averageLoss * maxRiskPercent));
    return riskOfRuin;
}

// monte carlo sim
double Blackjack::PreWin_Chance(){

}

double Blackjack::Natural_Blackjack_Chance(){
    return Used_Cards.TenProbability() * Used_Cards.Probability(Ace);
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

#endif