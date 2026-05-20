#ifndef MAGICPOTION_H
#define MAGICPOTION_H
#include "ConsumableItem.h"
class MagicPotion : public ConsumableItem {
public:
    MagicPotion();
    virtual void use(NovicePlayer* player) override;
};
#endif
