#ifndef BASEMONSTER_H
#define BASEMONSTER_H
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

class Item;

class BaseMonster {
private:
    int hp, mp;
    static int count; //
public:
    const string name; //
    const int attack, defense, exp, money, max_hp, max_mp;

    BaseMonster(string n, int atk, int def, int e, int m, int mhp, int mmp);
    virtual ~BaseMonster();

    void setHP(int h);
    int getHP() const;
    void setMP(int m);
    int getMP() const;

    static int getInstanceCount(); //
    virtual string serialize() = 0; //
    virtual Item* dropItem();
};
#endif