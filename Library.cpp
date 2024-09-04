#include "Library.hpp"

// ******************************************************   Hand Method   ******************************************************
void Hand::Add(int card){
    cards.push_back(card);
    if(card == 1){
        Ace_count++;
        if(total < 11){
            total += 11;
            Soft = true;
        }else{
            total++;
        }
    }else{
        total+= card;
    }

    if(Soft && total > 21){
        total -= 10;
        Soft = false;
    }
}

// technical debt
int Hand::High_Total() const {
    return total;
}

bool Hand::Is_Soft() const {
    return Soft;
}

bool Hand::Can_Split() const {
    return cards.size() == 2 && cards[0] == cards[1];
}

void Hand::Clear(){
    Ace_count = 0;
    Soft = false;
    cards.clear();
}

void Hand::Remove_Last(){
    int last_card = cards.back();
    if(last_card == 1){
        Ace_count--;
        if(Soft){
            total -= 11;
        }else{
            total--;
        }

        if(Ace_count == 0){
            Soft = false;
        }
    }
    total -= last_card;
    cards.pop_back();
}

// technical debt : MUST BE COMPUTED AFTER ADDING CARD
double Hand::Half_Permutation() const{
    double counter = 0;
    int target = cards.back();
    for(int i = 1; i < (int)cards.size(); i++){
        if(cards[i] == target){
            counter++;
        }
    }
    return double(cards.size() - 1) / counter;
}

Hand::Hand(){
    Ace_count = 0;
    Soft = false;
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
    hand.Add(draw);
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

void Move::Print(){
    cout << name << " = " << EV << endl;
}

Hashed_Query::Hashed_Query(const Absent_Map &a_map, int high_total) : map(a_map){
    total = high_total;
}
