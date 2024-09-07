#include "Library.hpp"

// ******************************************************   Hand Method   ******************************************************
void Hand::Add(int card){
    int mask = 0;
    switch(card){
    case 1:
        mask = 0x1F;
        break;
    case 2:
        mask = 0x1E0;
        break;
    case 3:
        mask = 0x1E00;
        break;
    case 4:
        mask = 0xE000;
        break;
    case 5:
        mask = 0x70000;
        break;
    case 6:
        mask = 0x380000;
        break;
    case 7:
        mask = 0x1C00000;
        break;
    case 8:
        mask = 0x6000000;
        break;
    case 9:
        mask = 0x18000000;
        break;
    case 10:
        mask = 0x60000000;
        break;
    }
    cards = (cards & ~mask) | ((cards & mask) + 1);
}

int Hand::High_Total() const {
    int counter = 0;
    int mask = 0;
    // case 10's
    mask = 0x60000000;
    counter += (cards & mask) >> 29;
    // case 9's
    mask = 0x18000000;
    counter += (cards & mask) >> 27;
    // case 8's
    mask = 0x6000000;
    counter += (cards & mask) >> 25;
    // case 7's
    mask = 0x1C00000;
    counter += (cards & mask) >> 22;
    // case 6's
    mask = 0x380000;
    counter += (cards & mask) >> 19;
    // case 5's
    mask = 0x70000;
    counter += (cards & mask) >> 16;
    // case 4's
    mask = 0xE000;
    counter += (cards & mask) >> 13;
    // case 3's
    mask = 0x1E00;
    counter += (cards & mask) >> 9;
    // case 2's
    mask = 0x1E0;
    counter += (cards & mask) >> 5;
    // case 1's
    mask = 0x1F;
    int tmp = (cards & mask);
    counter += (cards & mask);
    if(tmp > 0 && counter < 12){
        counter += 10;
    }
    return counter;
}

bool Hand::Can_Split() const {
    // case 10's
    if(cards == 0x40000000){
        return true;
    }
    // case 9's
    if(cards == 0x10000000){
        return true;
    }
    // case 8's
    if(cards == 0x4000000){
        return true;
    }
    // case 7's
    if(cards == 0x800000){
        return true;
    }
    // case 6's
    if(cards == 0x100000){
        return true;
    }
    // case 5's
    if(cards == 0x20000){
        return true;
    }
    // case 4's
    if(cards == 0x4000){
        return true;
    }
    // case 3's
    if(cards == 0x400){
        return true;
    }
    // case 2's
    if(cards == 0x40){
        return true;
    }
    // case 1's
    if(cards == 0x2){
        return true;
    }
    return false;
}

bool Hand::Ace() const{
    return (cards & 0x1F) != 0;
}

void Hand::Clear(){
    cards = 0;
}

void Hand::Remove(int card){
    int mask = 0;
    switch(card){
    case 1:
        mask = 0x1F;
        break;
    case 2:
        mask = 0x1E0;
        break;
    case 3:
        mask = 0x1E00;
        break;
    case 4:
        mask = 0xE000;
        break;
    case 5:
        mask = 0x70000;
        break;
    case 6:
        mask = 0x380000;
        break;
    case 7:
        mask = 0x1C00000;
        break;
    case 8:
        mask = 0x6000000;
        break;
    case 9:
        mask = 0x18000000;
        break;
    case 10:
        mask = 0x60000000;
        break;
    }
    cards = (cards & ~mask) | ((cards & mask) - 1);
}

void Hand::Split(){
    cards = cards >> 1;
}

Hand::Hand(){
    cards = 0;
}

Hand::~Hand(){
    // nothing
}
// ******************************************************   Player Method   ******************************************************
/*
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
    hand.Add(shoe.Deal());
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
*/
// ******************************************************   Miscellaneous   ******************************************************

void Move::Print(){
    cout << name << " = " << EV << endl;
}

Hashed_Query::Hashed_Query(const Absent_Map &a_map, int high_total) : map(a_map){
    total = high_total;
}
