#ifndef CARD_SHOE
#define CARD_SHOE

#include "Absent_Map.hpp"

class Card_Shoe{
   private:
    vector<int> deck;
    int shoe_index;

   public:
    void Reshuffle();
    void Permutate();
    int Deal();
    bool Half();
    void New_Shoe(const Absent_Map &map);
    Card_Shoe(const int number_of_decks);
    ~Card_Shoe();
};

#endif