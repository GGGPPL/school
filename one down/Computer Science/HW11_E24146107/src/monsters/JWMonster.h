#ifndef JWMONSTER_H
#define JWMONSTER_H
#include "BaseMonster.h"

class JWMonster : public BaseMonster {
public:
    JWMonster();
    virtual string serialize() override;
    virtual Item* dropItem() override;
    static BaseMonster* unserialize(string data);
};
#endif