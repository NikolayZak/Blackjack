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
    Move hard_chart[10][13];
    Move split_chart[10][10];
    Move soft_chart[10][8];

};

#endif