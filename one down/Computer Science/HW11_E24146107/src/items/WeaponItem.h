#ifndef WEAPONITEM_H
#define WEAPONITEM_H

#include "Item.h"

class WeaponItem : public Item {
public:
    const int attack_increment;

    WeaponItem(int level, string n, string eff, string desc, int w, int atk_inc)
        : Item(level, n, eff, desc, w, 'w'), attack_increment(atk_inc) {}
};

#endif
