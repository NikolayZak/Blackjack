#include "Blackjack.hpp"

// ************************************************   DATABASE METHODS   ****************************************************************
Blackjack::Blackjack(){
    if (!openDatabase()) {
        std::cerr << "Failed to open database." << std::endl;
    }
    createEVTable();
}

Blackjack::~Blackjack(){
    closeDatabase();
}

// Blob Layout: dealer_hand_total 0-4 | my_hand_total 5-9 | my_soft 10 | name 11-12
short Blackjack::Move_Key(char name, const Hand &my_hand, int dealer_card){
    short ans = 0;

    //Split is hashed via double and hit methods
    switch(name){
    case 'S':
        ans |= 0x0;
        break;
    case 'H':
        ans |= 0x1;
        break;
    case 'D':
        ans |= 0x2;
        break;
    default:
        std::cerr << "Name not recognized " << std::endl;
        break;
    }

    // my_soft
    ans = ans << 1;
    if(name != 'S' && my_hand.Ace){ // if it's a stand it is the same as a hard
        ans |= 0x1;
    }

    // my_hand_total
    ans = ans << 5;
    ans |= short(my_hand.High_Total() & 0x1F);

    // dealer_card
    ans = ans << 5;
    ans |= short(dealer_card & 0x1F);

    return ans;
}

bool Blackjack::openDatabase() {
    int rc = sqlite3_open(DATABASE, &db);
    return rc == SQLITE_OK;
}

void Blackjack::closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

void Blackjack::createEVTable() {
    const char* createTableSQL = 
        "CREATE TABLE IF NOT EXISTS EVTable ("
        "move_key INTEGER, "
        "pool_key INTEGER, "
        "EV REAL, "
        "PRIMARY KEY (move_key, pool_key)"
        ");";
    
    char* errorMessage = 0;
    int rc = sqlite3_exec(db, createTableSQL, 0, 0, &errorMessage);
    if (rc != SQLITE_OK) {
        std::cerr << "Error creating table: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
    }
}


bool Blackjack::getEVIfExists(short move_key, uint64_t pool_key, double& ev) {
    const char* selectSQL = "SELECT EV FROM EVTable WHERE move_key = ? AND pool_key = ?;";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        // Bind the 16-bit move_key
        sqlite3_bind_int(stmt, 1, move_key);
        // Bind the 64-bit pool_key
        sqlite3_bind_int64(stmt, 2, pool_key);

        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            ev = sqlite3_column_double(stmt, 0);  // Retrieve the EV value
            sqlite3_finalize(stmt);
            return true;  // Key exists
        }
    }
    sqlite3_finalize(stmt);
    return false;  // Key does not exist
}

void Blackjack::insertEV(short move_key, uint64_t pool_key, double ev) {
    const char* insertSQL = "INSERT INTO EVTable (move_key, pool_key, EV) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        // Bind the 16-bit move_key
        sqlite3_bind_int(stmt, 1, move_key);
        // Bind the 64-bit pool_key
        sqlite3_bind_int64(stmt, 2, pool_key);
        // Bind the EV value
        sqlite3_bind_double(stmt, 3, ev);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to insert data: " << sqlite3_errmsg(db) << std::endl;
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    }
}

// ****************************************************************************************************************************

double Blackjack::BJ_EV(Absent_Map &pool){
    return pool.Probability(10) * pool.Probability(1) * 5; // blackjack pays 2.5, there are 2 ways to draw 2 cards
}

// technical debt : reduce permutation counting : theoretically speed up = 10x
void Blackjack::Stand_Rec(Absent_Map pool, int my_total, Hand dealer_hand, double multiplier, double &EV){
    // assuming the player_total is less then 22
    // base case: Dealer has 17 or more: check for a player win
    int dealer_total = dealer_hand.High_Total();
    if(dealer_total > 16){                                // Dealer has a hand
        if(dealer_total > 21 || dealer_total < my_total){ // Dealer Lose: Bust || Dealer Short
            EV += multiplier * 2.0; // wins pay 2 times
        }else if(dealer_total == my_total){
            EV += multiplier; // push
        }
        // loss is assumed in EV = -1
        return;
    } // implied else (Dealer needs to pickup)

    int card_dups;
    double original_prob = multiplier; // save state
    for(int i = 1; i < 11; i++){ // go through the map
        card_dups = pool.Count(i);
        if(card_dups == 0){
            continue; // no copies
        }
        
        // iterate 1 hand
        multiplier *= pool.Probability(i);
        dealer_hand.Add(i);
        pool.Add(i);
        Stand_Rec(pool, my_total, dealer_hand, multiplier, EV);
        dealer_hand.Remove_Last();
        pool.Remove(i);
        multiplier = original_prob;
    }
}

// todo: make this call Stand_EV not Stand_Rec
double Blackjack::Dealer_Ace_Exception(Absent_Map pool, int my_total, Hand dealer_hand){
    int card_dups;
    double multy;
    double ans = -1.0;
    for(int i = 1; i < 10; i++){ // go through the map except 10
        card_dups = pool.Count(i);
        if(card_dups == 0){
            continue; // no copies
        }
        
        // iterate 1 hand
        multy = (double)pool.Count(i) / (double)(pool.Cards_Left() - pool.Count(10));
        dealer_hand.Add(i);
        pool.Add(i);
        Stand_Rec(pool, my_total, dealer_hand, multy, ans);
        dealer_hand.Remove_Last();
        pool.Remove(i);
    }
    return ans;
}

// returns the expected value of a stand in this position
// technical debt : Add hashing
double Blackjack::Stand_EV(const Absent_Map &pool, const Hand &current, int dealer_card){
    double ans = -1.0;

    if(current.High_Total() > 21){
        return ans;
    }

    // check hashed
    short move_key = Move_Key('S', current, dealer_card);
    uint64_t pool_key = pool.Map_Key();
    if(getEVIfExists(move_key, pool_key, ans)){
        return ans;
    }

    Hand D_Hand;
    D_Hand.Add(dealer_card);
    // dealer exception
    if(dealer_card == 1){
        return Dealer_Ace_Exception(pool, current.High_Total(), D_Hand);
    }
    Stand_Rec(pool, current.High_Total(), D_Hand, 1.0, ans);

    // hash
    insertEV(move_key, pool_key, ans);
    return ans;
}

double Blackjack::Hit_Rec(Absent_Map pool, Hand my_hand, int dealer_card, double multiplier){
    // base case my_hand > 21
    if(my_hand.High_Total() > 21){
        return -1.0;
    }

    // compute Stand EV and Hit EV
    double hit_ev = 0.0;
    double stand_ev = Stand_EV(pool, my_hand, dealer_card) * multiplier;
    double original_prob = multiplier; // save state
    int card_dups;
    for(int i = 1; i < 11; i++){
        card_dups = pool.Count(i);
        if(card_dups == 0){
            continue; // no copies
        }
        multiplier *= pool.Probability(i);
        my_hand.Add(i);
        pool.Add(i);
        hit_ev += Hit_Rec(pool, my_hand, dealer_card, multiplier) * multiplier;
        my_hand.Remove_Last();
        pool.Remove(i);
        multiplier = original_prob;
    }

    if(stand_ev > hit_ev){
        return stand_ev;
    }
    return hit_ev;
}

double Blackjack::Hit_EV(const Absent_Map &pool, const Hand &current, int dealer_card){
    Absent_Map map = pool;
    Hand my_hand = current;
    double ans = 0.0;
    double card_prob;
    int card_dups;

    // check hashed
    short move_key = Move_Key('H', current, dealer_card);
    uint64_t pool_key = pool.Map_Key();
    if(getEVIfExists(move_key, pool_key, ans)){
        return ans;
    }

    for(int i = 1; i < 11; i++){
        card_dups = map.Count(i);
        if(card_dups == 0){
            continue; // no copies
        }
        card_prob = map.Probability(i);
        my_hand.Add(i);
        map.Add(i);
        ans += Hit_Rec(map, my_hand, dealer_card, 1.0) * card_prob;
        my_hand.Remove_Last();
        map.Remove(i);
    }

    // hash
    insertEV(move_key, pool_key, ans);

    return ans;
}

double Blackjack::Double_EV(const Absent_Map &pool, const Hand &current, int dealer_card){
    Absent_Map map = pool;
    Hand my_hand = current;
    double ans = 0.0;
    double card_prob;
    int card_dups;

    // check hashed
    short move_key = Move_Key('D', current, dealer_card);
    uint64_t pool_key = pool.Map_Key();
    if(getEVIfExists(move_key, pool_key, ans)){
        return ans;
    }

    for(int i = 1; i < 11; i++){
        card_dups = map.Count(i);
        if(card_dups == 0){
            continue; // no copies
        }
        card_prob = map.Probability(i);
        my_hand.Add(i);
        map.Add(i);
        ans += Stand_EV(map, my_hand, dealer_card) * card_prob;
        my_hand.Remove_Last();
        map.Remove(i);
    }

    // hash
    insertEV(move_key, pool_key, ans * 2);

    return ans * 2;
}

// assuming the hand can be split
double Blackjack::Split_EV(const Absent_Map &pool, const Hand &current, int dealer_card){
    Hand my_hand = current;
    my_hand.Remove_Last();

    if(my_hand.Ace){ // only 1 card when splitting aces
        return Double_EV(pool, my_hand, dealer_card);
    }

    return Hit_EV(pool, my_hand, dealer_card) * 2;
}

void Blackjack::Print_Stats(const Absent_Map &pool, const Hand &current, int dealer_card){
    double S = Stand_EV(pool, current, dealer_card);
    double H = Hit_EV(pool, current, dealer_card);
    double D = Double_EV(pool, current, dealer_card);
    cout << "Stand EV: " << S << endl;
    cout << "Hit EV: " << H << endl;
    cout << "Double EV: " << D << endl;

    if(current.Can_Split()){
        double P = Split_EV(pool, current, dealer_card);
        cout << "Split EV: " << P << endl;
    }
}

Move Blackjack::Best_Move(const Absent_Map &pool, const Hand &current, int dealer_card){
    Move ans;   // compute values
    double S = Stand_EV(pool, current, dealer_card);
    double H = Hit_EV(pool, current, dealer_card);
    double D = Double_EV(pool, current, dealer_card);
    double P = -10.0;
    if(current.Can_Split()){
        P = Split_EV(pool, current, dealer_card);
    }
    // find max
    ans.EV = S;
    ans.name = 'S';
    if(H > ans.EV){
        ans.EV = H;
        ans.name = 'H';
    }
    if(D > ans.EV){
        ans.EV = D;
        ans.name = 'D';
    }
    if(P > ans.EV){
        ans.EV = P;
        ans.name = 'P';
    }
    return ans;
}
