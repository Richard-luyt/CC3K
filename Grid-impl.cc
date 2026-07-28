module grid;

using namespace std;

bool Grid::isFull(int c) const {
    for (const Position &p : chamberTiles[c]) {
        if (Map[p.row][p.col] == '.') return false;
    }
    return true;
}

bool Grid::isFloor(char c) {
    return c == '.' || c == '@' || c == '\\' ||
           (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z');
}

int Grid::inchamber_fill(int i, int j, int cno) {
    if (i < 0 || i >= HEIGHT || j < 0 || j >= WIDTH){
        return 0;
    } 
    if (!isFloor(Map[i][j])) {
        return 0;
    }

    if (inchamber[i][j] != 0) {
        return 0;
    }

    inchamber[i][j] = cno;
    chamberTiles[cno - 1].push_back(Position{i, j, cno - 1});

    return 1 + inchamber_fill(i+1, j, cno) + inchamber_fill(i-1, j, cno)
        + inchamber_fill(i, j+1, cno) + inchamber_fill(i, j-1, cno);
}


Grid::Grid(const string CreateMap[HEIGHT]) {
    
    inchamber.assign(HEIGHT, vector<int>(WIDTH, 0));

    for(int i = 0; i < HEIGHT; i++) {
        Map[i] = CreateMap[i];
    }

    for (int i = 1; i < HEIGHT - 1; i++) {
        for (int j = 1; j < WIDTH - 1; j++) {
            if (isFloor(Map[i][j]) && inchamber[i][j] == 0) {
                chamberTiles.push_back({});
                chamberSize.push_back(inchamber_fill(i, j, chamber_count));
                chamber_count++;
            }
        }
    }

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
            curStepOn = '.';
            return true;
        }
        return false;
    }
}

char Grid::get_position(int row, int col) const {
    return Map[row][col];
}

Position Grid::generatePoint(int stairChamber) const {
    
    int n = chamberTiles.size();
    int c = rand() % n;
    while (isFull(c) || c == stairChamber) {
        c = rand() % n;
    }

    const auto &tiles = chamberTiles[c];
    Position p = tiles[rand() % tiles.size()];
    while (Map[p.row][p.col] != '.') {
        p = tiles[rand() % tiles.size()];
    }

    return p;

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
     for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            char c = g.get_position(i, j);
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

void parse(string(&map)[HEIGHT], vector<unique_ptr<Enemy>>& enemies, Player& pc, vector<DragonHoard>& h, ifstream& f) {
    
    string inp;

    for (int i = 0; i < HEIGHT; i++) {
        getline(f, inp);
        map[i] = inp;
        for (int j = 0; j < WIDTH; j++) {
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
    g.set_position(p.row, p.col, '@');

    p = g.generatePoint(p.chamber);
    g.set_position(p.row, p.col, '\\');

    for(int i = 0; i < 10; i++) {
        int r = rand() % 6;
        p = g.generatePoint();
        g.set_position(p.row, p.col, '0' + r);
    }

    for(int i = 0; i < 10; i++) {
        int r = rand() % 8;
        if (r < 5){
            p = g.generatePoint();
            g.set_position(p.row, p.col, '6');
        } else if (r < 6){
            g.generateDragonPair(enemies, h);
        } else {
            p = g.generatePoint();
            g.set_position(p.row, p.col, '7');
        }
    }

    for(int i = 0; i < 20; i++) {
        int r = rand() % 18;
        p = g.generatePoint();
        if (r < 4){
            g.set_position(p.row, p.col, 'H');
            enemies.push_back(make_unique<Human>());
            enemies.back()->setPosition(p.row, p.col);
        } else if (r < 7){
            g.set_position(p.row, p.col, 'W');
            enemies.push_back(make_unique<Dwarf>());
            enemies.back()->setPosition(p.row, p.col);
        }else if (r < 12){
            g.set_position(p.row, p.col, 'L');
            enemies.push_back(make_unique<Halfling>());
            enemies.back()->setPosition(p.row, p.col);
        } else if (r < 14){
            g.set_position(p.row, p.col, 'E');
            enemies.push_back(make_unique<Elf>());
            enemies.back()->setPosition(p.row, p.col);
        } else if (r < 16){
            g.set_position(p.row, p.col, 'O');
            enemies.push_back(make_unique<Orc>());
            enemies.back()->setPosition(p.row, p.col);
        } else {
            g.set_position(p.row, p.col, 'M');
            enemies.push_back(make_unique<Merchant>());
            enemies.back()->setPosition(p.row, p.col);
        }
    }

        
}

