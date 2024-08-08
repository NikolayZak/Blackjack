#ifndef BLACKJACK
#define BLACKJACK
// blackjack setup
// Nikolay Zakirov, 2024-08-05

#define MAX_SPLIT 2
#define DOUBLE_ON_SPLIT false
#define MINIMUM_WINNING_ODDS 0.55

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cmath>

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
    bool Half();
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

bool Card_Shoe::Half(){
    if(shoe_index + 1 > (int)deck.size() / 2){
        return true;
    }
    return false;
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

class Hand{
    public:
    vector<Card> cards;
    int total;
    bool Ace;
    bool natural_blackjack;
    void Clear();
    Hand();
    ~Hand();
};

Hand::Hand(){
    total = 0;
    Ace = false;
    natural_blackjack = false;
}

Hand::~Hand(){
    cards.clear();
}

void Hand::Clear(){
    total = 0;
    Ace = false;
    natural_blackjack = false;
    cards.clear();
}

class Player{
   private:
    Hand hands[MAX_SPLIT];
    int valid_hands;
    int credit;
    int wager;
    int lowest_balance;
    
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
    valid_hands = 1;
    hands[0].Clear();
    wager = money;
    credit -= money;
    Hit(shoe, 0);
    Hit(shoe, 0);
    if(hands[0].Ace && hands[0].total == 11){
        credit += wager*2.5;
        hands[0].natural_blackjack = true;
        return;
    }
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
    int Call(Card_Shoe &shoe);
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
    if(draw.value > 9){
        hand.total += 10;
    }else{
        hand.total += draw.value + 1;
    }
    hand.cards.push_back(draw);
    return;
}

void Dealer::Deal_In(Card_Shoe &shoe){
    hand.Clear();
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

Card Dealer::Dealer_Card(){
    return hand.cards.front();
}

const Hand& Dealer::Cards(){
    return hand;
}

void Dealer::Clear(){
    hand.Clear();
}

struct Simulation_Results{
    int House_Balance;
    int player_wins;
    int player_loses;
    int player_splits;
    int player_doubles;
};

class Blackjack{
   private:
    vector<Player> players;
    Dealer Tom;
    Card_Shoe Shoe;
    Absent_Map Remaining_Cards;
    int House_Earnings;

    double Bust_Chance(Hand my_hand);
    double Risk_Of_Ruin(double winRate, double lossRate, double averageWin, double averageLoss, double maxRiskPercent, double tradingCapital);
    double Win_Chance();
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

double Blackjack::Risk_Of_Ruin(double winRate, double lossRate, double averageWin, double averageLoss, double maxRiskPercent, double tradingCapital) {
    double riskOfRuin = pow(lossRate / winRate, tradingCapital / (averageLoss * maxRiskPercent));
    return riskOfRuin;
}

// premeditated Win_Chance
double Blackjack::Win_Chance(){

}

double Blackjack::Natural_Blackjack_Chance(){

}

// splits if needed and splits as many times as possible
void Blackjack::Check_Split(int player_num){

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
        Remaining_Cards.Clear();
    }

    // check odds, bet accordingly, deal in each player and the dealer
    for(int i = 0; i < (int)players.size(); i++){
        double P_win = Win_Chance();
        double best_wager;
        if(P_win >= MINIMUM_WINNING_ODDS){
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
    ans.House_Balance = 0;
    ans.player_wins = 0;
    ans.player_loses = 0;
    ans.player_splits = 0;
    ans.player_doubles = 0;
    players.clear();
    Tom.Clear();
    Shoe.Reshuffle();
    Remaining_Cards.Clear();

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