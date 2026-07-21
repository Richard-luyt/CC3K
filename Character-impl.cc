module character;

Player::Player(int startHp, int startAtk, int startDef, int startRow, int startCol)
    : hp(startHp), atk(startAtk), def(startDef), row(startRow), col(startCol),
      maxHP(startHp), gold(0) {}

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

void Shade::damageDealt(Enemy &Other) {
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

void Drow::damageDealt(Enemy &Other) {
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

void Vampire::damageDealt(Enemy &Other) {
    if (dynamic_cast<Dwarf*>(&Other) != nullptr) {
        setHp(getHp() - 5);
    } else {
        setHp(getHp() + 5);
    }
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

void Troll::damageDealt(Enemy &Other) {
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

void Goblin::damageDealt(Enemy &Other) {
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

void Human::damageDealt(Player &Other) {
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

void Dwarf::damageDealt(Player &Other) {
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

void Elf::damageDealt(Player &Other) {
}

int Elf::getAttackCount() const {
    if (dynamic_cast<Drow*>(&/* need a Player& here */) ) {} 
    return 2;
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

void Orc::damageDealt(Player &Other) {
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

void Merchant::damageDealt(Player &Other) {
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

void Halfling::damageDealt(Player &Other) {
}

bool Halfling::causesOpponentMiss() const {
    return (rand() % 2 == 0);
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

void Dragon::damageDealt(Player &Other) {
}
