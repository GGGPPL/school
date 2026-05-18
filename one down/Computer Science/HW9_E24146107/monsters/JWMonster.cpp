#include "JWMonster.h"

JWMonster::JWMonster() 
    : BaseMonster("JWMaster", 120, 100, 42, 175, 250, 100) { // 套用表格數值 
    setHP(max_hp);
    setMP(max_mp);
}

string JWMonster::serialize() {
    return "JWMonster," + to_string(getHP()) + "," + to_string(getMP());
}

BaseMonster* JWMonster::unserialize(string data) {
    stringstream ss(data);
    string type, val;
    getline(ss, type, ',');
    JWMonster* m = new JWMonster();
    if(getline(ss, val, ',')) m->setHP(stoi(val));
    if(getline(ss, val, ',')) m->setMP(stoi(val));
    return m;
}