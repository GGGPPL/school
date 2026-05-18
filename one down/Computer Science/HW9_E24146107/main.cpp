#include <iostream>
#include <string>
#include <vector>
#include "players/KnightPlayer.h"
#include "players/MagicianPlayer.h"
#include "players/NovicePlayer.h"
#include "players/OrcPlayer.h"
#include "monsters/GoblinMonster.h"
#include "monsters/ZombieMonster.h"
#include "monsters/JWMonster.h"
#include "misc/Battle.h"

using namespace std;

int main()
{
    cout << "=== Game Setup ===" << endl;
    
    // Setup Players
    int numPlayers = 2;
    NovicePlayer** players = new NovicePlayer*[numPlayers];
    players[0] = new KnightPlayer(1, "Arthur");
    players[1] = new MagicianPlayer(1, "Merlin");

    // Setup Monsters
    int numMonsters = 2;
    BaseMonster** monsters = new BaseMonster*[numMonsters];
    monsters[0] = new GoblinMonster();
    monsters[1] = new ZombieMonster();

    // Start Battle
    Battle battle(players, numPlayers, monsters, numMonsters);
    battle.startBattle();

    // Cleanup
    for (int i = 0; i < numPlayers; i++) delete players[i];
    delete[] players;
    for (int i = 0; i < numMonsters; i++) delete monsters[i];
    delete[] monsters;

    return 0;
}
