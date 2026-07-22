export module character;

import <string>;
import <iostream>;
import state;

using namespace std;

export class Enemy;

export class Player {
    protected:
        int hp;
        int atk;
        int def;
        int row;
        int col;
        int maxHP;
        int gold;
        int atkModif = 0;
        int defModif = 0;
        double potionMult = 1.0;
        double goldMult = 1.0;
        Player(int startHp, int startAtk, int startDef, int startRow, int startCol);
    public:

        int getAtkClean() const;
        int getDefClean() const;
        void resetAtkDefdelta();
        char curStepOn = '.';
        int getHp() const;
        virtual int getAtk(Enemy &Other) = 0;
        virtual int getDef(Enemy &Other) = 0;
        virtual int damageDealt(Enemy &Other) = 0;
        virtual PlayerT getType() const = 0;

        void drinkPotion(int type);
        int getGold() const;
        void addGold(int amount);
        int getRow() const;
        int getCol() const;
        bool isAlive() const;

        void setHp(int newHp);
        void setAtk(int newAtk);
        void setDef(int newDef);
        void setPosition(int row, int col);
        virtual ~Player() = default;
};

export class Shade : public Player {
    public:
        Shade();  // hp=125, atk=25, def=25
        int getAtk (Enemy &Other) override;
        int getDef (Enemy &Other) override;
        int damageDealt(Enemy &Other) override;
        PlayerT getType() const override;
};

export class Drow : public Player {
    public:
        Drow();  // hp=150, atk=25, def=15, potionMult=1.5
        int getAtk (Enemy &Other) override;
        int getDef (Enemy &Other) override;
        int damageDealt(Enemy &Other) override;
        PlayerT getType() const override;
};

export class Vampire : public Player {
    public:
        Vampire();  // hp=50, atk=25, def=25
        int getAtk (Enemy &Other) override;
        int getDef (Enemy &Other) override;
        int damageDealt(Enemy &Other) override;
        PlayerT getType() const override;
};

export class Troll : public Player {
    public:
        Troll();  // hp=120, atk=25, def=15, maxHP=120
        int getAtk (Enemy &Other) override;
        int getDef (Enemy &Other) override;
        int damageDealt(Enemy &Other) override;
        PlayerT getType() const override;
};

export class Goblin : public Player {
    public:
        Goblin();  // hp=110, atk=15, def=20
        int getAtk (Enemy &Other) override;
        int getDef (Enemy &Other) override;
        int damageDealt(Enemy &Other) override;
        PlayerT getType() const override;
};


export class Enemy {
    protected:
        int hp;
        int atk;
        int def;
        int row;
        int col;
        int gold; // How much gold each enemy drops when killed

        Enemy(int startHp, int startAtk, int startDef, int startRow, int startCol);
    public:
        bool deathProcessed;
        int getHp() const;
        virtual int getAtk (Player &Other) = 0;
        virtual int getDef (Player &Other) = 0;
        virtual int damageDealt(Player &Other) = 0;
        virtual int getAttackCount() const { return 1; }          // Elf overrides
        virtual bool causesOpponentMiss() const { return false; } // Halfling overrides
        virtual EnemyT getType() const = 0;
        int getGold() const;
        int getRow() const;
        int getCol() const;
        bool isAlive() const;

        void setHp(int newHp);
        void setAtk(int newAtk);
        void setDef(int newDef);
        void setPosition(int row, int col);
        virtual ~Enemy() = default;
};

export class Human : public Enemy {
    public:
        Human();  // hp=140, atk=20, def=20
        int getAtk(Player &Other) override;
        int getDef(Player &Other) override;
        int damageDealt(Player &Other) override;
        EnemyT getType() const override;
};

export class Dwarf : public Enemy {
    public:
        Dwarf();  // hp=100, atk=20, def=30
        int getAtk(Player &Other) override;
        int getDef(Player &Other) override;
        int damageDealt(Player &Other) override;
        EnemyT getType() const override;
};

export class Elf : public Enemy {
    public:
        Elf();  // hp=140, atk=30, def=10
        int getAtk(Player &Other) override;
        int getDef(Player &Other) override;
        int damageDealt(Player &Other) override;
        int getAttackCount() const override;
        EnemyT getType() const override;
};

export class Orc : public Enemy {
    public:
        Orc();  // hp=180, atk=30, def=25
        int getAtk(Player &Other) override;
        int getDef(Player &Other) override;
        int damageDealt(Player &Other) override;
        EnemyT getType() const override;
};

export class Merchant : public Enemy {
    public:
        Merchant();  // hp=30, atk=70, def=5
        int getAtk(Player &Other) override;
        int getDef(Player &Other) override;
        int damageDealt(Player &Other) override;
        EnemyT getType() const override;
};

export class Halfling : public Enemy {
    public:
        Halfling();  // hp=100, atk=15, def=20
        int getAtk(Player &Other) override;
        int getDef(Player &Other) override;
        int damageDealt(Player &Other) override;
        bool causesOpponentMiss() const override;
        EnemyT getType() const override;
};

export class Dragon : public Enemy {
    public:
        Dragon();  // hp=150, atk=20, def=20
        int getAtk(Player &Other) override;
        int getDef(Player &Other) override;
        int damageDealt(Player &Other) override;
        EnemyT getType() const override;
};