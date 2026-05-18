#ifndef NOVICEPLAYER_H
#define NOVICEPLAYER_H
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
using namespace std;

class NovicePlayer {
private:
    string name;
    int hp, mp, exp, money;
protected:
    int level, attack, defense, maxhp, maxmp, lvupexp;
public:
    NovicePlayer();
    NovicePlayer(int l);
    NovicePlayer(int l, string n);
    NovicePlayer(const NovicePlayer& other);
    virtual ~NovicePlayer() {}

    void setName(string n);
    string getName() const;
    virtual void setLevel(int l); //
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

    virtual void specialSkill() {} //
    virtual string serialize(); //
    static NovicePlayer* unserialize(string data); //
    void print();
};
#endif