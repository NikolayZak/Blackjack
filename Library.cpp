#include "Library.hpp"

// ******************************************************   Hand Method   ******************************************************
void Hand::Add(int card){
    int mask = 0;
    int shift = 0; // Shift amount to isolate the specific card's bits
    switch(card){
    case 1:
        mask = 0x1F;
        shift = 0;
        break;
    case 2:
        mask = 0x1E0;
        shift = 5;
        break;
    case 3:
        mask = 0x1E00;
        shift = 9;
        break;
    case 4:
        mask = 0xE000;
        shift = 13;
        break;
    case 5:
        mask = 0x70000;
        shift = 16;
        break;
    case 6:
        mask = 0x380000;
        shift = 19;
        break;
    case 7:
        mask = 0x1C00000;
        shift = 22;
        break;
    case 8:
        mask = 0x6000000;
        shift = 25;
        break;
    case 9:
        mask = 0x18000000;
        shift = 27;
        break;
    case 10:
        mask = 0x60000000;
        shift = 29;
        break;
    }
    
    // Extract the current value, increment it, and apply it back to the bitmask
    int current_value = (cards & mask) >> shift;  // Isolate the bits and shift them to the right
    current_value++;  // Increment the count
    current_value = current_value << shift;  // Shift the incremented value back to its original position

    // Clear the old value in the bitfield and insert the incremented value
    cards = (cards & ~mask) | (current_value & mask);
}


int Hand::High_Total() const {
    int counter = 0;
    int mask = 0;
    // case 10's
    mask = 0x60000000;
    counter += ((cards & mask) >> 29) * 10;
    // case 9's
    mask = 0x18000000;
    counter += ((cards & mask) >> 27) * 9;
    // case 8's
    mask = 0x6000000;
    counter += ((cards & mask) >> 25) * 8;
    // case 7's
    mask = 0x1C00000;
    counter += ((cards & mask) >> 22) * 7;
    // case 6's
    mask = 0x380000;
    counter += ((cards & mask) >> 19) * 6;
    // case 5's
    mask = 0x70000;
    counter += ((cards & mask) >> 16) * 5;
    // case 4's
    mask = 0xE000;
    counter += ((cards & mask) >> 13) * 4;
    // case 3's
    mask = 0x1E00;
    counter += ((cards & mask) >> 9) * 3;
    // case 2's
    mask = 0x1E0;
    counter += ((cards & mask) >> 5) * 2;
    // case 1's
    mask = 0x1F;
    int tmp = (cards & mask);
    counter += tmp;
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
    int shift = 0; // Shift amount to isolate the specific card's bits
    switch(card){
    case 1:
        mask = 0x1F;
        shift = 0;
        break;
    case 2:
        mask = 0x1E0;
        shift = 5;
        break;
    case 3:
        mask = 0x1E00;
        shift = 9;
        break;
    case 4:
        mask = 0xE000;
        shift = 13;
        break;
    case 5:
        mask = 0x70000;
        shift = 16;
        break;
    case 6:
        mask = 0x380000;
        shift = 19;
        break;
    case 7:
        mask = 0x1C00000;
        shift = 22;
        break;
    case 8:
        mask = 0x6000000;
        shift = 25;
        break;
    case 9:
        mask = 0x18000000;
        shift = 27;
        break;
    case 10:
        mask = 0x60000000;
        shift = 29;
        break;
    }
    
    // Extract the current value, increment it, and apply it back to the bitmask
    int current_value = (cards & mask) >> shift;  // Isolate the bits and shift them to the right
    current_value--;  // Decrement the count
    current_value = current_value << shift;  // Shift the incremented value back to its original position

    // Clear the old value in the bitfield and insert the incremented value
    cards = (cards & ~mask) | (current_value & mask);
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
