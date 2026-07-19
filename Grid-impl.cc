module grid;

using namespace std;

bool Grid::isFull(const Chamber& c) const{
    for (int i = c.minRow; i < c.maxRow; i++) {
        for (int j = c.minCol[i - c.minRow]; j < c.maxCol[i - c.minRow]; j++) {
            if (Map[i][j] == '.' ) {
                return false;
            }
        }
    }
    return true;
}

Grid::Grid(const string CreateMap[25]) {
    for(int i = 0; i < 25; i++) {
        for(int j = 0; j < 79; j++) {
            Map[i][j] = CreateMap[i][j];
        }
    }

    vector<int> minCol1 = {4,4,4,4};
    vector<int> maxCol1 = {30,30,30,30};
    Chamber c1{104,4,8,minCol1,maxCol1};

    vector<int> minCol2 = {40,40,40,40,62,62,62,62,62,62};
    vector<int> maxCol2 = {63,63,71,74,77,77,77,77,77,77};
    Chamber c2{201,4,14,minCol2,maxCol2};
    
    vector<int> minCol3 = {39,39,39};
    vector<int> maxCol3 = {51,51,51};
    Chamber c3{36,11,14,minCol3,maxCol3};
    
    vector<int> minCol4 = {5,5,5,5,5,5,5};
    vector<int> maxCol4 = {26,26,26,26,26,26,26};
    Chamber c4{147,16,23,minCol4,maxCol4};

    vector<int> minCol5 = {66,66,66,38,38,38};
    vector<int> maxCol5 = {77,77,77,77,77,77};
    Chamber c5{150,17,23,minCol5,maxCol5};

    Chambers[0] = c1;
    Chambers[1] = c2;
    Chambers[2] = c3;
    Chambers[3] = c4;
    Chambers[4] = c5;

}


void Grid::set_position(int row, int col, char c) {
    Map[row][col] = c;
}

bool Grid::move(int prev_row, int prev_col, int cur_row, int cur_col, char& curStepOn) {
    if (Map[prev_row][prev_col] == '@') {
        if (Map[cur_row][cur_col] >= 'A' && Map[cur_row][cur_col] <= 'Z') {
            return false;
        } else if (Map[cur_row][cur_col] >= '0' && Map[cur_row][cur_col] <= '5')  {
            return false;
        }

        char temp = Map[cur_row][cur_col];
        Map[cur_row][cur_col] = Map[prev_row][prev_col];
        Map[prev_row][prev_col] = curStepOn;
        curStepOn = temp;
        return true;
    } else {
        if (Map[cur_row][cur_col] == '.') {
            Map[cur_row][cur_col] = Map[prev_row][prev_col];
            Map[prev_row][prev_col] = curStepOn;
            curStepOn = Map[cur_row][cur_col];
            return true;
        }
        return false;
    }
}

char Grid::get_position(int row, int col) const {
    return Map[row][col];
}

Position Grid::generatePoint() const {
    int c = rand() % 5;
    while (isFull(Chambers[c])) {
        c = rand() % 5;
    }

    const Chamber& selectedChamber = Chambers[c];
    int pos = rand() % selectedChamber.size;
    int row = 0;
    int col = 0;
    
    for (int i = 0; i < selectedChamber.maxRow - selectedChamber.minRow; i++) {
        if (pos < selectedChamber.maxCol[i] - selectedChamber.minCol[i]) {
            row = selectedChamber.minRow + i;
            col = selectedChamber.minCol[i] + pos;
            break;
        }
        pos -= selectedChamber.maxCol[i] - selectedChamber.minCol[i];
    }

    while(Map[row][col] != '.') {
        pos = rand() % selectedChamber.size;
        for (int i = 0; i < selectedChamber.maxRow - selectedChamber.minRow; i++) {
            if (pos < selectedChamber.maxCol[i] - selectedChamber.minCol[i]) {
                row = selectedChamber.minRow + i;
                col = selectedChamber.minCol[i] + pos;
                break;
            }
            pos -= selectedChamber.maxCol[i] - selectedChamber.minCol[i];
        }
    }

    return Position{row, col};

}


void Grid::generateDragonPair(vector<unique_ptr<Enemy>>& enemies, vector<DragonHoard>& h) {
    
    Position p;
    bool success = false;
    int dc;
    int dr;
    while (success == false) {

        p = generatePoint();

        if (Map[p.row-1][p.col-1] == '.' || Map[p.row-1][p.col] == '.' ||
            Map[p.row-1][p.col+1] == '.' || Map[p.row][p.col-1] == '.' ||
            Map[p.row][p.col+1] == '.' || Map[p.row+1][p.col-1] == '.' ||
            Map[p.row+1][p.col+1] == '.' || Map[p.row+1][p.col] == '.') {
                dr = rand() % 3;
                dc = rand() % 3;
                while (Map[p.row + dr - 1][p.col + dc - 1] != '.'  || (dr == 1 && dc == 1)) {
                    dr = rand() % 3;
                    dc = rand() % 3;
                }
            success = true;
        }

    }

    h.push_back(DragonHoard());
    h.back().row = p.row;
    h.back().col = p.col;
    enemies.push_back(make_unique<Dragon>());
    enemies.back()->setPosition(p.row + dr - 1, p.col + dc - 1);
    h.back().guardian = enemies.back().get();
    Map[p.row][p.col] = '9';
    Map[p.row + dr - 1][p.col + dc - 1] = 'D';


}

ostream& operator<<(ostream &out, const Grid &g) {
     for(int i = 0; i < 25; i++) {
        for(int j = 0; j < 79; j++) {
            char c = g.Map[i][j];
            if (c == '@' || c == '\\') {
                out << "\033[34m" << c << "\033[0m";
            } else if (c >= 'A' && c <= 'Z') {
                out << "\033[31m" << c << "\033[0m";
            } else if (c >= '0' && c <= '5') {
                out << "\033[32mP\033[0m";
            } else if (c >= '6' && c <= '9') {
                out << "\033[33mG\033[0m";
            } else {
                out << c;
            }
        }
        out << '\n';
    }

    return out;

}

void parse(string(&map)[25], vector<unique_ptr<Enemy>>& enemies, Player& pc, vector<DragonHoard>& h, ifstream& f) {
    
    string inp;

    for (int i = 0; i < 25; i++) {
        getline(f, inp);
        map[i] = inp;
        for (int j = 0; j < 79; j++) {
            if (inp[j] == '@') {
                pc.setPosition(i,j);
            } else if (inp[j] == 'H') {
                enemies.push_back(make_unique<Human>());
                enemies.back()->setPosition(i, j);
            } else if (inp[j] == 'W') {
                enemies.push_back(make_unique<Dwarf>());
                enemies.back()->setPosition(i, j);
            } else if (inp[j] == 'E') {
                enemies.push_back(make_unique<Elf>());
                enemies.back()->setPosition(i, j);
            } else if (inp[j] == 'O') {
                enemies.push_back(make_unique<Orc>());
                enemies.back()->setPosition(i, j);
            } else if (inp[j] == 'M') {
                enemies.push_back(make_unique<Merchant>());
                enemies.back()->setPosition(i, j);
            } else if (inp[j] == 'D') {
                enemies.push_back(make_unique<Dragon>());
                enemies.back()->setPosition(i, j);
            } else if (inp[j] == 'L') {
                enemies.push_back(make_unique<Halfling>());
                enemies.back()->setPosition(i, j);
            } else if (inp[j] == '9') {
                h.push_back(DragonHoard());
                h.back().row = i;
                h.back().col = j;
            }
        }
    }
}

void create(Grid& g, vector<unique_ptr<Enemy>>& enemies, Player& pc, vector<DragonHoard>& h) {

    Position p = g.generatePoint();
    pc.setPosition(p.row,p.col);
    g.Map[p.row][p.col] = '@';

    p = g.generatePoint();
    g.Map[p.row][p.col] = '\\';

    for(int i = 0; i < 10; i++) {
        int r = rand() % 6;
        p = g.generatePoint();
        g.Map[p.row][p.col] = '0' + r;
    }

    for(int i = 0; i < 10; i++) {
        int r = rand() % 8;
        if (r < 5){
            p = g.generatePoint();
            g.Map[p.row][p.col] = '6';
        } else if (r < 6){
            g.generateDragonPair(enemies, h);
        } else {
            p = g.generatePoint();
            g.Map[p.row][p.col] = '7';
        }
    }

    for(int i = 0; i < 20; i++) {
        int r = rand() % 18;
        p = g.generatePoint();
        if (r < 4){
            g.Map[p.row][p.col] = 'H';
            enemies.push_back(make_unique<Human>());
            enemies.back()->setPosition(p.row, p.col);
        } else if (r < 7){
            g.Map[p.row][p.col] = 'W';
            enemies.push_back(make_unique<Dwarf>());
            enemies.back()->setPosition(p.row, p.col);
        }else if (r < 12){
            g.Map[p.row][p.col] = 'L';
            enemies.push_back(make_unique<Halfling>());
            enemies.back()->setPosition(p.row, p.col);
        } else if (r < 14){
            g.Map[p.row][p.col] = 'E';
            enemies.push_back(make_unique<Elf>());
            enemies.back()->setPosition(p.row, p.col);
        } else if (r < 16){
            g.Map[p.row][p.col] = 'O';
            enemies.push_back(make_unique<Orc>());
            enemies.back()->setPosition(p.row, p.col);
        } else {
            g.Map[p.row][p.col] = 'M';
            enemies.push_back(make_unique<Merchant>());
            enemies.back()->setPosition(p.row, p.col);
        }
    }

        
}

//To be added in the utilities.cc file
bool compare(const Enemy *A, const Enemy *B) {
    if(A->getRow() != B->getRow()) {
        return A->getRow() < B->getRow();
    }
    return A->getCol() < B->getCol();
}