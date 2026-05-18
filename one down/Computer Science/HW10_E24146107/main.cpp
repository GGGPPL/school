#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "src/players/KnightPlayer.h"
#include "src/players/MagicianPlayer.h"
#include "src/players/OrcPlayer.h"
#include "src/monsters/GoblinMonster.h"
#include "src/monsters/ZombieMonster.h"
#include "src/monsters/JWMonster.h"
#include "src/misc/Battle.h"
#include "src/field/Field.h"

using namespace std;

void triggerBattle(NovicePlayer** players, int numPlayers) {
    cout << "\n!!! An enemy appears !!!" << endl;
    
    // Randomly generate 1-3 monsters
    int numMonsters = rand() % 3 + 1;
    BaseMonster** monsters = new BaseMonster*[numMonsters];
    for (int i = 0; i < numMonsters; i++) {
        int r = rand() % 3;
        if (r == 0) monsters[i] = new GoblinMonster();
        else if (r == 1) monsters[i] = new ZombieMonster();
        else monsters[i] = new JWMonster();
    }

    Battle battle(players, numPlayers, monsters, numMonsters);
    battle.startBattle();

    for (int i = 0; i < numMonsters; i++) delete monsters[i];
    delete[] monsters;
    cout << "\nReturning to exploration..." << endl;
}

int main() {
    srand(time(0));
    
    cout << "========================================" << endl;
    cout << "       WELCOME TO THE RPG WORLD         " << endl;
    cout << "========================================" << endl;
    cout << "Story: You are brave heroes sent to the" << endl;
    cout << "Forest of Trials to defeat the dark forces." << endl;
    cout << "Good luck!" << endl;

    // Setup Players
    int numPlayers = 2;
    NovicePlayer** players = new NovicePlayer*[numPlayers];
    players[0] = new KnightPlayer(1, "Arthur");
    players[1] = new MagicianPlayer(1, "Merlin");

    // Setup Field (Map file, initial x, initial y, vision width, vision height)
    Field field("common/map1.txt", 1, 1, 7, 7);

    bool running = true;
    while (running) {
        field.display();
        cout << "\nCommands: [W/A/S/D] Move, [Q] Quit Game" << endl;
        cout << "Input: ";
        char cmd;
        cin >> cmd;

        if (cmd == 'q' || cmd == 'Q') {
            running = false;
            break;
        }

        if (field.move(cmd)) {
            int x = field.getCurrentPositionX();
            int y = field.getCurrentPositionY();
            int symbol = field.getMapSymbol(x, y);

            // Check for encounter
            if (symbol == 3) {
                cout << "\n[BOSS EVENT] You encountered a visible monster group!" << endl;
                triggerBattle(players, numPlayers);
                field.setMapSymbol(0, x, y); // Remove monster from map after battle
            } else {
                // Random encounter chance (15%)
                if (rand() % 100 < 15) {
                    triggerBattle(players, numPlayers);
                }
            }
        } else {
            cout << "Ouch! You hit a wall or edge." << endl;
        }
    }

    // Cleanup
    for (int i = 0; i < numPlayers; i++) delete players[i];
    delete[] players;

    cout << "\nThank you for playing!" << endl;
    return 0;
}
