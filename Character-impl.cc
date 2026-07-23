module character;
import state;
import <cmath>;
import <climits>;
import <random>;

using namespace std;

Player::Player(int startHp, int startAtk, int startDef, int startRow, int startCol)
    : hp(startHp), atk(startAtk), def(startDef), row(startRow), col(startCol),
      maxHP(startHp), gold(0) {}

int Player::getAtkClean() const{
    return max(0, atk + atkModif);
}

int Player::getDefClean() const{
    return max(0, def + defModif);
}

void Player::resetAtkDefdelta(){
    atkModif = 0;
    defModif = 0;
}

int Player::getHp() const {
    return hp;
}

void Player::drinkPotion(int potionType) {
    switch (potionType) {
        case 0: {
            int amount = static_cast<int>(10 * potionMult);
            setHp(min(hp + amount, maxHP));
            break;
        }
        case 1:
            atkModif += static_cast<int>(5 * potionMult);
            break;
        case 2:
            defModif += static_cast<int>(5 * potionMult);
            break;
        case 3: {
            int amount = static_cast<int>(10 * potionMult);
            setHp(max(hp - amount, 0));
            break;
        }
        case 4:
            atkModif -= static_cast<int>(5 * potionMult);
            break;
        case 5:  
            defModif -= static_cast<int>(5 * potionMult);
            break;
        default:
            break; 
    }
}

int Player::getGold() const {
    return gold;
}

void Player::addGold(int amount) {
    gold += amount;
}

int Player::getRow() const {
    return row;
}

int Player::getCol() const {
    return col;
}

bool Player::isAlive() const {
    return hp > 0;
}

void Player::setHp(int newHp) {
    hp = newHp;
}

void Player::setAtk(int newAtk) {
    atk = newAtk;
}

void Player::setDef(int newDef) {
    def = newDef;
}

void Player::setPosition(int newRow, int newCol) {
    row = newRow;
    col = newCol;
}

// ===== Shade =====
Shade::Shade()
    : Player(125, 25, 25, 0, 0) {
}

int Shade::getAtk(Enemy &Other) {
    return max(0, atk + atkModif);
}

int Shade::getDef(Enemy &Other) {
    return max(0, def + defModif);
}

int Shade::damageDealt(Enemy &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

PlayerT Shade::getType() const {
    return PlayerT::Shade;
}

// ===== Drow =====
Drow::Drow()
    : Player(150, 25, 15, 0, 0) {
    potionMult = 1.5;
}

int Drow::getAtk(Enemy &Other) {
    return max(0, atk + atkModif);
}

int Drow::getDef(Enemy &Other) {
    return max(0, def + defModif);
}

int Drow::damageDealt(Enemy &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

PlayerT Drow::getType() const {
    return PlayerT::Drow;
}

// ===== Vampire =====
Vampire::Vampire()
    : Player(50, 25, 25, 0, 0) {
        maxHP = INT_MAX;
}

int Vampire::getAtk(Enemy &Other) {
    return max(0, atk + atkModif);
}

int Vampire::getDef(Enemy &Other) {
    return max(0, def + defModif);
}

int Vampire::damageDealt(Enemy &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    if (dynamic_cast<Dwarf*>(&Other) != nullptr) {
        setHp(max(0, getHp() - 5));
    } else {
        setHp(getHp() + 5);
    }
    return damage;
}

PlayerT Vampire::getType() const {
    return PlayerT::Vampire;
}

// ===== Troll =====
Troll::Troll()
    : Player(120, 25, 15, 0, 0) {
    maxHP = 120;
}

int Troll::getAtk(Enemy &Other) {
    return max(0, atk + atkModif);
}

int Troll::getDef(Enemy &Other) {
    return max(0, def + defModif);
}

int Troll::damageDealt(Enemy &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

PlayerT Troll::getType() const {
    return PlayerT::Troll;
}

void Troll::onTurnEnd() {
    setHp(min(hp + 5, maxHP));
}

// ===== Goblin =====
Goblin::Goblin()
    : Player(110, 15, 20, 0, 0) {
}

int Goblin::getAtk(Enemy &Other) {
    return max(0, atk + atkModif);
}

int Goblin::getDef(Enemy &Other) {
    return max(0, def + defModif);
}

int Goblin::damageDealt(Enemy &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

PlayerT Goblin::getType() const {
    return PlayerT::Goblin;
}

// ENEMY

Enemy::Enemy(int startHp, int startAtk, int startDef, int startRow, int startCol)
    : hp(startHp), atk(startAtk), def(startDef), row(startRow), col(startCol), gold(0) {
}

int Enemy::getHp() const {
    return hp;
}

int Enemy::getGold() const {
    return gold;
}

int Enemy::getRow() const {
    return row;
}

int Enemy::getCol() const {
    return col;
}

bool Enemy::isAlive() const {
    return hp > 0;
}

void Enemy::setHp(int newHp) {
    hp = newHp;
}

void Enemy::setAtk(int newAtk) {
    atk = newAtk;
}

void Enemy::setDef(int newDef) {
    def = newDef;
}

void Enemy::setPosition(int newRow, int newCol) {
    row = newRow;
    col = newCol;
}


// ===== Human =====

Human::Human()
    : Enemy(140, 20, 20, 0, 0) {
    gold = 4;
}

int Human::getAtk(Player &Other) {
    return atk;
}

int Human::getDef(Player &Other) {
    return def;
}

int Human::damageDealt(Player &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

EnemyT Human::getType() const {
    return EnemyT::Human;
}

// ===== Dwarf =====
Dwarf::Dwarf()
    : Enemy(100, 20, 30, 0, 0) {
}

int Dwarf::getAtk(Player &Other) {
    return atk;
}

int Dwarf::getDef(Player &Other) {
    return def;
}

int Dwarf::damageDealt(Player &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

EnemyT Dwarf::getType() const {
    return EnemyT::Dwarf;
}

// ===== Elf =====
Elf::Elf()
    : Enemy(140, 30, 10, 0, 0) {
}

int Elf::getAtk(Player &Other) {
    return atk;
}

int Elf::getDef(Player &Other) {
    return def;
}

int Elf::damageDealt(Player &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

int Elf::getAttackCount(Player &Other) const {
    if (dynamic_cast<Drow*>(&Other) != nullptr) {
        return 1;
    }
    return 2;
}

EnemyT Elf::getType() const {
    return EnemyT::Elf;
}

// ===== Orc =====
Orc::Orc()
    : Enemy(180, 30, 25, 0, 0) {
}

int Orc::getAtk(Player &Other) {
    if (dynamic_cast<Goblin*>(&Other) != nullptr) {
        return static_cast<int>(atk * 1.5);
    }
    return atk;
}

int Orc::getDef(Player &Other) {
    return def;
}

int Orc::damageDealt(Player &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

EnemyT Orc::getType() const {
    return EnemyT::Orc;
}

// ===== Merchant =====

Merchant::Merchant()
    : Enemy(30, 70, 5, 0, 0) {
    gold = 4; // merchant hoard
}

int Merchant::getAtk(Player &Other) {
    return atk;
}

int Merchant::getDef(Player &Other) {
    return def;
}

int Merchant::damageDealt(Player &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

EnemyT Merchant::getType() const {
    return EnemyT::Merchant;
}

// ===== Halfling =====

Halfling::Halfling()
    : Enemy(100, 15, 20, 0, 0) {
}

int Halfling::getAtk(Player &Other) {
    return atk;
}

int Halfling::getDef(Player &Other) {
    return def;
}

int Halfling::damageDealt(Player &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

bool Halfling::causesOpponentMiss() const {
    // srand(static_cast<unsigned int>(time(nullptr)));
    return (rand() % 2 == 0);
}

EnemyT Halfling::getType() const {
    return EnemyT::Halfling;
}

// ===== Dragon =====


Dragon::Dragon()
    : Enemy(150, 20, 20, 0, 0) {
    gold = 0;
}

int Dragon::getAtk(Player &Other) {
    return atk;
}

int Dragon::getDef(Player &Other) {
    return def;
}

int Dragon::damageDealt(Player &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

EnemyT Dragon::getType() const {
    return EnemyT::Dragon;
}
