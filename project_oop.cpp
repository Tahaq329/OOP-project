#include <iostream>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
#include <set>
using namespace std;

class Character;

class Item {
protected:
    string name;
public:
    Item(string n) : name(n) {}
    virtual ~Item() {}
    string getName() { return name; }
    virtual void use(Character* target) = 0;
};

class HealItem : public Item {
public:
    HealItem() : Item("Heal Potion") {}
    void use(Character* target);
};

class RageItem : public Item {
public:
    RageItem() : Item("Rage Potion") {}
    void use(Character* target);
};

class Character {
protected:
    string name;
    int hp, maxHp;
    int damage;
    int special;
    int heals;
    bool defending;

public:
    Character(string n, int h, int d) : name(n), hp(h), maxHp(h), damage(d), special(0), heals(3), defending(false) {}
    virtual ~Character() {}

    string getName() { return name; }
    int getHealth() { return hp; }
    int getMaxHealth() { return maxHp; }
    int getSpecial() { return special; }
    int getHeals() { return heals; }
    bool isAlive() { return hp > 0; }

    void takeDamage(int dmg) {
        if (defending) {
            dmg = dmg / 2;
            defending = false;
            cout << name << " blocks half damage! Reduced to " << dmg << " damage!\n";
        }
        hp -= dmg;
        if (hp < 0) hp = 0;
        cout << name << " takes " << dmg << " damage! HP: " << hp << "/" << maxHp << "\n";
    }

    void attack(Character* target) {
        cout << name << " attacks " << target->getName() << " for " << damage << " damage!\n";
        target->takeDamage(damage);
    }

    void specialAttack(Character* target) {
        if (special >= 3) {
            int dmg = damage * 2;
            cout << name << " uses SPECIAL ATTACK on " << target->getName() << " for " << dmg << " damage!\n";
            target->takeDamage(dmg);
            special = 0;
        }
        else {
            cout << "Special not ready! (" << special << "/3)\n";
        }
    }

    void defend() {
        defending = true;
        cout << name << " assumes a defensive stance!\n";
    }

    void heal() {
        if (heals > 0) {
            int healAmount = 25;
            hp += healAmount;
            if (hp > maxHp) hp = maxHp;
            heals--;
            cout << name << " heals for " << healAmount << " HP! Current HP: " << hp << "/" << maxHp << " (Heals remaining: " << heals << ")\n";
        }
        else {
            cout << name << " has no heals left!\n";
        }
    }

    void gainSpecial() {
        if (special < 3) {
            special++;
        }
    }

    void addHealth(int h) {
        hp += h;
        if (hp > maxHp) hp = maxHp;
        cout << name << " recovers " << h << " HP! Current HP: " << hp << "/" << maxHp << "\n";
    }

    void addDamage(int d) {
        damage += d;
        cout << name << " gains +" << d << " damage! New damage: " << damage << "\n";
    }

   
};

void HealItem::use(Character* target) {
    target->addHealth(30);
}

void RageItem::use(Character* target) {
    target->addDamage(10);
}

class Inventory {
private:
    Item* items[3];
    int count;

public:
    Inventory() : count(0) {
        for (int i = 0; i < 3; i++) items[i] = nullptr;
    }

    ~Inventory() {
        for (int i = 0; i < count; i++) {
            if (items[i]) delete items[i];
        }
    }

    bool addItem(Item* item) {
        if (count < 3) {
            items[count++] = item;
            cout << "Found " << item->getName() << "!\n";
            return true;
        }
        cout << "Inventory full!\n";
        delete item;
        return false;
    }

    bool useItem(int index, Character* user) {
        if (index >= 0 && index < count && items[index]) {
            cout << "Using " << items[index]->getName() << "...\n";
            items[index]->use(user);
            delete items[index];
            for (int i = index; i < count - 1; i++) {
                items[i] = items[i + 1];
            }
            items[count - 1] = nullptr;
            count--;
            return true;
        }
        return false;
    }

    void display() {
        cout << "\n=== INVENTORY ===\n";
        if (count == 0) {
            cout << "Empty\n";
        }
        else {
            for (int i = 0; i < count; i++) {
                cout << (i + 1) << ". " << items[i]->getName() << "\n";
            }
        }
        cout << "\n";
    }

    int getCount() { return count; }
};

class BarbarianKing : public Character {
public:
    BarbarianKing() : Character("Barbarian King", 120, 15) {}
};

class ArcherQueen : public Character {
public:
    ArcherQueen() : Character("Archer Queen", 100, 18) {}
};

class MinionPrince : public Character {
public:
    MinionPrince() : Character("Minion Prince", 90, 16) {}
};

class RoyalChampion : public Character {
public:
    RoyalChampion() : Character("Royal Champion", 110, 17) {}
};

class GrandWarden : public Character {
public:
    GrandWarden() : Character("Grand Warden", 105, 14) {}
};

class Ghost : public Character {
public:
    Ghost() : Character("Ghost", 95, 19) {}
};

class Goblin : public Character {
public:
    Goblin() : Character("Goblin", 50, 10) {}
};

class Orc : public Character {
public:
    Orc() : Character("Orc", 70, 12) {}
};

class Dragon : public Character {
public:
    Dragon() : Character("Dragon", 90, 15) {}
};

class Skeleton : public Character {
public:
    Skeleton() : Character("Skeleton", 60, 11) {}
};

class Troll : public Character {
public:
    Troll() : Character("Troll", 80, 13) {}
};

class Demon : public Character {
public:
    Demon() : Character("Demon", 85, 14) {}
};

class FinalBoss : public Character {
public:
    FinalBoss() : Character("Dark Overlord", 150, 20) {}
};

class Player {
private:
    string name;
    Character* character;
    Inventory inventory;

public:
    Player(string n) : name(n), character(nullptr) {}
    ~Player() {
        if (character) delete character;
    }

    void setCharacter(Character* c) {
        if (character) delete character;
        character = c;
    }

    Character* getCharacter() { return character; }
    string getName() { return name; }
    Inventory* getInventory() { return &inventory; }
};

class PuzzleRoom {
private:
    bool mines[5][5];
    bool visited[5][5];
    char display[5][5];

    void printGrid(bool revealMines = false) {
        cout << "\n    0   1   2   3   4\n";
        cout << "  +---+---+---+---+---+\n";
        for (int i = 0; i < 5; ++i) {
            cout << i << " |";
            for (int j = 0; j < 5; ++j) {
                if (revealMines && mines[i][j]) {
                    cout << " B";
                }
                else {
                    cout << " " << display[i][j] << " |";
                }
            }
            cout << "\n  +---+---+---+---+---+\n";
        }
    }

public:
    PuzzleRoom() {
        srand(time(nullptr));
        for (int i = 0; i < 5; ++i)
            for (int j = 0; j < 5; ++j) {
                mines[i][j] = false;
                visited[i][j] = false;
                display[i][j] = '?';
            }
    }

    bool play() {
        cout << "\n=== MINESWEEPER PUZZLE ===\n";
        cout << "Find 5 safe spots! Avoid the mines!\n";
        cout << "You have 8 attempts to find 5 safe spots.\n";

        int minesPlaced = 0;
        while (minesPlaced < 5) {
            int row = rand() % 5;
            int col = rand() % 5;
            if (!mines[row][col]) {
                mines[row][col] = true;
                minesPlaced++;
            }
        }

        int safeCells = 0;
        int attempts = 0;

        while (attempts < 8 && safeCells < 5) {
            printGrid();
            int row, col;
            cout << "\nAttempt " << (attempts + 1) << ": Enter row (0-4) and column (0-4): ";
            cin >> row >> col;

            if (row < 0 || row >= 5 || col < 0 || col >= 5) {
                cout << "Invalid input! Try coordinates between 0 and 4.\n";
                continue;
            }

            if (visited[row][col]) {
                cout << "You already checked this spot!\n";
                continue;
            }

            visited[row][col] = true;
            attempts++;

            if (mines[row][col]) {
                display[row][col] = 'X';
                printGrid(true);
                cout << "BOOM! You hit a mine at (" << row << "," << col << ")! Game Over.\n";
                return false;
            }
            else {
                display[row][col] = 'S';
                safeCells++;
                cout << "Safe! Progress: " << safeCells << "/5\n";
            }
        }

        printGrid(true); 

        if (safeCells >= 5) {
            cout << " Success! You found 5 safe spots and won the game!\n";
            return true;
        }
        else {
            cout << " Failed! You ran out of attempts.\n";
            return false;
        }
    }
};

class GameMap {
private:
    static const int ROWS = 10;
    static const int COLS = 12;
    char map[ROWS][COLS];
    int playerRow, playerCol;

public:
    GameMap() : playerRow(1), playerCol(1) {
        char initialMap[ROWS][COLS] = {
            {'#','#','#','#','#','#','#','#','#','#','#','#'},
            {'#','P',' ','M',' ','M',' ','M',' ','M',' ','#'},
            {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
            {'#','M',' ','M',' ','?',' ','M',' ','M',' ','#'},
            {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
            {'#','M',' ','M',' ','T',' ','M',' ','M',' ','#'},
            {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','#'},
            {'#','M',' ','M',' ','M',' ','M',' ','M',' ','#'},
            {'#',' ',' ',' ',' ',' ',' ',' ',' ',' ','B','#'},
            {'#','#','#','#','#','#','#','#','#','#','#','#'}
        };

        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                map[i][j] = initialMap[i][j];
            }
        }
    }

    void display() {
        cout << "\n=== DUNGEON MAP ===\n";
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                cout << map[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\nP=Player, M=Monster, T=Treasure, ?=Puzzle, B=Boss\n\n";
    }

    bool movePlayer(char direction) {
        int newRow = playerRow;
        int newCol = playerCol;

        switch (direction) {
        case 'w': case 'W': newRow--; break;
        case 's': case 'S': newRow++; break;
        case 'a': case 'A': newCol--; break;
        case 'd': case 'D': newCol++; break;
        default: return false;
        }

        if (newRow >= 0 && newRow < ROWS && newCol >= 0 && newCol < COLS && map[newRow][newCol] != '#') {
            map[playerRow][playerCol] = ' ';

            playerRow = newRow;
            playerCol = newCol;
            return true;
        }
        return false;
    }
    
    char getTileAt(int row, int col) {
        if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
            return map[row][col];
        }
        return '#';
    }

    char getCurrentTile() {
        return map[playerRow][playerCol];
    }

    void clearCurrentTile() {
        map[playerRow][playerCol] = ' ';
    }

    void setPlayerOnMap() {
        map[playerRow][playerCol] = 'P';
    }

    int getPlayerRow() { return playerRow; }
    int getPlayerCol() { return playerCol; }

    string getCurrentRoom() {
        return to_string(playerRow) + "," + to_string(playerCol);
    }
};

class Game {
private:
    Player* player;
    FinalBoss* boss;
    GameMap gameMap;
    bool gameWon;
    bool gameOver;
    bool puzzleSolved;
    int monstersDefeated;

    static const int REQUIRED_MONSTERS = 6;

public:
    Game() : player(nullptr), boss(nullptr), gameWon(false), gameOver(false), puzzleSolved(false), monstersDefeated(0) {
        srand(time(nullptr));
    }

    ~Game() {
        if (player) delete player;
        if (boss) delete boss;
    }

    void start() {
        cout << "=== DUNGEON CRAWLER RPG ===\n\n";

        string playerName;
        cout << "Enter your name: ";
        getline(cin, playerName);
        player = new Player(playerName);

        selectCharacter();
        boss = new FinalBoss();

        cout << "\nWelcome, " << player->getName() << "!\n";
        cout << "Mission: Defeat the Dark Overlord!\n";
        cout << "Requirements:\n";
        cout << "1. Solve the puzzle (?)\n";
        cout << "2. Defeat " << REQUIRED_MONSTERS << " monsters\n\n";

        gameLoop();
    }

    void selectCharacter() {
        cout << "\nSelect character:\n";
        cout << "1. Barbarian King (HP: 120, Damage: 15)\n";
        cout << "2. Archer Queen (HP: 100, Damage: 18)\n";
        cout << "3. Minion Prince (HP: 90, Damage: 16)\n";
        cout << "4. Royal Champion (HP: 110, Damage: 17)\n";
        cout << "5. Grand Warden (HP: 105, Damage: 14)\n";
        cout << "6. Ghost (HP: 95, Damage: 19)\n";

        int choice;
        cout << "Choice (1-6): ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: player->setCharacter(new BarbarianKing()); break;
        case 2: player->setCharacter(new ArcherQueen()); break;
        case 3: player->setCharacter(new MinionPrince()); break;
        case 4: player->setCharacter(new RoyalChampion()); break;
        case 5: player->setCharacter(new GrandWarden()); break;
        case 6: player->setCharacter(new Ghost()); break;
        default:
            cout << "Invalid choice, defaulting to Barbarian King.\n";
            player->setCharacter(new BarbarianKing());
            break;
        }

        cout << "You selected: " << player->getCharacter()->getName() << "\n";
    }

    void gameLoop() {
        while (!gameOver) {
            
            showProgress();
            handleCurrentTile();
            gameMap.display();
            if (!gameOver && player->getCharacter()->isAlive()) {
                playerTurn();
            }
        }

        if (gameWon) {
            cout << "\n VICTORY! You defeated the Dark Overlord! \n";
        }
        else {
            cout << "\n GAME OVER \n";
        }
    }

    void showProgress() {
        cout << "=== PROGRESS ===\n";
        cout << "Player: " << player->getCharacter()->getName() << "\n";
        cout << "HP: " << player->getCharacter()->getHealth() << "/" << player->getCharacter()->getMaxHealth() << "\n";
        cout << "Puzzle: " << (puzzleSolved ? " SOLVED" : " NOT SOLVED") << "\n";
        cout << "Monsters Defeated: " << monstersDefeated << "/" << REQUIRED_MONSTERS << "\n";
        cout << "Boss Status: " << (canFightBoss() ? " UNLOCKED!" : " LOCKED") << "\n\n";
    }

    bool canFightBoss() {
        return puzzleSolved && monstersDefeated >= REQUIRED_MONSTERS;
    }

    void handleCurrentTile() {
        char currentTile = gameMap.getCurrentTile();

        switch (currentTile) {
        case 'M':
            cout << "A monster blocks your path!\n";
            fightMonster();
            if (player->getCharacter()->isAlive()) {
                gameMap.clearCurrentTile(); 
                monstersDefeated++;
                cout << "Monster defeated! Room cleared!\n";
            }
            break;

        case '?':
            if (!puzzleSolved) {
                solvePuzzle();
                if (puzzleSolved) {
                    gameMap.clearCurrentTile();
                }
            }
            else {
                cout << "The puzzle has already been solved.\n";
            }
            break;

        case 'B':
            if (canFightBoss()) {
                fightBoss();
            }
            else {
                cout << "The Dark Overlord is sealed behind magical barriers!\n";
                cout << "Complete all requirements first!\n";
            }
            break;

        case 'T':
            cout << "You found a treasure chest!\n";
            findTreasure();
            gameMap.clearCurrentTile();
            break;

        case ' ':
            cout << "You are in an empty corridor.\n";
            break;

        default:
            break;
        }

        gameMap.setPlayerOnMap();
    }



    void playerTurn() {
        if (!player->getCharacter()->isAlive()) {
            gameOver = true;
            return;
        }

        cout << "=== YOUR TURN ===\n";
        cout << "HP: " << player->getCharacter()->getHealth() << "/" << player->getCharacter()->getMaxHealth();
        cout << " | Special: " << player->getCharacter()->getSpecial() << "/3";
        cout << " | Heals: " << player->getCharacter()->getHeals() << "\n";
        cout << "Commands: [W]up [S]down [A]left [D]right [I]Inventory [Q]Quit\n";
        cout << "Enter command: ";

        char input;
        cin >> input;
        cin.ignore();

        switch (input) {
        case 'q': case 'Q':
            cout << "Thanks for playing!\n";
            gameOver = true;
            break;
        case 'i': case 'I':
            useInventory();
            break;
        case 'w': case 'W':
        case 's': case 'S':
        case 'a': case 'A':
        case 'd': case 'D':
            if (!gameMap.movePlayer(input)) {
                cout << "Can't move that way!\n";
            }
            break;
        default:
            cout << "Invalid command!\n";
            break;
        }
    }

    void useInventory() {
        player->getInventory()->display();
        if (player->getInventory()->getCount() > 0) {
            cout << "Use item (1-" << player->getInventory()->getCount() << ") or 0 to cancel: ";
            int choice;
            cin >> choice;
            cin.ignore();
            if (choice > 0 && choice <= player->getInventory()->getCount()) {
                player->getInventory()->useItem(choice - 1, player->getCharacter());
            }
            else if (choice != 0) {
                cout << "Invalid choice!\n";
            }
        }
    }

    void fightMonster() {
        int type = rand() % 6;
        Character* monster = nullptr;

        switch (type) {
        case 0: monster = new Goblin(); break;
        case 1: monster = new Orc(); break;
        case 2: monster = new Dragon(); break;
        case 3: monster = new Skeleton(); break;
        case 4: monster = new Troll(); break;
        case 5: monster = new Demon(); break;
        }

        cout << "\n COMBAT BEGINS! \n";
        cout << "A wild " << monster->getName() << " appears!\n";
        cout << monster->getName() << " HP: " << monster->getHealth() << "/" << monster->getMaxHealth() << "\n\n";

        combat(monster);
        delete monster;
    }

    void fightBoss() {
        cout << "\n FINAL BATTLE! \n";
        cout << "The Dark Overlord emerges from the shadows!\n";
        cout << "\"You dare challenge me, mortal?\"\n\n";
        combat(boss);
        if (!boss->isAlive()) {
            gameWon = true;
            gameOver = true;
        }
    }

    void combat(Character* enemy) {
        cout << "=== COMBAT START ===\n";

        while (enemy->isAlive() && player->getCharacter()->isAlive()) {
            cout << "\n--- BATTLE STATUS ---\n";
            cout << " " << player->getCharacter()->getName() << " HP: " << player->getCharacter()->getHealth() << "/" << player->getCharacter()->getMaxHealth() << "\n";
            cout << " " << enemy->getName() << " HP: " << enemy->getHealth() << "/" << enemy->getMaxHealth() << "\n";
            cout << "Special Meter: " << player->getCharacter()->getSpecial() << "/3\n";
            cout << "Heals Remaining: " << player->getCharacter()->getHeals() << "\n\n";

            cout << "Choose your action:\n";
            cout << "1.  Attack (" << player->getCharacter()->getName() << " deals damage)\n";
            cout << "2.  Special Attack (" << (player->getCharacter()->getSpecial() >= 3 ? "READY!" : "Not ready") << ")\n";
            cout << "3.  Defend (Reduce next damage by 50%)\n";
            cout << "4.  Heal (Restore 25 HP)\n";
            cout << "5.  Use Item\n";
            cout << "Enter choice (1-5): ";

            int choice;
            cin >> choice;
            cin.ignore();

            bool playerActed = true;
            switch (choice) {
            case 1:
                player->getCharacter()->attack(enemy);
                break;
            case 2:
                player->getCharacter()->specialAttack(enemy);
                break;
            case 3:
                player->getCharacter()->defend();
                break;
            case 4:
                player->getCharacter()->heal();
                break;
            case 5:
                useInventory();
                playerActed = false;
                continue;
            default:
                cout << "Invalid choice! Turn wasted!\n";
                break;
            }

            if (!enemy->isAlive()) {
                cout << "\n " << enemy->getName() << " has been defeated! \n";
                break;
            }

            cout << "\n--- ENEMY TURN ---\n";
            int enemyChoice = rand() % 3; 
            switch (enemyChoice) {
            case 0:
                enemy->attack(player->getCharacter());
                break;
            case 1:
                if (enemy->getSpecial() >= 3) {
                    enemy->specialAttack(player->getCharacter());
                }
                else {
                    enemy->attack(player->getCharacter());
                }
                break;
            case 2:
                enemy->defend();
                break;
            }

            if (playerActed) {
                player->getCharacter()->gainSpecial();
            }
            enemy->gainSpecial();

            if (!player->getCharacter()->isAlive()) {
                cout << "\n You have been defeated! \n";
                gameOver = true;
                break;
            }

            cout << "\nPress Enter to continue...";
            cin.get();
        }

        if (player->getCharacter()->isAlive() && !enemy->isAlive()) {
            cout << "\n VICTORY! " << enemy->getName() << " has been defeated!\n";
            cout << "You gain experience and feel stronger!\n";
        }
    }

    void solvePuzzle() {
        cout << "\n You've discovered an ancient puzzle chamber!\n";
        cout << "The air crackles with magical energy...\n\n";

        PuzzleRoom puzzle;
        if (puzzle.play()) {
            puzzleSolved = true;
            cout << "\n PUZZLE SOLVED! \n";
            cout << "The magical seal breaks! Monsters throughout the dungeon are now vulnerable!\n";
            cout << "You receive a reward for your cleverness!\n";
            player->getInventory()->addItem(new HealItem());
        }
        else {
            cout << "\n Puzzle failed! The magical energies reject you.\n";
            cout << "You can try again by returning to this room.\n";
        }
        cout << "\nPress Enter to continue...";
        cin.get();
    }

    void findTreasure() {
        cout << "\n TREASURE DISCOVERED! \n";
        cout << "You find an ornate chest filled with magical items!\n";

        int treasureType = rand() % 2;
        if (treasureType == 0) {
            cout << "You found a glowing healing potion!\n";
            player->getInventory()->addItem(new HealItem());
        }
        else {
            cout << "You found a vial of berserker rage!\n";
            player->getInventory()->addItem(new RageItem());
        }
        cout << "\nPress Enter to continue...";
        cin.get();
    }
};

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "|--------------------------------------|\n";
    cout << "|        DUNGEON CRAWLER RPG           |\n";
    cout << "|           Adventure Awaits!          |\n";
    cout << "|--------------------------------------|\n\n";

    Game game;
    game.start();

    cout << "\nThank you for playing!\n";
    return 0;
}


