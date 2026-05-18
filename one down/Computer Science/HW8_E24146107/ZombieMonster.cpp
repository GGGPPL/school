#include "ZombieMonster.h"

ZombieMonster::ZombieMonster() 
    : BaseMonster("Zombie", 50, 65, 17, 65, 150, 30) { // 套用表格數值 
    setHP(max_hp);
    setMP(max_mp);
}

string ZombieMonster::serialize() {
    return "ZombieMonster," + to_string(getHP()) + "," + to_string(getMP());
}

BaseMonster* ZombieMonster::unserialize(string data) {
    stringstream ss(data);
    string type, val;
    getline(ss, type, ','); // 讀取類別名稱
    ZombieMonster* m = new ZombieMonster();
    if(getline(ss, val, ',')) m->setHP(stoi(val));
    if(getline(ss, val, ',')) m->setMP(stoi(val));
    return m; // 回傳基底類別指標 [cite: 233]
}