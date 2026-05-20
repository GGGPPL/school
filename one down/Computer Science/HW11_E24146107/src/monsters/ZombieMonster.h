#ifndef ZOMBIEMONSTER_H
#define ZOMBIEMONSTER_H
#include "BaseMonster.h"

class ZombieMonster : public BaseMonster {
public:
    ZombieMonster();
    virtual string serialize() override; // 實作序列化 [cite: 225]
    virtual Item* dropItem() override;
    static BaseMonster* unserialize(string data); // 實作反序列化 [cite: 232]
};
#endif