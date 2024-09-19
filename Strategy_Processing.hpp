#ifndef STRATEGY_PROCESSING
#define STRATEGY_PROCESSING

#include "Blackjack.hpp"
#include <thread>
// NOTES: Best Move is computed based off the pool counting the deal
//        Weight is computed based off of a hypothetical previous pool

class Computed_Strategy_Chart{
    private:
    Blackjack BJ;
    vector<thread> workers;
    double Compute_Weight(const Absent_Map &pool, const Hand &current, int dealer_card);

    public:
    // [Dealer_Card][Hard_Total]
    W_Move hard_chart[15][10];
    // [Dealer_Card][Soft_Total]
    W_Move soft_chart[9][10];
    // [Dealer_Card][Split_Cards]
    W_Move split_chart[10][10];

    double EV;


    void Configure(const Absent_Map &pool);
    void Print_Hard();
    void Print_Soft();
    void Print_Split();
    void Print_All();

    Computed_Strategy_Chart();
    ~Computed_Strategy_Chart();
};

#endif