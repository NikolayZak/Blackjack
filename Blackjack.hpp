#ifndef BLACKJACK
#define BLACKJACK
// blackjack setup
// Nikolay Zakirov, 2024-08-05
#include "Library.hpp"
#include "sqlite3.h"

#define DATABASE "BlackjackDB.db"


class Blackjack{
    private:
    // database for storing hashed values
    sqlite3* db;
    bool openDatabase();
    void closeDatabase();
    // Blob Layout: name 0-1 | soft 2 | my_hand_total 3-7 | pool 8-63
    short Move_Key(char name, const Hand &my_hand, int dealer_card);

    // dealer_hand layout: total 0-6 | soft 7
    bool getEVIfExists(short move_key, uint64_t pool_key, double& ev);
    void insertEV(short move_key, uint64_t pool_key, double ev);

    void Stand_Rec(Absent_Map pool, int my_total, Hand dealer_hand, double multiplier, double &ans);
    double Hit_Rec(Absent_Map pool, Hand my_hand, int dealer_card, double multiplier);
    double Dealer_Ace_Exception(Absent_Map pool, int my_total, Hand dealer_hand);

    public:
    Blackjack();
    ~Blackjack();


    double BJ_EV(Absent_Map &pool);
    double Stand_EV(const Absent_Map &pool, const Hand &current, int dealer_card);
    double Hit_EV(const Absent_Map &pool, const Hand &current, int dealer_card);
    double Double_EV(const Absent_Map &pool, const Hand &current, int dealer_card);
    double Split_EV(const Absent_Map &pool, const Hand &current, int dealer_card);
    Move Best_Move(const Absent_Map &pool, const Hand &current, int dealer_card);
    void Print_Stats(const Absent_Map &pool,const Hand &current, int dealer_card);
};




#endif