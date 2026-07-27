export module grid;

import <string>;
import <iostream>;
import <fstream>;
import <vector>;
import <random>;
import <memory>;

import character;

using namespace std;

export class DragonHoard {
    public:
        int row;
        int col;
        Enemy *guardian = nullptr;


export struct Position {
    int row;
    int col;
    int chamber = -1;
};

export class Grid {
    private:
    
        string Map[25];
        int chamber_count = 1;
        vector<int> chamberSize;
        vector<vector<int>> inchamber;
        vector<vector<Position>> chamberTiles;
        bool isFull(int c) const;
        int inchamber_fill(int i, int j, int cno);
        bool isFloor(char c);
    public:
        Grid(const string CreateMap[25]);
        void set_position(int row, int col, char c);
        bool move(int prev_row, int prev_col, int cur_row, int cur_col, char& currStepOn);
        char get_position(int row, int col) const;
        Position generatePoint(int stairChamber = -1) const;
        void generateDragonPair(vector<unique_ptr<Enemy>>& enemies, vector<DragonHoard>& h);

};

export ostream& operator<<(ostream &out, const Grid &g);

export void parse(string(&map)[25], vector<unique_ptr<Enemy>>& enemies, Player& pc, vector<DragonHoard>& h, ifstream& f);

export void create(Grid& g, vector<unique_ptr<Enemy>>& enemies, Player& pc, vector<DragonHoard>& h);


