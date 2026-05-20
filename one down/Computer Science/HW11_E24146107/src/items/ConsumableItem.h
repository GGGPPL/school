#ifndef CONSUMABLEITEM_H
#define CONSUMABLEITEM_H

#include "Item.h"

class NovicePlayer; // Forward declaration

class ConsumableItem : public Item {
public:
    ConsumableItem(int level, string n, string eff, string desc, int w)
        : Item(level, n, eff, desc, w, 'c') {}

    virtual void use(NovicePlayer* player) = 0;
};

#endif
