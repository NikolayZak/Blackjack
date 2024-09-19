#include "Library.hpp"

// ******************************************************   Hand Method   ******************************************************
void Hand::Add(int card){
    switch(card){
    case 1:
        cards++;
        break;
    case 2:
        cards += 0x20;
        break;
    case 3:
        cards += 0x200;
        break;
    case 4:
        cards += 0x2000;
        break;
    case 5:
        cards += 0x10000;
        break;
    case 6:
        cards += 0x80000;
        break;
    case 7:
        cards += 0x400000;
        break;
    case 8:
        cards += 0x2000000;
        break;
    case 9:
        cards += 0x8000000;
        break;
    case 10:
        cards += 0x20000000;
        break;
    }
}


int Hand::High_Total() const {
    int counter = 0;
    int mask;
    // case 10's
    mask = 0x60000000;
    counter += ((cards & mask) >> 29) * 10;
    // case 9's
    mask = 0x18000000;
    counter += ((cards & mask) >> 27) * 9;
    // case 8's
    mask = 0x6000000;
    counter += ((cards & mask) >> 25) * 8;
    // case 7's
    mask = 0x1C00000;
    counter += ((cards & mask) >> 22) * 7;
    // case 6's
    mask = 0x380000;
    counter += ((cards & mask) >> 19) * 6;
    // case 5's
    mask = 0x70000;
    counter += ((cards & mask) >> 16) * 5;
    // case 4's
    mask = 0xE000;
    counter += ((cards & mask) >> 13) * 4;
    // case 3's
    mask = 0x1E00;
    counter += ((cards & mask) >> 9) * 3;
    // case 2's
    mask = 0x1E0;
    counter += ((cards & mask) >> 5) * 2;
    // case 1's
    mask = 0x1F;
    int tmp = (cards & mask);
    counter += tmp;
    if(tmp > 0 && counter < 12){
        counter += 10;
    }
    return counter;
}

bool Hand::Can_Split() const {
    // case 10's
    if(cards == 0x40000000){
        return true;
    }
    // case 9's
    if(cards == 0x10000000){
        return true;
    }
    // case 8's
    if(cards == 0x4000000){
        return true;
    }
    // case 7's
    if(cards == 0x800000){
        return true;
    }
    // case 6's
    if(cards == 0x100000){
        return true;
    }
    // case 5's
    if(cards == 0x20000){
        return true;
    }
    // case 4's
    if(cards == 0x4000){
        return true;
    }
    // case 3's
    if(cards == 0x400){
        return true;
    }
    // case 2's
    if(cards == 0x40){
        return true;
    }
    // case 1's
    if(cards == 0x2){
        return true;
    }
    return false;
}

// checks for an
bool Hand::Ace() const{
    return (cards & 0x1F) != 0;
}

void Hand::Clear(){
    cards = 0;
}

void Hand::Remove(int card){
    switch(card){
    case 1:
        cards--;
        break;
    case 2:
        cards -= 0x20;
        break;
    case 3:
        cards -= 0x200;
        break;
    case 4:
        cards -= 0x2000;
        break;
    case 5:
        cards -= 0x10000;
        break;
    case 6:
        cards -= 0x80000;
        break;
    case 7:
        cards -= 0x400000;
        break;
    case 8:
        cards -= 0x2000000;
        break;
    case 9:
        cards -= 0x8000000;
        break;
    case 10:
        cards -= 0x20000000;
        break;
    }
}

// Precondition: 2 of the same card gets turned into 1 of that card
void Hand::Split(){
    cards = cards >> 1;
}

Hand::Hand(){
    cards = 0;
}

Hand::~Hand(){
    // nothing
}
