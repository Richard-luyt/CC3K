export module Character;

class character {
    int HP;
    int Atk;
    int Def;
    int x;
    bool dead = false;
    int y;
    int Gold = -1;
    char type;

    //----Enemies----
    // H -> Human
    // W -> Dwarf
    // E -> Elf
    // O -> Orc
    // M -> Merchant
    // D -> Dragon
    // L -> Halfling

    //----Characters----
    // s -> Shade
    // d -> Drow
    // v -> vampire
    // t -> troll
    // g -> goblin

    public:
        character(int HP, int Atk, int Def, int x, int y) {

        }
        int getAtk() const;
        int getDef() const;
        int getHP() const;
        int getx() const;
        int gety() const;
        int getGold() const;
        bool getDead() const;

        void setAtk();
        void setDef();
        void setHP();
        void setx();
        void sety();
        void setGold();
        void setDead();

};