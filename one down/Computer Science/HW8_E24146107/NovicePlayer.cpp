#include "NovicePlayer.h"
#include <cmath>

NovicePlayer::NovicePlayer() : name("anonymous"), exp(0), money(0) { setLevel(1); hp = maxhp; mp = maxmp; }
NovicePlayer::NovicePlayer(int l) : name("anonymous"), exp(0), money(0) { setLevel(l); hp = maxhp; mp = maxmp; }
NovicePlayer::NovicePlayer(int l, string n) : name(n), exp(0), money(0) { setLevel(l); hp = maxhp; mp = maxmp; }
NovicePlayer::NovicePlayer(const NovicePlayer& other) {
    name = other.name; hp = other.hp; mp = other.mp; exp = other.exp; money = other.money;
    level = other.level; attack = other.attack; defense = other.defense;
    maxhp = other.maxhp; maxmp = other.maxmp; lvupexp = other.lvupexp;
}

void NovicePlayer::setName(string n) { name = n; }
string NovicePlayer::getName() const { return name; }
void NovicePlayer::setLevel(int l) {
    level = l; maxhp = 100 + 10 * level; maxmp = 40 + 5 * level;
    attack = 20 + 5 * level; defense = 20 + 5 * level;
    lvupexp = ceil(pow(10, log2(level + 1)));
    if (hp > maxhp) hp = maxhp; if (mp > maxmp) mp = maxmp;
}
int NovicePlayer::getLevel() const { return level; }
void NovicePlayer::setHp(int h) { hp = (h < 0) ? 0 : (h > maxhp ? maxhp : h); }
int NovicePlayer::getHp() const { return hp; }
void NovicePlayer::setMp(int m) { mp = (m < 0) ? 0 : (m > maxmp ? maxmp : m); }
int NovicePlayer::getMp() const { return mp; }
void NovicePlayer::setExp(int e) { exp = e; }
int NovicePlayer::getExp() const { return exp; }
void NovicePlayer::setMoney(int m) { money = m; }
int NovicePlayer::getMoney() const { return money; }
int NovicePlayer::getAttack() const { return attack; }
int NovicePlayer::getDefense() const { return defense; }
int NovicePlayer::getMaxHP() const { return maxhp; }
int NovicePlayer::getMaxMP() const { return maxmp; }
int NovicePlayer::getLvupExp() const { return lvupexp; }

string NovicePlayer::serialize() { //
    return "NovicePlayer," + name + "," + to_string(level) + "," + to_string(hp) + "," + to_string(mp) + "," + to_string(exp) + "," + to_string(money);
}

NovicePlayer* NovicePlayer::unserialize(string data) { //
    stringstream ss(data); string t, n, v;
    getline(ss, t, ','); getline(ss, n, ',');
    NovicePlayer* p = new NovicePlayer(1, n);
    getline(ss, v, ','); p->setLevel(stoi(v));
    getline(ss, v, ','); p->setHp(stoi(v));
    getline(ss, v, ','); p->setMp(stoi(v));
    getline(ss, v, ','); p->setExp(stoi(v));
    getline(ss, v, ','); p->setMoney(stoi(v));
    return p;
}

void NovicePlayer::print() {
    cout << "-------------------------" << endl;
    cout << setw(10) << "name:" << getName() << endl;
    cout << setw(10) << "hp:" << getHp() << endl;
    cout << setw(10) << "mp:" << getMp() << endl;
    cout << setw(10) << "level:" << getLevel() << endl;
    cout << "-------------------------" << endl;
}