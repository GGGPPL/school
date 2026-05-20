#ifndef NOVICEPLAYER_H
#define NOVICEPLAYER_H
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

class Item;
class WeaponItem;
class ArmorItem;
class ConsumableItem;

class NovicePlayer {
private:
    string name;
    int hp, mp, exp;
protected:
    int level, attack, defense, maxhp, maxmp, lvupexp;
    int money;
    
    // HW11 Additions
    Item** backpack;
    WeaponItem* weapon;
    ArmorItem* armor;
    int backpack_weight;
    int backpack_weight_limit;
    int backpack_slot_limit;

    virtual void setAttr(); // New method for setting attributes and limits

public:
    NovicePlayer();
    NovicePlayer(int l);
    NovicePlayer(int l, string n);
    NovicePlayer(const NovicePlayer& other);
    virtual ~NovicePlayer(); // Updated to cleanup backpack

    void setName(string n);
    string getName() const;
    virtual void setLevel(int l); 
    int getLevel() const;
    void setHp(int h);
    int getHp() const;
    void setMp(int m);
    int getMp() const;
    void setExp(int e);
    int getExp() const;
    void setMoney(int m);
    int getMoney() const;
    int getAttack() const;
    int getDefense() const;
    int getMaxHP() const;
    int getMaxMP() const;
    int getLvupExp() const;

    // HW11 Instance Methods
    bool equipWeapon(WeaponItem* w);
    bool equipArmor(ArmorItem* a);
    void useConsumable(ConsumableItem* c);
    bool putItem(Item* i);
    Item* takeItem(int index);
    
    void printBackpack() const;

    virtual void specialSkill() {} 
    virtual string serialize(); 
    static NovicePlayer* unserialize(string data); 
    void print();
};
#endif