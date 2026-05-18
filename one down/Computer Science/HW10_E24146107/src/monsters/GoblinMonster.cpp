#include "GoblinMonster.h"
#include <sstream>

GoblinMonster::GoblinMonster() : BaseMonster("Goblin", 60, 40, 12, 30, 100, 50) { //
    setHP(max_hp); setMP(max_mp);
}
string GoblinMonster::serialize() { return "GoblinMonster," + to_string(getHP()) + "," + to_string(getMP()); }
BaseMonster* GoblinMonster::unserialize(string d) {
    stringstream ss(d); string t, v; getline(ss, t, ',');
    GoblinMonster* m = new GoblinMonster();
    getline(ss, v, ','); m->setHP(stoi(v));
    getline(ss, v, ','); m->setMP(stoi(v));
    return m;
}