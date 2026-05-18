#ifndef JWMONSTER_H
#define JWMONSTER_H
#include "BaseMonster.h"

class JWMonster : public BaseMonster {
public:
    JWMonster();
    virtual string serialize() override;
    static BaseMonster* unserialize(string data);
};
#endif