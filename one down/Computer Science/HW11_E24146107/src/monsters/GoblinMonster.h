#ifndef GOBLINMONSTER_H
#define GOBLINMONSTER_H
#include "BaseMonster.h"

class GoblinMonster : public BaseMonster {
public:
    GoblinMonster();
    virtual string serialize() override;
    virtual Item* dropItem() override;
    static BaseMonster* unserialize(string data);
};
#endif