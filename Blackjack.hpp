#ifndef BLACKJACK
#define BLACKJACK
// blackjack setup
// Nikolay Zakirov, 2024-08-05

#define MAX_SPLIT 4

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>

using namespace std;

enum Card_Value{
    Ace,
    Two,
    Three,
    Four,
    Five,
    Six,
    Seven,
    Eight,
    Nine,
    Ten,
    Jack,
    Queen,
    King
};

const string Card_Value_Str[] = {
    "Ace",
    "Two",
    "Three",
    "Four",
    "Five",
    "Six",
    "Seven",
    "Eight",
    "Nine",
    "Ten",
    "Jack",
    "Queen",
    "King"
};

enum Card_Suit{
    Diamonds,
    Clubs,
    Hearts,
    Spades
};

const string Card_Suit_Str[] = {
    "Diamonds",
    "Clubs",
    "Hearts",
    "Spades"
};

struct Card{
    Card_Value value;
    Card_Suit suit;
};

class Card_Shoe{
   private:
    vector<Card> deck;
    int shoe_index;

   public:
    void Reshuffle();
    Card Deal();
    Card_Shoe(const int number_of_decks);
    ~Card_Shoe();
};

Card_Shoe::Card_Shoe(const int number_of_decks){
    shoe_index = 0;
    Card tmp;
    for(int i = 0; i < number_of_decks; i++){ // for every deck
        for(int j = 0; j < 13; j++){    // for every value
            tmp.value = (Card_Value)j;
            for(int k = 0; k < 4; k++){ // for every suit
                tmp.suit = (Card_Suit)k;
                deck.push_back(tmp);
            }
        }
    }
    Reshuffle();
}

Card_Shoe::~Card_Shoe(){
    deck.clear();
}

void Card_Shoe::Reshuffle(){
    shoe_index = 0;
    std::random_device rd;
    std::mt19937 g(rd());
    shuffle(deck.begin(), deck.end(), g);
}

Card Card_Shoe::Deal(){
    Card ans = deck[shoe_index];
    shoe_index++;
    return ans;
}

struct Hand{
    vector<Card> cards;
    int total;
    bool Ace;
};

class Player{
   private:
    Hand hands[MAX_SPLIT];
    int valid_hands;
    int credit;
    int wager;
    
   public:
    Player(int money);
    ~Player();
    bool Hit(Card_Shoe &shoe, int hand_num); // returns false on bust
    bool Deal_In(Card_Shoe &shoe, int money); // returns true on blackjack & pays out 2.5x
    void Split(Card_Shoe &shoe, int hand_num);
    void Double(Card_Shoe &shoe, int hand_num);
    void Win();
    int Hand_Total(int hand_num);
    const vector<Card>& Cards(int hand_num);
    int Hands_In_Play();
};

Player::Player(int money){
    credit = money;
}

Player::~Player(){
    for(int i = 0; i < MAX_SPLIT; i++){
        hands[i].cards.clear();
    }
}

bool Player::Hit(Card_Shoe &shoe, int hand_num){
    Card draw = shoe.Deal();
    if(draw.value == Ace){
        hands[hand_num].Ace = true;
    }
    if(draw.value > 9){
        hands[hand_num].total += 10;
    }else{
        hands[hand_num].total += draw.value + 1;
    }
    hands[hand_num].cards.push_back(draw);
    if(hands[hand_num].total > 21){
        return false;
    }
    return true;
}

bool Player::Deal_In(Card_Shoe &shoe, int money){
    valid_hands = 1;
    hands[0].Ace = false;
    hands[0].total = 0;
    hands[0].cards.clear();
    wager = money;
    credit -= money;
    (void)Hit(shoe, 0);
    (void)Hit(shoe, 0);
    if(hands[0].Ace && hands[0].total == 11){
        credit += wager*2.5;
        wager = 0;
        return true;
    }
    return false;
}

void Player::Split(Card_Shoe &shoe, int hand_num){
    
    Card descendant = hands[hand_num].cards.back();
    hands[hand_num].cards.pop_back();
    hands[hand_num + 1].cards.push_back(descendant);
    (void)Hit(shoe, hand_num);
    (void)Hit(shoe, hand_num + 1);
    wager += wager/valid_hands;
    valid_hands++;
}

void Player::Double(Card_Shoe &shoe, int hand_num){
    (void)Hit(shoe, hand_num);
    wager += wager/valid_hands;
}

void Player::Win(){
    credit += wager*2;
}

int Player::Hand_Total(int hand_num){
    return hands[hand_num].total;
}

const vector<Card>& Player::Cards(int hand_num){
    return hands[hand_num].cards;
}

int Player::Hands_In_Play(){
    return valid_hands;
}

class Dealer{
   private:
    vector<Card> hand;
    int total;
    bool Ace;

    void Hit(Card_Shoe &shoe);

   public:
    void Deal_In(Card_Shoe &shoe);
    int Call(Card_Shoe &shoe); // returns the dealer's total hand
    Card Face_Card();
    const vector<Card>& Cards();
    Dealer();
    ~Dealer();
};

Dealer::Dealer(){
    //nothing
}

Dealer::~Dealer(){
    hand.clear();
}

void Dealer::Hit(Card_Shoe &shoe){
    Card draw = shoe.Deal();
    if(draw.value == Ace){
        Ace = true;
    }
    if(draw.value > 9){
        total += 10;
    }else{
        total += draw.value + 1;
    }
    hand.push_back(draw);
    return;
}

void Dealer::Deal_In(Card_Shoe &shoe){
    hand.clear();
    total = 0;
    Ace = false;
    Hit(shoe);
    Hit(shoe);
}

int Dealer::Call(Card_Shoe &shoe){
    while(total < 17){
        if(Ace && total > 6 && total < 12){
            return total + 10;
        }
        Hit(shoe);
    }
    return total;
}

Card Dealer::Face_Card(){
    return hand.front();
}

const vector<Card>& Dealer::Cards(){
    return hand;
}

#endif