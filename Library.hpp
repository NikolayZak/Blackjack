#ifndef LIBRARY
#define LIBRARY

#include "Card_Shoe.hpp"

struct Hand {
    int cards;

    void Add(int card);
    int High_Total() const;
    bool Can_Split() const;
    bool Ace() const;
    void Clear();
    void Remove(int card);
    void Split();
    Hand();
    ~Hand();
};
/*
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
*/
struct Move{
    double EV;
    char name;
    void Print();
};

struct W_Move : Move{
    double weight;
    W_Move() : Move(), weight(0.0) {}
    W_Move(const Move& move, double w) : Move(move), weight(w) {}
};

struct Hashed_Query{
    Absent_Map map;
    int total;
    Hashed_Query(const Absent_Map &map, int high_total);
};


#endif