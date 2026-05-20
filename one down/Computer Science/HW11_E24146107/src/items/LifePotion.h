#ifndef LIFEPOTION_H
#define LIFEPOTION_H
#include "ConsumableItem.h"
class LifePotion : public ConsumableItem {
public:
    LifePotion();
    virtual void use(NovicePlayer* player) override;
};
#endif
