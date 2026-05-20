#include "OrcPlayer.h"
#include <cmath>

OrcPlayer::OrcPlayer() : NovicePlayer() { 
    setLevel(1); setHp(maxhp); setMp(maxmp); 
}
OrcPlayer::OrcPlayer(int l) : NovicePlayer(l) { 
    setLevel(l); setHp(maxhp); setMp(maxmp); 
}
OrcPlayer::OrcPlayer(int l, string n) : NovicePlayer(l, n) { 
    setLevel(l); setHp(maxhp); setMp(maxmp); 
}
OrcPlayer::OrcPlayer(const OrcPlayer& other) : NovicePlayer(other) {}

void OrcPlayer::setAttr() {
    maxhp = 200 + 20 * level; 
    maxmp = 50 + 5 * level;
    attack = 50 + 12 * level; 
    defense = 30 + 10 * level;
    lvupexp = ceil(pow(10, log2(level + 1)));
    
    backpack_weight_limit = 150;
    backpack_slot_limit = 10;
    if (backpack == nullptr) {
        backpack = new Item*[backpack_slot_limit];
        for (int i = 0; i < backpack_slot_limit; ++i) {
            backpack[i] = nullptr;
        }
    }
}

void OrcPlayer::setLevel(int l) {
    level = l; 
    setAttr();
    if (getHp() > maxhp) setHp(maxhp); 
    if (getMp() > maxmp) setMp(maxmp);
}

string OrcPlayer::serialize() {
    return "OrcPlayer," + getName() + "," + to_string(level) + "," + to_string(getHp()) + "," + to_string(getMp()) + "," + to_string(getExp()) + "," + to_string(getMoney());
}

NovicePlayer* OrcPlayer::unserialize(string data) {
    stringstream ss(data); string t, n, v;
    getline(ss, t, ','); getline(ss, n, ',');
    OrcPlayer* p = new OrcPlayer(1, n);
    getline(ss, v, ','); p->setLevel(stoi(v));
    getline(ss, v, ','); p->setHp(stoi(v));
    getline(ss, v, ','); p->setMp(stoi(v));
    getline(ss, v, ','); p->setExp(stoi(v));
    getline(ss, v, ','); p->setMoney(stoi(v));
    return p;
}