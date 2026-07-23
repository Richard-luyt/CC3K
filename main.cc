import state;
import character;
import grid;
import utility;

import <iostream>;
import <random>;
import <chrono>;
import <fstream>;
import <vector>;
import <string>;
import <algorithm>;
import <utility>;
import <memory>;

using namespace std;

enum class GameResult {Won, Died, Restarted, Quit, Init};

unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine rng{seed};



const string potionNames[6] = { "RH", "BA", "BD", "PH", "WA", "WD" };

GameResult runGame(int argc, char **argv, Player &pc) {
    bool isMerchantHostile = false;
    bool freeze = false;
    string action = "";
    bool knownPotion[6] = {};
    char temp_character = '.';

    const char *filename;

    // step 0 - parse the graph and save Enemies, Player, DragonHoards
    if(argc == 1) {
        filename = "emptyfloor.txt";
    } else {
        filename = argv[1];
    }

    //cout << "before parsing" << endl;

    ifstream iff{filename};

    if(!iff) {
        cout << "The floor file can not be read" << endl;
        return GameResult::Quit;
    }

    for(int i = 0; i < 5; i++) {

        vector<unique_ptr<Enemy>> enemies;
        vector<DragonHoard> dragonHoards;

        string map[25];

        //cout << "before parsing" << endl;
        parse(map, enemies, pc, dragonHoards, iff);
        Grid game{map};
        //cout << "before create" << endl;
        if(argc == 1) {
            create(game, enemies, pc, dragonHoards);
            iff.clear();
            iff.seekg(0);
        } else {
            linkDragonHoards(enemies, dragonHoards);
        }
        //cout << "done" << endl;

        // all the possible commands:
        // 1. dir : no,so,ea,we,ne,nw,se,sw
        // 2. u + dir -> use the potion
        // 3. a + dir -> attack to direction
        // 4. f -> freeze
        // 5. r -> restart
        // 6. q -> quit
        
        action = "Player character has spawned";
        pc.curStepOn = '.';
        pc.resetAtkDefdelta(); // resets deltaATK, deltaDef to 0
        Display(game, pc, i+1, action);

        while (true) {
            string command;
            action = "";
            if(!(cin >> command)) {
                return GameResult::Quit;
            }
            int playerRow = pc.getRow();
            int playerCol = pc.getCol();

            pair<int,int> getans;
            int nrow, ncol;

            if(command == "q"){
                return GameResult::Quit;
            } else if(command == "r") {
                return GameResult::Restarted;
            } else if(command == "f") {
                if(freeze == false)
                    action += "Enemy movement is frozen. ";
                else
                    action += "Enemy movement is unfrozen. ";
                freeze = !freeze;
            } else if(command == "u") {
                // using potion in a certain direction
                string dir;
                if (!(cin >> dir)) {
                    return GameResult::Quit;
                }
                if(!validDirection(dir)) {
                    action = "Invalid direction, please re-enter";
                    Display(game, pc, i+1, action);
                    continue;
                }
                getans = nextMove(playerRow, playerCol ,dir);
                nrow = getans.first;
                ncol = getans.second;
                char potion = game.get_position(nrow, ncol);
                if(potion < '0' || potion > '5') {
                    Display(game, pc, i + 1, "There is no potion in that direction.");
                    continue;
                }
                int type = UsePotion(game, pc, nrow, ncol);
                if(knownPotion[type] == false) {
                    knownPotion[type] = true;
                    action += "PC Discovered new potion " + potionNames[type];
                } else {
                    action += "PC Used known potion " + potionNames[type];
                }
            } else if(command == "a") {
                // attacking in a certain direction
                string dir;
                if (!(cin >> dir)) {
                    return GameResult::Quit;
                }
                if(!validDirection(dir)) {
                    action = "Invalid direction, please re-enter";
                    Display(game, pc, i+1, action);
                    continue;
                }
                getans = nextMove(playerRow, playerCol ,dir);
                nrow = getans.first;
                ncol = getans.second;

                bool isenemy = false;
                for(auto &element : enemies) {
                    if(element->getRow() == nrow && element->getCol() == ncol && element->isAlive()) {
                        isenemy = true;
                        EnemyT nextEnemy = element->getType();

                        if(nextEnemy == EnemyT::Merchant) {
                            isMerchantHostile = true;
                        }

                        char T = game.get_position(nrow, ncol);

                        if(nextEnemy == EnemyT::Halfling && element->causesOpponentMiss()) {
                            action += "PC misses, ";
                        } else {
                            int damage = playerAttack(pc, *element);
                            int nowHP = element->getHp();
                            action += "PC deals " + to_string(damage) + " damage to " + T + " (" + to_string(nowHP) + " HP). ";    
                        }
                        

                        if(!element->deathProcessed && !element->isAlive()) {
                            if (pc.getType() == PlayerT::Goblin) {
                                pc.addGold(5);
                            }
                            element->deathProcessed = true;
                            game.set_position(nrow, ncol, '.');
                            if(nextEnemy == EnemyT::Human) {
                                // the document also didn't mention how to handle the postion of the second gold pile
                                // so here we assume that it generates near the people
                                // if there is no empty space then it is not generated

                                vector<pair <int, int>> availableGold;

                                for(int dx = -1; dx <= 1; dx++) {
                                    for(int dy = -1; dy <= 1; dy++) {
                                        if(dx == 0 && dy == 0) {
                                            continue;
                                        }
                                        int prow = nrow + dx;
                                        int pcol = ncol + dy;

                                        if(game.get_position(prow, pcol) == '.') {
                                            pair<int,int> temp;
                                            temp.first = prow;
                                            temp.second = pcol;
                                            availableGold.push_back(temp);
                                        }
                                    }
                                }

                                if(!availableGold.empty()) {
                                    int place = rng() % availableGold.size();
                                    int grow = availableGold[place].first;
                                    int gcol = availableGold[place].second;
                                    game.set_position(grow, gcol, '6');
                                }
                                
                                game.set_position(nrow, ncol, '6');
                            } else if(nextEnemy == EnemyT::Merchant) {
                                game.set_position(nrow, ncol, '8');
                            } else if(nextEnemy != EnemyT::Dragon){
                                // the document didn't mention at what rate pc gets 1 gold or 2 gold
                                // so here we use random
                                if(rng() % 2 == 0) pc.addGold(2);
                                else pc.addGold(1);
                            }
                        }
                        break;
                    }
                }
                if (!isenemy) {
                    action = "There is no enemy in that direction.";
                }
            } else {
                // moving in a certain direction
                string dir;
                dir = command;
                if(!validDirection(dir)) {
                    action = "Invalide direction, please re-input";
                    Display(game, pc, i+1, action);
                    continue;
                }
                getans = nextMove(playerRow, playerCol ,dir);
                nrow = getans.first;
                ncol = getans.second;

                if(!verifyMove(game, nrow, ncol, true)){
                    action = "Invalide move, please re-input";
                    Display(game, pc, i+1, action);
                    continue;
                }

                string converter = Converter(dir);
                action += "PC moves " + converter + " ";

                char destination = game.get_position(nrow, ncol);
                game.move(playerRow, playerCol, nrow, ncol, pc.curStepOn);
                pc.setPosition(nrow, ncol);
 
                if(destination == '\\') {
                    if(i == 4) {
                        return GameResult::Won;
                    }
                    break;
                } else if(destination == '6') {
                    pc.addGold(2);
                    pc.curStepOn = '.';
                    action += " and picks up a 2 gold";
                } else if(destination == '7') {
                    pc.addGold(1);
                    pc.curStepOn = '.';
                    action += " and picks up a 1 gold";
                } else if(destination == '8') {
                    pc.addGold(4);
                    pc.curStepOn = '.';
                    action += " and picks up a 4 gold";
                } else if(destination == '9') {
                    bool dragon_slayed = false;
                    for(auto &element : dragonHoards) {
                        int row = element.row;
                        int col = element.col;
                        if(row == nrow && col == ncol) {
                            if(element.guardian != nullptr && !element.guardian->isAlive()) {
                                dragon_slayed = true;
                            }
                            break;
                        }
                    }
                    if(dragon_slayed){
                        pc.addGold(6);
                        pc.curStepOn = '.';
                        action += " and picks up a dragon hoard";
                    }
                }

                for(int dx = -1; dx <= 1; dx++) {
                    bool saw = false;
                    for(int dy = -1; dy <= 1; dy++) {
                        int nx = pc.getRow() + dx;
                        int ny = pc.getCol() + dy;
                        char item = game.get_position(nx, ny);
                        if(item - '0' <= 5 && item - '0' >= 0 && knownPotion[item - '0'] == false) {
                            action += " and sees an unknown potion";
                            saw = true;
                            break;
                        }
                    }
                    if(saw) {
                        break;
                    }
                }
                action += ".";
            }

            if(!pc.isAlive()) {
                cout << "The Player Died" << endl;
                return GameResult::Died;
            }

            Enemy *attackedDragon[20];
            int cntDragon = 0;

            for(auto &element : dragonHoards) {
                if(IsNear(pc.getRow(), pc.getCol(), element.row, element.col)) {
                    if(element.guardian != nullptr && element.guardian->isAlive()) {
                        if(rng() % 2 == 0) {
                            int damage = enemyAttack(pc, *element.guardian);
                            action += "Dragon deals " + to_string(damage) + " damage to PC.";
                            if(!pc.isAlive()) {
                                cout << "The Player Died" << endl;
                                return GameResult::Died;
                            }  
                        } else {
                            action += "Dragon misses.";
                        }
                        
                        attackedDragon[cntDragon] = element.guardian;
                        cntDragon++;
                    }
                }
            }
            
            sort(enemies.begin(), enemies.end(), compare);
            for(auto &element : enemies) {
                int indicator = rng() % 2;
                if(!element->isAlive()) { continue;}
                char type = game.get_position(element->getRow(), element->getCol());
                if(element->getType() == EnemyT::Dragon) {
                    bool checkAtk = false;
                    for(int j = 0; j < cntDragon; j++) {
                        if(attackedDragon[j] == element.get()){
                            checkAtk = true;
                            break;
                        }
                    }
                    if(checkAtk == false && IsNear(pc.getRow(), pc.getCol(), element->getRow(), element->getCol())) {
                        if(indicator == 0) {
                            int damage = enemyAttack(pc, *element);
                            action += "Dragon deals " + to_string(damage) + " damage to PC.";
                            if(!pc.isAlive()) {
                                cout << "The Player Died" << endl;
                                return GameResult::Died;
                            }
                        } else {
                            action += "Dragon misses.";    
                        }
                        
                    }
                } else if(IsNear(pc.getRow(), pc.getCol(), element->getRow(), element->getCol()) && ((element->getType() == EnemyT::Merchant && isMerchantHostile) || element->getType() != EnemyT::Merchant)) {  
                    int attackCount = element->getAttackCount(pc);

                    for(int k = 0; k < attackCount; k++) {
                        if(rng() % 2 == 0) {
                            int damage = enemyAttack(pc, *element);
                            action += string(1, type) + " deals " + to_string(damage) + " damage to PC,"; 
                            if(pc.isAlive() == false) {
                                cout << "The player Died" << endl;
                                return GameResult::Died;
                            }
                        } else {
                            action += " " + string(1, type) + " misses.";
                        }
                    }
                } else if(!freeze){
                    int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
                    int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
                    bool available[8];
                    int cnt = 0;
                    for(int j = 0; j < 8; j++) {
                        int nrow = element->getRow() + dx[j];
                        int ncol = element->getCol() + dy[j];
                        if(verifyMove(game, nrow, ncol, false) == true) {
                            available[j] = true;
                            cnt++;
                        } else {
                            available[j] = false;
                        }
                    }
                    if(cnt != 0) {
                        int choice = rng() % cnt + 1;
                        for(int j = 0; j < 8; j++) {
                            if(available[j] == true) {
                                if(choice == 1) {
                                    int nrow = element->getRow() + dx[j];
                                    int ncol = element->getCol() + dy[j];
                                    
                                    game.move(element->getRow(), element->getCol(), nrow, ncol, temp_character);
                                    element->setPosition(nrow, ncol);
                                    break;
                                } else {
                                    choice --;
                                }
                            }
                        }
                    }
                    

                }
            }
            if(pc.isAlive() == false) {
                cout << "The player Died" << endl;
                return GameResult::Died;
            }

            if(pc.getType() == PlayerT::Troll && pc.isAlive() == true) {
                pc.onTurnEnd();
            }

            Display(game, pc, i+1, action);
        }
    }
    return GameResult::Won;
}

GameResult playgame(int argc, char **argv, char race_choice) {
    switch (race_choice) {
        case 's' : {
            Shade pc;
            GameResult result = runGame(argc, argv, pc);
            if(result == GameResult::Won) {
                cout << "You Won, your score is: " << pc.getGold() * 1.5 << endl;
            }
            return result;
        }
        case 'd' : {
            Drow pc;
            GameResult result = runGame(argc, argv, pc);
            if(result == GameResult::Won) {
                cout << "You Won, your score is: " << pc.getGold() << endl;
            }
            return result;
        }
        case 'v' : {
            Vampire pc;
            GameResult result = runGame(argc, argv, pc);
            if(result == GameResult::Won) {
                cout << "You Won, your score is: " << pc.getGold() << endl;
            }
            return result;
        }
        case 'g' : {
            Goblin pc;
            GameResult result = runGame(argc, argv, pc);
            if(result == GameResult::Won) {
                cout << "You Won, your score is: " << pc.getGold() << endl;
            }
            return result;
        }
        case 't' : {
            Troll pc;
            GameResult result = runGame(argc, argv, pc);
            if(result == GameResult::Won) {
                cout << "You Won, your score is: " << pc.getGold() << endl;
            }
            return result;
        }
        case 'q' : {
            return GameResult::Quit;
        }
        default: {
            cout << "Invalid race, please choose a valid race to continue, or quit directly" << endl;
            return GameResult::Restarted;
        }
    }
}

int main(int argc, char **argv) {
    srand(static_cast<unsigned int>(time(nullptr)));
    GameResult gameState = GameResult::Init;
    char race_choice;
    cout << "please input a character race to continue ..." << endl;
    while(gameState != GameResult::Quit && cin >> race_choice) {
        gameState = playgame(argc, argv, race_choice);
        if(gameState == GameResult::Died) {
            cout << " You Died, please input a race and try again" << endl;
        } else if (gameState != GameResult::Quit){
            cout << "please input a character race to continue ..." << endl;
        }
        
    }
}