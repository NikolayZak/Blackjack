#ifndef STRATEGY_PROCESSING
#define STRATEGY_PROCESSING

#include "Blackjack.hpp"

// NOTES: Best Move is computed based off the pool counting the deal
//        Weight is computed based off of a hypothetical previous pool

//                         Dealer Upcard
//  Hard Total| 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |
//      5     | H | H | H | H | H | H | H | H | H  | H |
//      6     | H | H | H | H | H | H | H | H | H  | H |
//      7     | H | H | H | H | H | H | H | H | H  | H |
//      8     | H | H | H | H | H | H | H | H | H  | H |
//      9     | H | D | D | D | D | H | H | H | H  | H |
//      10    | D | D | D | D | D | D | D | D | H  | H |
//      11    | D | D | D | D | D | D | D | D | D  | D |
//      12    | H | H | S | S | S | H | H | H | H  | H |
//      13    | S | S | S | S | S | H | H | H | H  | H |
//      14    | S | S | S | S | S | H | H | H | H  | H |
//      15    | S | S | S | S | S | H | H | H | H  | H |
//      16    | S | S | S | S | S | H | H | H | H  | H |
//      17    | S | S | S | S | S | S | S | S | S  | S |


//                         Dealer Upcard
//  Soft Total| 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |
//      13    | H | H | H | D | D | H | H | H | H  | H |
//      14    | H | H | H | D | D | H | H | H | H  | H |
//      15    | H | H | D | D | D | H | H | H | H  | H |
//      16    | H | H | D | D | D | H | H | H | H  | H |
//      17    | H | D | D | D | D | H | H | H | H  | H |
//      18    | S | D | D | D | D | S | S | H | H  | H |
//      19    | S | S | S | S | S | S | S | S | S  | S |
//      20    | S | S | S | S | S | S | S | S | S  | S |


//                         Dealer Upcard
// Split Cards| 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | A |
//     2,2    | H | H | P | P | P | P | H | H | H  | H |
//     3,3    | H | H | P | P | P | P | H | H | H  | H |
//     4,4    | H | H | H | H | H | H | H | H | H  | H |
//     5,5    | D | D | D | D | D | D | D | D | H  | H |
//     6,6    | H | P | P | P | P | H | H | H | H  | H |
//     7,7    | P | P | P | P | P | P | H | H | H  | H |
//     8,8    | P | P | P | P | P | P | P | P | P  | P |
//     9,9    | P | P | P | P | P | S | P | P | S  | S |
//    10,10   | S | S | S | S | S | S | S | S | S  | S |
//     A,A    | P | P | P | P | P | P | P | P | P  | P |

class Computed_Strategy_Chart{
    private:
    Blackjack BJ;
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
    void Print_Hard_Weight();
    void Print_Soft_Weight();
    void Print_Split_Weight();
    void Print_All();

    Computed_Strategy_Chart();
    ~Computed_Strategy_Chart();
};

#endif