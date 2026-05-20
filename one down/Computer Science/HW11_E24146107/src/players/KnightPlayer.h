#ifndef KNIGHTPLAYER_H
#define KNIGHTPLAYER_H
#include "NovicePlayer.h"

class KnightPlayer : public NovicePlayer {
protected:
    virtual void setAttr() override;
public:
    KnightPlayer();
    KnightPlayer(int l);
    KnightPlayer(int l, string n);
    KnightPlayer(const KnightPlayer& other);
    virtual void setLevel(int l) override; //
    virtual void specialSkill() override; //
    virtual string serialize() override;
    static NovicePlayer* unserialize(string data);
};
#endif