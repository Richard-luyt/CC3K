import Enemy;
import Player;
import Grid;
import state;

import <iostream>;
import <random>;
import <chrono>;
import <vector>;
import <memory>;

using namespace std;

enum class GameResult {Won, Died, Restarted, Quit, Init};

unsigned seed = chrono::system_clock::now().time_since_epoch().count();
default_random_engine rng{seed};


void playerAttack(Player &pc, Enemy &enemy) {
    pc.damagedealt(enemy);
}

void enemyAttack(Player &pc, Enemy &enemy) {
    enemy.damagedealt(pc);
}

bool verifyMove(const Grid &grid, int nextRow, int nextCol, bool player) {
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
        return false;
    }
}

GameResult runGame(int argc, char **argv, Player &pc) {

}

GameResult playgame(int argc, char **argv, char race_choice) {
    switch (race_choice) {
        case 's' : {
            Shade pc;
            return runGame(argc, argv, pc);
        }
        case 'd' : {
            Drow pc;
            return runGame(argc, argv, pc);
        }
        case 'v' : {
            Vampire pc;
            return runGame(argc, argv, pc);
        }
        case 'g' : {
            Goblin pc;
            return runGame(argc, argv, pc);
        }
        case 't' : {
            Troll pc;
            return runGame(argc, argv, pc);
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
    GameResult gameState = GameResult::Init;
    char race_choice;
    cout << "please input a character race to continue ..." << endl;
    while(gameState != GameResult::Quit && cin >> race_choice) {
        gameState = playgame(argc, argv, race_choice);
        cout << "please input a character race to continue ..." << endl;
    }
}