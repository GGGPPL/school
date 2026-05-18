#include "NovicePlayer.h"
#include <cmath>

// 預設建構子
NovicePlayer::NovicePlayer() {
    name = "anonymous";
    exp = 0;
    money = 0;
    setLevel(1);
    hp = maxhp;
    mp = maxmp;
}

// 帶有初始等級的建構子
NovicePlayer::NovicePlayer(int l) {
    name = "anonymous";
    exp = 0;
    money = 0;
    setLevel(l);
    hp = maxhp;
    mp = maxmp;
}

// 帶有初始等級與名字的建構子
NovicePlayer::NovicePlayer(int l, string n) {
    name = n;
    exp = 0;
    money = 0;
    setLevel(l);
    hp = maxhp;
    mp = maxmp;
}

// 拷貝建構子
NovicePlayer::NovicePlayer(const NovicePlayer& other) {
    name = other.name;
    hp = other.hp;
    mp = other.mp;
    exp = other.exp;
    money = other.money;
    level = other.level;
    attack = other.attack;
    defense = other.defense;
    maxhp = other.maxhp;
    maxmp = other.maxmp;
    lvupexp = other.lvupexp;
}

void NovicePlayer::setName(string n) {
    name = n;
}

string NovicePlayer::getName() const {
    return name;
}

// 設定等級並自動計算所有相關能力值
void NovicePlayer::setLevel(int l) {
    level = l;
    maxhp = 100 + 10 * level;
    maxmp = 40 + 5 * level;
    attack = 20 + 5 * level;
    defense = 20 + 5 * level;
    lvupexp = ceil(pow(10, log2(level + 1)));

    // 確保當前數值不超過新的上限
    if (hp > maxhp) hp = maxhp;
    if (mp > maxmp) mp = maxmp;
    if (exp > lvupexp) exp = lvupexp;
}

int NovicePlayer::getLevel() const {
    return level;
}

void NovicePlayer::setHp(int h) {
    hp = h;
    if (hp > maxhp) hp = maxhp;
    if (hp < 0) hp = 0;
}

int NovicePlayer::getHp() const {
    return hp;
}

void NovicePlayer::setMp(int m) {
    mp = m;
    if (mp > maxmp) mp = maxmp;
    if (mp < 0) mp = 0;
}

int NovicePlayer::getMp() const {
    return mp;
}

void NovicePlayer::setExp(int e) {
    exp = e;
    if (exp > lvupexp) exp = lvupexp;
    if (exp < 0) exp = 0;
}

int NovicePlayer::getExp() const {
    return exp;
}

void NovicePlayer::setMoney(int m) {
    money = m;
    if (money < 0) money = 0;
}

int NovicePlayer::getMoney() const {
    return money;
}

int NovicePlayer::getAttack() const {
    return attack;
}

int NovicePlayer::getDefense() const {
    return defense;
}

int NovicePlayer::getMaxHP() const {
    return maxhp;
}

int NovicePlayer::getMaxMP() const {
    return maxmp;
}

int NovicePlayer::getLvupExp() const {
    return lvupexp;
}