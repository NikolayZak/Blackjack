#ifndef LIBRARY
#define LIBRARY

#include "Card_Shoe.hpp"

struct Hand {
    vector<int> cards; // technical debt : use a custom vector with purpose built methods
    bool Ace;

    void Add(int card);
    int High_Total() const;
    bool Can_Split() const;
    void Clear();
    void Remove_Last();
    double Half_Permutation() const;
    Hand();
    ~Hand();
};

struct Player{
    Hand hands[2];
    int credit;
    int wager;
    bool split;
    bool blackjack;
    
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
    double EV;
    char name;
};

struct Simulation_Results{
    double time;
    int rounds_played;
    int player_balance;
    int player_pushes;
    int player_wins;
    int player_losses;
    int player_splits;
    int player_doubles;
    int player_blackjack;
    int bet_size;
    Simulation_Results();
    void Print();
};

struct EV_Results{
    double win_P;
    double loss_P;
    double push_P;
    double current_hand_P;
    EV_Results();
};

struct Hashed_Query{
    Absent_Map map;
    int total;
    Hashed_Query(const Absent_Map &map, int high_total);
};

#endif