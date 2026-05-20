#ifndef MAGICIANPLAYER_H
#define MAGICIANPLAYER_H
#include "NovicePlayer.h"

class MagicianPlayer : public NovicePlayer {
protected:
    virtual void setAttr() override;
public:
    MagicianPlayer();
    MagicianPlayer(int l);
    MagicianPlayer(int l, string n);
    MagicianPlayer(const MagicianPlayer& other);
    virtual void setLevel(int l) override;
    virtual void specialSkill() override; //
    virtual string serialize() override;
    static NovicePlayer* unserialize(string data);
};
#endif