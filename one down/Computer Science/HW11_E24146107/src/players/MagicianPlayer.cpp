#include "MagicianPlayer.h"
#include <cmath>

MagicianPlayer::MagicianPlayer() : NovicePlayer() { 
    setLevel(1); setHp(maxhp); setMp(maxmp); 
}
MagicianPlayer::MagicianPlayer(int l) : NovicePlayer(l) { 
    setLevel(l); setHp(maxhp); setMp(maxmp); 
}
MagicianPlayer::MagicianPlayer(int l, string n) : NovicePlayer(l, n) { 
    setLevel(l); setHp(maxhp); setMp(maxmp); 
}
MagicianPlayer::MagicianPlayer(const MagicianPlayer& other) : NovicePlayer(other) {}

void MagicianPlayer::setAttr() {
    maxhp = 120 + 15 * level; 
    maxmp = 100 + 15 * level;
    attack = 30 + 8 * level; 
    defense = 20 + 7 * level;
    lvupexp = ceil(pow(10, log2(level + 1)));
    
    backpack_weight_limit = 30;
    backpack_slot_limit = 20;
    if (backpack == nullptr) {
        backpack = new Item*[backpack_slot_limit];
        for (int i = 0; i < backpack_slot_limit; ++i) {
            backpack[i] = nullptr;
        }
    }
}

void MagicianPlayer::setLevel(int l) {
    level = l; 
    setAttr();
    if (getHp() > maxhp) setHp(maxhp); 
    if (getMp() > maxmp) setMp(maxmp);
}

void MagicianPlayer::specialSkill() { // 原 pray 邏輯
    if (getHp() >= level * 5) {
        setHp(getHp() - level * 5);
        setMp(getMp() + level * 10);
    }
}

string MagicianPlayer::serialize() {
    return "MagicianPlayer," + getName() + "," + to_string(level) + "," + to_string(getHp()) + "," + to_string(getMp()) + "," + to_string(getExp()) + "," + to_string(getMoney());
}

NovicePlayer* MagicianPlayer::unserialize(string data) {
    stringstream ss(data); string t, n, v;
    getline(ss, t, ','); getline(ss, n, ',');
    MagicianPlayer* p = new MagicianPlayer(1, n);
    getline(ss, v, ','); p->setLevel(stoi(v));
    getline(ss, v, ','); p->setHp(stoi(v));
    getline(ss, v, ','); p->setMp(stoi(v));
    getline(ss, v, ','); p->setExp(stoi(v));
    getline(ss, v, ','); p->setMoney(stoi(v));
    return p;
}