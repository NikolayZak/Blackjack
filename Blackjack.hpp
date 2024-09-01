#ifndef BLACKJACK
#define BLACKJACK
// blackjack setup
// Nikolay Zakirov, 2024-08-05
#include "Library.hpp"

// EV(Game) = P(Hand[i]) * P(D_Card[j]) * Max_EV(Hand[i], D_Card[j]) + P(Blackjack) * 2.5
class Blackjack{
    private:
    // shhh private stuff
    void Stand_Rec(Absent_Map pool, int my_total, Hand dealer_hand, double multiplier, double &ans);

    public:
    double BJ_EV(Absent_Map &pool);
    double Stand_EV(const Absent_Map &pool, const Hand &current, int dealer_card);
    double Hit_EV(const Absent_Map &pool, const Hand &current, int dealer_card);
};




#endif