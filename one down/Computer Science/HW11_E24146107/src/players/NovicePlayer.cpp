#include "NovicePlayer.h"
#include <cmath>
#include "../items/Item.h"
#include "../items/WeaponItem.h"
#include "../items/ArmorItem.h"
#include "../items/ConsumableItem.h"

NovicePlayer::NovicePlayer() : name("anonymous"), exp(0), money(0) { 
    weapon = nullptr; armor = nullptr; backpack = nullptr; backpack_weight = 0;
    setLevel(1); hp = maxhp; mp = maxmp; 
}

NovicePlayer::NovicePlayer(int l) : name("anonymous"), exp(0), money(0) { 
    weapon = nullptr; armor = nullptr; backpack = nullptr; backpack_weight = 0;
    setLevel(l); hp = maxhp; mp = maxmp; 
}

NovicePlayer::NovicePlayer(int l, string n) : name(n), exp(0), money(0) { 
    weapon = nullptr; armor = nullptr; backpack = nullptr; backpack_weight = 0;
    setLevel(l); hp = maxhp; mp = maxmp; 
}

NovicePlayer::NovicePlayer(const NovicePlayer& other) {
    name = other.name; hp = other.hp; mp = other.mp; exp = other.exp; money = other.money;
    level = other.level; attack = other.attack; defense = other.defense;
    maxhp = other.maxhp; maxmp = other.maxmp; lvupexp = other.lvupexp;
    
    backpack_weight_limit = other.backpack_weight_limit;
    backpack_slot_limit = other.backpack_slot_limit;
    backpack_weight = 0;
    weapon = nullptr;
    armor = nullptr;
    backpack = nullptr;
    if (other.backpack != nullptr) {
        backpack = new Item*[backpack_slot_limit];
        for (int i = 0; i < backpack_slot_limit; ++i) {
            backpack[i] = nullptr;
        }
    }
}

NovicePlayer::~NovicePlayer() {
    if (backpack != nullptr) {
        for (int i = 0; i < backpack_slot_limit; ++i) {
            if (backpack[i] != nullptr) {
                delete backpack[i];
            }
        }
        delete[] backpack;
    }
    if (weapon != nullptr) delete weapon;
    if (armor != nullptr) delete armor;
}

void NovicePlayer::setAttr() {
    maxhp = 100 + 10 * level; 
    maxmp = 40 + 5 * level;
    attack = 20 + 5 * level; 
    defense = 20 + 5 * level;
    lvupexp = ceil(pow(10, log2(level + 1)));
    
    backpack_weight_limit = 50;
    backpack_slot_limit = 10;
    if (backpack == nullptr) {
        backpack = new Item*[backpack_slot_limit];
        for (int i = 0; i < backpack_slot_limit; ++i) {
            backpack[i] = nullptr;
        }
    }
}

void NovicePlayer::setName(string n) { name = n; }
string NovicePlayer::getName() const { return name; }

void NovicePlayer::setLevel(int l) {
    level = l; 
    setAttr();
    if (hp > maxhp) hp = maxhp; 
    if (mp > maxmp) mp = maxmp;
}

int NovicePlayer::getLevel() const { return level; }
void NovicePlayer::setHp(int h) { hp = (h < 0) ? 0 : (h > maxhp ? maxhp : h); }
int NovicePlayer::getHp() const { return hp; }
void NovicePlayer::setMp(int m) { mp = (m < 0) ? 0 : (m > maxmp ? maxmp : m); }
int NovicePlayer::getMp() const { return mp; }
void NovicePlayer::setExp(int e) { exp = e; }
int NovicePlayer::getExp() const { return exp; }
void NovicePlayer::setMoney(int m) { money = (m < 0) ? 0 : m; }
int NovicePlayer::getMoney() const { return money; }
int NovicePlayer::getAttack() const { return attack; }
int NovicePlayer::getDefense() const { return defense; }
int NovicePlayer::getMaxHP() const { return maxhp; }
int NovicePlayer::getMaxMP() const { return maxmp; }
int NovicePlayer::getLvupExp() const { return lvupexp; }

bool NovicePlayer::equipWeapon(WeaponItem* w) {
    if (w == nullptr) return false;
    if (level < w->level_required) return false;
    if (weapon != nullptr) {
        if (!putItem(weapon)) return false; 
    }
    weapon = w;
    return true;
}

bool NovicePlayer::equipArmor(ArmorItem* a) {
    if (a == nullptr) return false;
    if (level < a->level_required) return false;
    if (armor != nullptr) {
        if (!putItem(armor)) return false; 
    }
    armor = a;
    return true;
}

void NovicePlayer::useConsumable(ConsumableItem* c) {
    if (c != nullptr) {
        c->use(this);
        delete c;
    }
}

bool NovicePlayer::putItem(Item* i) {
    if (i == nullptr) return false;
    if (backpack_weight + i->weight > backpack_weight_limit) return false;
    
    for (int idx = 0; idx < backpack_slot_limit; ++idx) {
        if (backpack[idx] == nullptr) {
            backpack[idx] = i;
            backpack_weight += i->weight;
            return true;
        }
    }
    return false;
}

Item* NovicePlayer::takeItem(int index) {
    if (index < 0 || index >= backpack_slot_limit) return nullptr;
    if (backpack[index] == nullptr) return nullptr;
    
    Item* i = backpack[index];
    backpack[index] = nullptr;
    backpack_weight -= i->weight;
    return i;
}

void NovicePlayer::printBackpack() const {
    cout << "Backpack: " << backpack_weight << "/" << backpack_weight_limit << " (weight), " 
         << "slots used: ";
    int used = 0;
    for (int i = 0; i < backpack_slot_limit; ++i) {
        if (backpack[i] != nullptr) used++;
    }
    cout << used << "/" << backpack_slot_limit << endl;
    for (int i = 0; i < backpack_slot_limit; ++i) {
        if (backpack[i] != nullptr) {
            cout << "[" << i << "] " << backpack[i]->name << " (w:" << backpack[i]->weight << ")" << endl;
        }
    }
}

string NovicePlayer::serialize() { 
    return "NovicePlayer," + name + "," + to_string(level) + "," + to_string(hp) + "," + to_string(mp) + "," + to_string(exp) + "," + to_string(money);
}

NovicePlayer* NovicePlayer::unserialize(string data) { 
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
    cout << setw(10) << "weapon:" << (weapon ? weapon->name : "None") << endl;
    cout << setw(10) << "armor:" << (armor ? armor->name : "None") << endl;
    printBackpack();
    cout << "-------------------------" << endl;
}
