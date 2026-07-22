export module utility;

import state;
import character;
import grid;

import <iostream>;
import <string>;
import <utility>;
import <cstdlib>;
import <memory>;
import <vector>;

using namespace std;

//To be added in the utilities.cc file
export bool compare(const unique_ptr<Enemy> &A, const unique_ptr<Enemy> &B) {
    if(A->getRow() != B->getRow()) {
        return A->getRow() < B->getRow();
    }
    return A->getCol() < B->getCol();
}

export int playerAttack(Player &pc, Enemy &enemy) {
    return pc.damageDealt(enemy);
}

export int enemyAttack(Player &pc, Enemy &enemy) {
    return enemy.damageDealt(pc);
}

export bool verifyMove(const Grid &grid, int nextRow, int nextCol, bool player) {
    if (nextRow < 0 || nextRow >= 25 || nextCol < 0 || nextCol >= 79) {
        return false;
    }
    char next_pos = grid.get_position(nextRow, nextCol);
    if(player) {
        // player is currently moving
        if(next_pos == '.' || next_pos == '\\' || next_pos == '+' || next_pos == '#') {
            return true;
        }
        int treasure = next_pos - '0';
        if(treasure >= 6 && treasure <= 9) {
            return true;
        }
        return false;
    } else {
        // an enemy is currently moving
        if(next_pos != '.'){
            return false;
        }
        return true;
    }
}

export pair<int,int> nextMove(int row, int col, string direction) {
    pair<int,int> ans;
    ans.first = row;
    ans.second = col;
    if(direction == "no"){
        ans.first -= 1;
        return ans;
    }
    if(direction == "so"){
        ans.first += 1;
        return ans;
    }
    if(direction == "ea"){
        ans.second += 1;
        return ans;
    }
    if(direction == "we"){
        ans.second -= 1;
        return ans;
    }
    if(direction == "ne"){
        ans.first -= 1;
        ans.second += 1;
        return ans;
    }
    if(direction == "nw"){
        ans.first -= 1;
        ans.second -= 1;
        return ans;
    }
    if(direction == "se"){
        ans.first += 1;
        ans.second += 1;
        return ans;
    }
    if(direction == "sw"){
        ans.first += 1;
        ans.second -= 1;
        return ans;
    }
}

export bool validDirection(string direction) {
    if(direction == "no") return true;
    if(direction == "so") return true;
    if(direction == "ea") return true;
    if(direction == "we") return true;
    if(direction == "ne") return true;
    if(direction == "nw") return true;
    if(direction == "se") return true;
    if(direction == "sw") return true;
    return false;
}

export string Converter(string direction) {
    if(direction == "no"){
        return "North";
    }
    if(direction == "so"){
        return "South";
    }
    if(direction == "ea"){
        return "East";
    }
    if(direction == "we"){
        return "West";
    }
    if(direction == "ne"){
        return "North East";
    }
    if(direction == "nw"){
        return "North West";
    }
    if(direction == "se"){
        return "South East";
    }
    if(direction == "sw"){
        return "South West";
    }
}

export int UsePotion(Grid &game, Player &pc, int row, int col) {
    int type = game.get_position(row, col) - '0';
    pc.drinkPotion(type);
    game.set_position(row, col, '.');
    return type;
}

export bool IsNear(int ar, int ac, int br, int bc) {
    return abs(ar-br) <= 1 && abs(ac-bc) <= 1;
}

export void Display(const Grid &grid, const Player &pc, int floor, const string &action) {

    cout << "\033[H\033[2J" << flush; //used to clear screen up

    cout << grid;
    cout << endl;
    PlayerT type = pc.getType();
    int gold = pc.getGold();
    int HP = pc.getHp();
    int Atk = pc.getAtkClean();
    int Def = pc.getDefClean();
    cout << "Race : ";
    if(type == PlayerT::Shade) {cout << "Shade ";}
    if(type == PlayerT::Drow) {cout << "Drow ";}
    if(type == PlayerT::Vampire) {cout << "Vampire ";}
    if(type == PlayerT::Troll) {cout << "Troll ";}
    if(type == PlayerT::Goblin) {cout << "Goblin ";}
    cout << "Gold: " << gold << endl;
    cout << "HP: " << HP << endl;
    cout << "Atk: " << Atk << endl;
    cout << "Def: " << Def << endl;
    cout << "Action: " << action << endl; 

}

export void linkDragonHoards(vector<unique_ptr<Enemy>> &enemies, vector<DragonHoard> &dragonHoards) {
    vector <Enemy *> Dragons;
    for (auto &element : enemies) {
        if (element->getType() == EnemyT::Dragon) {
            Dragons.push_back(element.get());
        }
    }

    for(auto &hoard : dragonHoards) {
        int bestplace = 10000;
        int bestind = 0;
        int i = 0;
        for(auto &element : Dragons) {
            int rowDif = abs(element->getRow() - hoard.row);
            int colDif = abs(element->getCol() - hoard.col);

            if(rowDif <= 1 && colDif <= 1) {
                int distance = rowDif + colDif;
                if(distance < bestplace) {
                    bestplace = distance;
                    bestind = i;
                }
            }            
        }
        
        hoard.guardian = Dragons[bestind];
        Dragons.erase(Dragons.begin() + bestind);
        i++;
    }
}