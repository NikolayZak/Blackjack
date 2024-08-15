#include "Library.hpp"

// ******************************************************   Hand Method   ******************************************************
void Hand::Add(int card){
    cards.push_back(card);
    if(card == 1){
        Ace = true;
    }
}

int Hand::High_Total() const {
    int total = 0;
    for (int card : cards) {
        total += card;
    }
    if(Ace && total < 12){
        total += 10;
    }
    return total;
}

bool Hand::Can_Split() const {
    return cards.size() == 2 && cards[0] == cards[1];
}

void Hand::Clear(){
    Ace = false;
    cards.clear();
}

void Hand::Remove_Last(){
    cards.pop_back();
    for(int i = 0; i < (int)cards.size(); i++){
        if(cards[i] == 1){
            Ace = true;
            return;
        }
    }
    Ace = false;
}

Hand::Hand(){
    Ace = false;
}

Hand::~Hand(){
    cards.clear();
}
// ******************************************************   Player Method   ******************************************************
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
// ******************************************************   Dealer Method   ******************************************************
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
// ******************************************************   Miscellaneous   ******************************************************
Simulation_Results::Simulation_Results(){
    rounds_played = 0;
    player_balance = 0;
    player_wins = 0;
    player_losses = 0;
    player_pushes = 0;
    player_doubles = 0;
    player_splits = 0;
    player_blackjack = 0;
    time = 0;
    bet_size = 0;
}

void Simulation_Results::Print(){
    cout << "Time Elapsed: " << time << " seconds" << endl;
    cout << "Bet Size: " << bet_size << endl;
    cout << "Rounds Played: " << rounds_played << endl;
    cout << "Player Balance: " << player_balance << endl;
    cout << "Player Wins: " << player_wins << endl;
    cout << "Player Losses: " << player_losses << endl;
    cout << "Player Pushes: " << player_pushes << endl;
    cout << "Player Blackjacks: " << player_blackjack << endl;
    cout << "Player Doubles: " << player_doubles << endl;
    cout << "Player Splits: " << player_splits << endl;
}

EV_Results::EV_Results(){
    win_P = 0;
    loss_P = 0;
    push_P = 0;
    prob_hand = 1;
}