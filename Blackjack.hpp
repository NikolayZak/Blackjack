#ifndef BLACKJACK
#define BLACKJACK
// blackjack setup
// Nikolay Zakirov, 2024-08-05
#include "Library.hpp"


class Blackjack{
    private:
    // shhh private stuff
    void Stand_Rec(Absent_Map pool, int my_total, Hand dealer_hand, double multiplier, double &ans);
    double Hit_Rec(Absent_Map pool, Hand my_hand, int dealer_card, double multiplier);
    double Dealer_Ace_Exception(Absent_Map pool, int my_total, Hand dealer_hand);

    public:
    double BJ_EV(Absent_Map &pool);
    double Stand_EV(const Absent_Map &pool, const Hand &current, int dealer_card);
    double Hit_EV(const Absent_Map &pool, const Hand &current, int dealer_card);
    double Double_EV(const Absent_Map &pool, const Hand &current, int dealer_card);
    double Split_EV(const Absent_Map &pool, const Hand &current, int dealer_card);
    Move Best_Move(const Absent_Map &pool, const Hand &current, int dealer_card);
    void Print_Stats(const Absent_Map &pool,const Hand &current, int dealer_card);
};




#endif