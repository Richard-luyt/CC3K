export module grid;

import string;
import iostream;
import fstream;
import vector;
import random;
import memory;

import player;
import enemy;

using namespace std;

export class DragonHoard {
    public:
        int row;
        int col;
        Enemy *guardian = nullptr; //this points to the dragon that looks after it
};

export class Chamber {
    public:
        int size;
        int minRow;
        int maxRow;
        vector<int> minCol;
        vector<int> maxCol;
};

export struct Position {
    int row;
    int col;
};

export class Grid {
    private:
        string Map[25];
        Chamber Chambers[5];
        bool isFull(const Chamber& c) const;
    public:
        Grid(const string CreateMap[25]);
        void set_position(int row, int col, char c);
        bool move(int prev_row, int prev_col, int cur_row, int cur_col, char& currStepOn);
        char get_position(int row, int col) const;
        Position generatePoint() const; //(Option 1)
        // pair<int, int> generatePoint();  (Option 2)
        void generateDragonPair(vector<unique_ptr<Enemy>>& enemies, vector<DragonHoard>& h);

        friend ostream& operator<<(ostream &out, const Grid &g);
        friend void create(Grid& g, vector<unique_ptr<Enemy>>& enemies, Player& pc, vector<DragonHoard>& h);

};

export ostream& operator<<(ostream &out, const Grid &g);

export void parse(string(&map)[25], vector<unique_ptr<Enemy>>& enemies, Player& pc, vector<DragonHoard>& h, ifstream& f);

export void create(Grid& g, vector<unique_ptr<Enemy>>& enemies, Player& pc, vector<DragonHoard>& h);

//To be added in the utilities.cc file
export bool compare(const Enemy *A, const Enemy *B);
