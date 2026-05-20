#ifndef ARMORITEM_H
#define ARMORITEM_H

#include "Item.h"

class ArmorItem : public Item {
public:
    const int defense_increment;

    ArmorItem(int level, string n, string eff, string desc, int w, int def_inc)
        : Item(level, n, eff, desc, w, 'a'), defense_increment(def_inc) {}
};

#endif
