#ifndef BATTLE_H
#define BATTLE_H

#include "../players/NovicePlayer.h"
#include "../monsters/BaseMonster.h"
#include <vector>

struct Character {
    char type; // 'P' for Player, 'M' for Monster
    bool alive;
    void* instance;
};

class Battle {
private:
    int numPlayers;
    int numMonsters;
    int totalRounds;
    Character* ActionList;
    int actionListSize;

    NovicePlayer** players;
    BaseMonster** monsters;

public:
    Battle(NovicePlayer** p, int pCount, BaseMonster** m, int mCount);
    ~Battle();

    void startBattle();
    bool checkWin();
    bool checkLoss();
    void processTurn();
    void updateStatus();
    void reward();
};

#endif
