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
