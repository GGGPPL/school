#include "KnightPlayer.h"
#include <cmath>

KnightPlayer::KnightPlayer() : NovicePlayer() { 
    setLevel(1); setHp(maxhp); setMp(maxmp); 
}
KnightPlayer::KnightPlayer(int l) : NovicePlayer(l) { 
    setLevel(l); setHp(maxhp); setMp(maxmp); 
}
KnightPlayer::KnightPlayer(int l, string n) : NovicePlayer(l, n) { 
    setLevel(l); setHp(maxhp); setMp(maxmp); 
}
KnightPlayer::KnightPlayer(const KnightPlayer& other) : NovicePlayer(other) {}

void KnightPlayer::setAttr() {
    maxhp = 150 + 25 * level; 
    maxmp = 70 + 10 * level;
    attack = 40 + 10 * level; 
    defense = 20 + 12 * level;
    lvupexp = ceil(pow(10, log2(level + 1)));
    
    backpack_weight_limit = 100;
    backpack_slot_limit = 15;
    if (backpack == nullptr) {
        backpack = new Item*[backpack_slot_limit];
        for (int i = 0; i < backpack_slot_limit; ++i) {
            backpack[i] = nullptr;
        }
    }
}

void KnightPlayer::setLevel(int l) {
    level = l; 
    setAttr();
    if (getHp() > maxhp) setHp(maxhp); 
    if (getMp() > maxmp) setMp(maxmp);
}

void KnightPlayer::specialSkill() { // 原 heal 邏輯
    if (getMp() >= level * 5) {
        setMp(getMp() - level * 5);
        setHp(getHp() + level * 10);
    }
}

string KnightPlayer::serialize() {
    return "KnightPlayer," + getName() + "," + to_string(level) + "," + to_string(getHp()) + "," + to_string(getMp()) + "," + to_string(getExp()) + "," + to_string(getMoney());
}

NovicePlayer* KnightPlayer::unserialize(string data) {
    stringstream ss(data); string t, n, v;
    getline(ss, t, ','); getline(ss, n, ',');
    KnightPlayer* p = new KnightPlayer(1, n);
    getline(ss, v, ','); p->setLevel(stoi(v));
    getline(ss, v, ','); p->setHp(stoi(v));
    getline(ss, v, ','); p->setMp(stoi(v));
    getline(ss, v, ','); p->setExp(stoi(v));
    getline(ss, v, ','); p->setMoney(stoi(v));
    return p;
}