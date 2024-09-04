#ifndef HASHER
#define HASHER

#include "Library.hpp"

// compresses the pool by x: if x is 2 each card number in the pool is divided by 2, this rounds information and increases hash hit rate
Absent_Map Compressed_Map(const Absent_Map &pool, int compression_factor);

// writes to the Hash
void Write_Hash(const Absent_Map &pool, const Hand &current, int dealer_card, char EV_name);

// returns -10 if Hash not found else returns the EV
double Read_Hash(const Absent_Map &pool, const Hand &current, int dealer_card, char EV_name);


#endif
