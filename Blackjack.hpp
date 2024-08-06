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
#include <unordered_map>

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

const int Card_Value_Int[] = {
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10
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
    bool natural_blackjack;
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
    void Hit(Card_Shoe &shoe, int hand_num); // returns false on bust
    void Deal_In(Card_Shoe &shoe, int money); // pays out 2.5x on natural blackjack
    void Split(Card_Shoe &shoe, int hand_num);
    void Double(Card_Shoe &shoe, int hand_num);
    void Win();
    int Hand_Total(int hand_num);
    const Hand& Cards(int hand_num);
    int Hands_In_Play();
    int Balance();
};

Player::Player(int money){
    credit = money;
}

Player::~Player(){
    for(int i = 0; i < MAX_SPLIT; i++){
        hands[i].cards.clear();
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
    valid_hands = 1;
    hands[0].Ace = false;
    hands[0].total = 0;
    hands[0].cards.clear();
    wager = money;
    credit -= money;
    Hit(shoe, 0);
    Hit(shoe, 0);
    if(hands[0].Ace && hands[0].total == 11){
        credit += wager*2.5;
        hands[0].natural_blackjack = true;
        return;
    }
    hands[0].natural_blackjack = false;
}

void Player::Split(Card_Shoe &shoe, int hand_num){
    Card descendant = hands[hand_num].cards.back();
    hands[hand_num].cards.pop_back();
    hands[hand_num + 1].cards.push_back(descendant);
    Hit(shoe, hand_num);
    Hit(shoe, hand_num + 1);
    hands[hand_num].natural_blackjack = false;
    hands[hand_num + 1].natural_blackjack = false;
    wager += wager/valid_hands;
    valid_hands++;
}

void Player::Double(Card_Shoe &shoe, int hand_num){
    Hit(shoe, hand_num);
    wager += wager/valid_hands;
}

void Player::Win(){
    credit += wager*2;
}

int Player::Hand_Total(int hand_num){
    return hands[hand_num].total;
}

const Hand& Player::Cards(int hand_num){
    return hands[hand_num];
}

int Player::Hands_In_Play(){
    return valid_hands;
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
    int Call(Card_Shoe &shoe); // returns the dealer's total hand
    Card Face_Card();
    const Hand& Cards();
    Dealer();
    ~Dealer();
};

Dealer::Dealer(){
    //nothing
}

Dealer::~Dealer(){
    hand.cards.clear();
}

void Dealer::Hit(Card_Shoe &shoe){
    Card draw = shoe.Deal();
    if(draw.value == Ace){
        hand.Ace = true;
    }
    if(draw.value > 9){
        hand.total += 10;
    }else{
        hand.total += draw.value + 1;
    }
    hand.cards.push_back(draw);
    return;
}

void Dealer::Deal_In(Card_Shoe &shoe){
    hand.cards.clear();
    hand.total = 0;
    hand.Ace = false;
    Hit(shoe);
    Hit(shoe);
}

int Dealer::Call(Card_Shoe &shoe){
    while(hand.total < 17){
        if(hand.Ace && hand.total > 6 && hand.total < 12){
            return hand.total + 10;
        }
        Hit(shoe);
    }
    return hand.total;
}

Card Dealer::Face_Card(){
    return hand.cards.front();
}

const Hand& Dealer::Cards(){
    return hand;
}

class Absent_Map{
   private:
    int drawn_cards[13];
    int cards;
    int decks;
    int duplicates;

   public:
    void Add(Card_Value drawn);
    void Clear();
    double Probability(Card_Value Theta);
    Absent_Map(int number_of_decks);
    ~Absent_Map();
};

Absent_Map::Absent_Map(int number_of_decks){
    decks = number_of_decks;
    cards = number_of_decks * 52;
    duplicates = number_of_decks * 4;
    for(int i = 0; i < 13; i++){
        drawn_cards[i] = duplicates;
    }
}

Absent_Map::~Absent_Map(){
    //nothing
}

void Absent_Map::Add(Card_Value drawn){
    drawn_cards[drawn]++;
    cards--;
}

void Absent_Map::Clear(){
    cards = decks * 52;
    for(int i = 0; i < 13; i++){
        drawn_cards[i] = duplicates;
    }
}

double Absent_Map::Probability(Card_Value Theta){
    return drawn_cards[Theta]/cards;
}

struct Player_Result{
    int wins;
    int loses;
};

struct Simulation_Results{
    int House_Balance;
    int wins;
    int loses;
    int splits;
    int doubles;
};

class Blackjack{
   private:
    vector<Player> players;
    Dealer Tom;
    Card_Shoe Shoe;
    Absent_Map Remaining_Cards;
    vector<Player_Result> Game_Result;
    int House_Earnings;

    double Bust_Chance(Hand my_hand);

   public:
    Simulation_Results Play(int games, int players, int money);
};

double Blackjack::Bust_Chance(Hand my_hand){
    if(my_hand.total > 13){// check for odds of not busting and invert it
        double safe_chance = 0;
        Card_Value test_card = Ace;
        while(Card_Value_Int[test_card] + my_hand.total < 22){
            safe_chance += Remaining_Cards.Probability(test_card);
            test_card = (Card_Value)((int)test_card + 1);
        }
        return 1 - safe_chance;
    }else{// check for odds of busting
        double bust_chance = 0;
        Card_Value test_card = King;
        while(Card_Value_Int[test_card] + my_hand.total > 21){
            bust_chance += Remaining_Cards.Probability(test_card);
            test_card = (Card_Value)((int)test_card - 1);
        }
        return bust_chance;
    }
}

Simulation_Results Blackjack::Play(int games, int players, int money){
    Simulation_Results ans;
    ans.House_Balance = 0;
    ans.wins = 0;
    ans.loses = 0;
    ans.splits = 0;
    ans.doubles = 0;

}

#endif