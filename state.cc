export module state;

import <vector>;
import <utility>;

using namespace std;

export enum class EnemyT {Human, Dwarf, Halfling, Elf, Orc, Merchant, Dragon};
export enum class PlayerT {Shade, Drow, Vampire, Troll, Goblin};

export struct Pairing {
    int hoardRow;
    int hoardCol;
    int dragonRow;
    int dragonCol;
};

export struct EnemyInfo {
    vector<Pairing> hoDrag;
    vector<pair<EnemyT, pair<int,int>>> EnemyPose;
    pair<int,int> PlayerPos;
};


// Potion
// 0 - RH
// 1 - BA
// 2 - BD
// 3 - PH
// 4 - WA
// 5 - WD

