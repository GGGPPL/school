#include "Battle.h"
#include <iostream>
#include <iomanip>

using namespace std;

Battle::Battle(NovicePlayer** p, int pCount, BaseMonster** m, int mCount) 
    : players(p), numPlayers(pCount), monsters(m), numMonsters(mCount), totalRounds(0) {
    actionListSize = numPlayers + numMonsters;
    ActionList = new Character[actionListSize];
    
    // Initialize ActionList: Players then Monsters as per ID
    for (int i = 0; i < numPlayers; ++i) {
        ActionList[i].type = 'P';
        ActionList[i].alive = (players[i]->getHp() > 0);
        ActionList[i].instance = players[i];
    }
    for (int i = 0; i < numMonsters; ++i) {
        ActionList[numPlayers + i].type = 'M';
        ActionList[numPlayers + i].alive = (monsters[i]->getHP() > 0);
        ActionList[numPlayers + i].instance = monsters[i];
    }
}

Battle::~Battle() {
    delete[] ActionList;
}

void Battle::startBattle() {
    cout << "========== Battle Start! ==========" << endl;
    while (!checkWin() && !checkLoss()) {
        totalRounds++;
        cout << "\n--- Round " << totalRounds << " ---" << endl;
        processTurn();
        updateStatus();
    }

    if (checkWin()) {
        cout << "\nVictory! All monsters defeated." << endl;
        reward();
    } else {
        cout << "\nGame Over! All players defeated." << endl;
        exit(0); // Ends the game as per requirement
    }
}

bool Battle::checkWin() {
    for (int i = 0; i < numMonsters; ++i) {
        if (monsters[i]->getHP() > 0) return false;
    }
    return true;
}

bool Battle::checkLoss() {
    for (int i = 0; i < numPlayers; ++i) {
        if (players[i]->getHp() > 0) return false;
    }
    return true;
}

void Battle::processTurn() {
    for (int i = 0; i < actionListSize; ++i) {
        if (!ActionList[i].alive) continue;

        if (ActionList[i].type == 'P') {
            NovicePlayer* p = static_cast<NovicePlayer*>(ActionList[i].instance);
            if (p->getHp() <= 0) continue;

            cout << "Player " << p->getName() << "'s turn. (HP: " << p->getHp() << ", MP: " << p->getMp() << ")" << endl;
            cout << "Choose action: 1. Normal Attack  2. Special Skill: ";
            int choice;
            cin >> choice;

            if (choice == 2) {
                p->specialSkill();
                cout << p->getName() << " used special skill!" << endl;
            } else {
                // Find target: first alive monster
                BaseMonster* target = nullptr;
                for (int j = 0; j < numMonsters; ++j) {
                    if (monsters[j]->getHP() > 0) {
                        target = monsters[j];
                        break;
                    }
                }
                if (target) {
                    int damage = p->getAttack() - target->defense;
                    if (damage < 0) damage = 0;
                    target->setHP(target->getHP() - damage);
                    cout << p->getName() << " attacked " << target->name << " for " << damage << " damage!" << endl;
                }
            }
        } else {
            BaseMonster* m = static_cast<BaseMonster*>(ActionList[i].instance);
            if (m->getHP() <= 0) continue;

            // Monster attack first alive player
            NovicePlayer* target = nullptr;
            for (int j = 0; j < numPlayers; ++j) {
                if (players[j]->getHp() > 0) {
                    target = players[j];
                    break;
                }
            }

            if (target) {
                int damage = m->attack - target->getDefense();
                if (damage < 0) damage = 0;
                target->setHp(target->getHp() - damage);
                cout << m->name << " attacked " << target->getName() << " for " << damage << " damage!" << endl;
            }
        }
        
        // Break early if battle ended mid-turn
        if (checkWin() || checkLoss()) break;
    }
}

void Battle::updateStatus() {
    for (int i = 0; i < numPlayers; ++i) {
        ActionList[i].alive = (players[i]->getHp() > 0);
    }
    for (int i = 0; i < numMonsters; ++i) {
        ActionList[numPlayers + i].alive = (monsters[i]->getHP() > 0);
    }
}

void Battle::reward() {
    int totalExp = 0;
    int totalMoney = 0;
    for (int i = 0; i < numMonsters; ++i) {
        totalExp += monsters[i]->exp;
        totalMoney += monsters[i]->money;
    }

    cout << "Total Reward: " << totalExp << " EXP, " << totalMoney << " Money." << endl;

    for (int i = 0; i < numPlayers; ++i) {
        // Revive dead players with 1 HP
        if (players[i]->getHp() <= 0) {
            players[i]->setHp(1);
            cout << players[i]->getName() << " has been revived with 1 HP!" << endl;
        }

        players[i]->setExp(players[i]->getExp() + totalExp);
        players[i]->setMoney(players[i]->getMoney() + totalMoney);

        // Check Level Up
        while (players[i]->getExp() >= players[i]->getLvupExp()) {
            players[i]->setExp(players[i]->getExp() - players[i]->getLvupExp());
            players[i]->setLevel(players[i]->getLevel() + 1);
            players[i]->setHp(players[i]->getMaxHP());
            players[i]->setMp(players[i]->getMaxMP());
            cout << players[i]->getName() << " Leveled Up to " << players[i]->getLevel() << "!" << endl;
        }
    }
}
