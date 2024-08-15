#ifndef BLACKJACK
#define BLACKJACK
// blackjack setup
// Nikolay Zakirov, 2024-08-05
#include "Card_Shoe.hpp"


struct Hand {
    vector<int> cards;
    bool Ace;

    void Add(int card){
        cards.push_back(card);
        if(card == 1){
            Ace = true;
        }
    }

    int Low_Total() const {
        int total = 0;
        for (int card : cards) {
            total += card;
        }
        return total;
    }

    int High_Total() const {
        int total = 0;
        for (int card : cards) {
            total += card;
        }
        if(Ace && total < 12){
            total += 10;
        }
        return total;
    }

    bool Can_Split() const {
        return cards.size() == 2 && cards[0] == cards[1];
    }

    void Clear(){
        Ace = false;
        cards.clear();
    }

    void Remove_Last(){
        cards.pop_back();
        for(int i = 0; i < cards.size(); i++){
            if(cards[i] == 1){
                Ace = true;
                return;
            }
        }
        Ace = false;
    }

    Hand(){
        Ace = false;
    }

    ~Hand(){
        cards.clear();
    }
};

struct Player{
    Hand hands[2];
    bool split;
    bool blackjack;
    int credit;
    int wager;
    
    Player(int money);
    ~Player();
    void Hit(Card_Shoe &shoe, int hand_num);
    void Deal_In(Card_Shoe &shoe, int money);
    void Split(Card_Shoe &shoe);
    void Double(Card_Shoe &shoe);
    void Win();
    void Lose();
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
    int draw = shoe.Deal();
    hands[hand_num].Add(draw);
}

void Player::Deal_In(Card_Shoe &shoe, int money){
    split = false;
    blackjack = false;
    hands[0].Clear();
    hands[1].Clear();
    wager = money;

    Hit(shoe, 0);
    Hit(shoe, 0);
    if(hands[0].High_Total() == 21){
        blackjack = true;
    }
}

void Player::Split(Card_Shoe &shoe){
    int descendant = hands[0].cards.back();
    hands[0].cards.pop_back();
    hands[1].Add(descendant);
    Hit(shoe, 0);
    Hit(shoe, 1);
    wager *= 2;
    split = true;
}

void Player::Double(Card_Shoe &shoe){
    Hit(shoe, 0);
    wager *= 2;
}

void Player::Win(){
    if(split){ // split
        credit += wager / 2;
    }else if(blackjack){ // blackjack
        credit += wager * 1.5;
    }else{// regular win
        credit += wager;
    }
}

void Player::Lose(){
    if(split){ // split
        credit -= wager / 2;
    }else{ // regular loss
        credit -= wager;
    }
}

class Dealer{
   private:
    Hand hand;

    void Hit(Card_Shoe &shoe);

   public:
    void Deal_In(Card_Shoe &shoe);
    void Call(Card_Shoe &shoe);
    int Dealer_Card();
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
    int draw = shoe.Deal();
    if(draw == 1){
        hand.Ace = true;
    }
    hand.cards.push_back(draw);
}

void Dealer::Deal_In(Card_Shoe &shoe){
    hand.Clear();
    Hit(shoe); // dealer only has 1 card to simplify
}

void Dealer::Call(Card_Shoe &shoe){
    Hit(shoe);
    while(hand.High_Total() < 17){
        Hit(shoe);
    }
}

int Dealer::Total(){
    return hand.High_Total();
}

int Dealer::Dealer_Card(){
    return hand.cards.front();
}

void Dealer::Clear(){
    hand.Clear();
}

struct Move{
    double stand_EV;
    double hit_EV;
    double double_EV;
    double split_EV;
    char best;
};

struct Simulation_Results{
    int rounds_played;
    int player_balance;
    int player_pushes;
    int player_wins;
    int player_losses;
    int player_splits;
    int player_doubles;
    int player_blackjack;
    Simulation_Results(){
        rounds_played = 0;
        player_balance = 0;
        player_wins = 0;
        player_losses = 0;
        player_pushes = 0;
        player_doubles = 0;
        player_splits = 0;
        player_blackjack = 0;
    }
};

struct EV_Results{
    double win_P;
    double loss_P;
    double push_P;
    double prob_hand;
    EV_Results(){
        win_P = 0;
        loss_P = 0;
        push_P = 0;
        prob_hand = 1;
    }
};

class Blackjack{
   private:
    bool split;
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
};

Blackjack::Blackjack(){
    split = false;
}

Blackjack::~Blackjack(){
    // nothing
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
    int card_dups;
    int my_total = my_hand.High_Total();
    double multiplier = 1;
    Split_Rec(a_map, my_hand, dealer_card, multiplier, ans);
    return ans * 2;
}

Move Blackjack::Best_Move(const Absent_Map &a_map, const Hand &my_hand, const int &dealer_card){
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
        split = false;
    }
    cout << "Stephen's Hand: ";
    for(int i = 0; i < (int)Stephen.hands[0].cards.size(); i++){
        cout << Stephen.hands[0].cards[i] << " ";
    }
    cout << " | Total " << Stephen.hands[0].High_Total();
    cout << endl << "Dealer's Card: " << Tom.Dealer_Card();
    // blackjack not counted as a win
    if(Stephen.blackjack){
        tally.player_blackjack++;
        Stephen.Win();
        tally.player_balance += Stephen.credit;
        cout << endl;
        return;
    }

    // Evaluate
    Tom.Call(sim_shoe);
    int D_total = Tom.Total();
    cout << " | Tom's Total: " << D_total << endl << endl;
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
    }else if(S_total < D_total){
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
}

Simulation_Results Blackjack::Simulate(int num_decks, Absent_Map Card_Count, int n, int Bet_Amount){
    Simulation_Results ans;
    Card_Shoe Shoe(num_decks);
    for(int i = 0; i < n; i++){
        Shoe.New_Shoe(Card_Count);
        Play(Shoe, Card_Count, ans, Bet_Amount);
    }
    return ans;
}

// ****************************************************************************************************************************************************************



/*

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