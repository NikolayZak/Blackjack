#ifndef ABSENT_MAP
#define ABSENT_MAP

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <string>
#include <cmath>
#include <chrono>

using namespace std;

class Absent_Map{
    private:
    int remaining_cards[10];
    int cards;
    int decks;
    int duplicates;

    public:
    void Add(int drawn);
    void Remove(int Theta);
    void Clear();
    double Probability(int Theta) const;
    double Probability(int first, int second, int dealer) const;
    int Count(int Theta) const;
    int Cards_Left() const;
    uint64_t Map_Key() const;
    bool operator==(const Absent_Map &other) const;
    void Print() const;
    Absent_Map(int number_of_decks);
    Absent_Map(const Absent_Map &a_map);
    ~Absent_Map();
};


#endif