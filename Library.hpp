#ifndef LIBRARY
#define LIBRARY

#include "Card_Shoe.hpp"

struct Hand {
    vector<int> cards;
    bool Ace;

    void Add(int card);
    int High_Total() const;
    bool Can_Split() const;
    void Clear();
    void Remove_Last();
    Hand();
    ~Hand();
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
    double time;
    int bet_size;
    Simulation_Results();
    void Print();
};

struct EV_Results{
    double win_P;
    double loss_P;
    double push_P;
    double prob_hand;
    EV_Results();
};

#endif