#ifndef ITEM_H
#define ITEM_H

#include <string>
using namespace std;

class Item {
public:
    const int level_required;
    const string name;
    const string effects;
    const string description;
    const int weight;
    const char type; // 'w' for weapons, 'a' for armors, 'c' for consumables

    Item(int level, string n, string eff, string desc, int w, char t)
        : level_required(level), name(n), effects(eff), description(desc), weight(w), type(t) {}
    virtual ~Item() {}
};

#endif
