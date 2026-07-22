module character;
import state;

Player::Player(int startHp, int startAtk, int startDef, int startRow, int startCol)
    : hp(startHp), atk(startAtk), def(startDef), row(startRow), col(startCol),
      maxHP(startHp), gold(0) {}

int Player::getAtkClean() {
    return atk;
}

int Player::getDefClean() {
    return def;
}

int Player::getHp() const {
    return hp;
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
Shade::Shade(int row, int col)
    : Player(125, 25, 25, row, col) {
}

int Shade::getAtk(Enemy &Other) {
    return atk + atkModif;
}

int Shade::getDef(Enemy &Other) {
    return def + defModif;
}

int Shade::damageDealt(Enemy &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

// ===== Drow =====
Drow::Drow(int row, int col)
    : Player(150, 25, 15, row, col) {
    potionMult = 1.5;
}

int Drow::getAtk(Enemy &Other) {
    return atk + atkModif;
}

int Drow::getDef(Enemy &Other) {
    return def + defModif;
}

int Drow::damageDealt(Enemy &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

PlayerT Drow::getType() {
    return PlayerT::Drow;
}

// ===== Vampire =====
Vampire::Vampire(int row, int col)
    : Player(50, 25, 25, row, col) {
}

int Vampire::getAtk(Enemy &Other) {
    return atk + atkModif;
}

int Vampire::getDef(Enemy &Other) {
    return def + defModif;
}

int Vampire::damageDealt(Enemy &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    if (dynamic_cast<Dwarf*>(&Other) != nullptr) {
        setHp(getHp() - 5);
    } else {
        setHp(getHp() + 5);
    }
    return damage;
}

PlayerT Vampire::getType() {
    return PlayerT::Vampire;
}

// ===== Troll =====
Troll::Troll(int row, int col)
    : Player(120, 25, 15, row, col) {
    maxHP = 120;
}

int Troll::getAtk(Enemy &Other) {
    return atk + atkModif;
}

int Troll::getDef(Enemy &Other) {
    return def + defModif;
}

int Troll::damageDealt(Enemy &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

PlayerT Troll::getType() {
    return PlayerT::Troll;
}

// ===== Goblin =====
Goblin::Goblin(int row, int col)
    : Player(110, 15, 20, row, col) {
}

int Goblin::getAtk(Enemy &Other) {
    return atk + atkModif;
}

int Goblin::getDef(Enemy &Other) {
    return def + defModif;
}

int Goblin::damageDealt(Enemy &Other) {
    int damage = static_cast<int>(ceil((100.0 / (100 + Other.getDef(*this))) * getAtk(Other)));
    Other.setHp(max(Other.getHp() - damage, 0));
    return damage;
}

PlayerT Goblin::getType() {
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
Human::Human() : Enemy(140, 20, 20, 0, 0) {}

Human::Human(int row, int col)
    : Enemy(140, 20, 20, row, col) {
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

EnemyT getType() {
    return EnemyT::Human;
}

// ===== Dwarf =====
Dwarf::Dwarf(int row, int col)
    : Enemy(100, 20, 30, row, col) {
    gold = (rand() % 2 == 0) ? 1 : 2; // 50/50 small vs. normal pile
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

EnemyT getType() {
    return EnemyT::Dwarf;
}

// ===== Elf =====
Elf::Elf(int row, int col)
    : Enemy(140, 30, 10, row, col) {
    gold = (rand() % 2 == 0) ? 1 : 2; // 50/50 small vs. normal pile
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

int Elf::getAttackCount() const {
    if (dynamic_cast<Drow*>(&/* need a Player& here */) ) {} 
    return 2;
}

EnemyT getType() {
    return EnemyT::Elf;
}

// ===== Orc =====
Orc::Orc(int row, int col)
    : Enemy(180, 30, 25, row, col) {
    gold = (rand() % 2 == 0) ? 1 : 2;
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

EnemyT getType() {
    return EnemyT::Ord;
}

// ===== Merchant =====
Merchant::Merchant(int row, int col)
    : Enemy(30, 70, 5, row, col) {
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

EnemyT getType() {
    return EnemyT::Merchant;
}

// ===== Halfling =====
Halfling::Halfling(int row, int col)
    : Enemy(100, 15, 20, row, col) {
    gold = (rand() % 2 == 0) ? 1 : 2;
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
    return (rand() % 2 == 0);
}

EnemyT getType() {
    return EnemyT::Halfling;
}

// ===== Dragon =====
Dragon::Dragon(int row, int col)
    : Enemy(150, 20, 20, row, col) {
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

EnemyT getType() {
    return EnemyT::Dragon;
}
