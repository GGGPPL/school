#include "BaseMonster.h"

int BaseMonster::count = 0; //

BaseMonster::BaseMonster(string n, int atk, int def, int e, int m, int mhp, int mmp)
    : name(n), attack(atk), defense(def), exp(e), money(m), max_hp(mhp), max_mp(mmp) {
    count++; //
}

BaseMonster::~BaseMonster() { count--; } //

void BaseMonster::setHP(int h) { hp = (h < 0) ? 0 : (h > max_hp ? max_hp : h); }
int BaseMonster::getHP() const { return hp; }
void BaseMonster::setMP(int m) { mp = (m < 0) ? 0 : (m > max_mp ? max_mp : m); }
int BaseMonster::getMP() const { return mp; }
int BaseMonster::getInstanceCount() { return count; }

Item* BaseMonster::dropItem() {
    return nullptr;
}