#ifndef ORCPLAYER_H
#define ORCPLAYER_H
#include "NovicePlayer.h"

class OrcPlayer : public NovicePlayer {
protected:
    virtual void setAttr() override;
public:
    OrcPlayer();
    OrcPlayer(int l);
    OrcPlayer(int l, string n);
    OrcPlayer(const OrcPlayer& other);
    virtual void setLevel(int l) override;
    virtual void specialSkill() override {} //
    virtual string serialize() override;
    static NovicePlayer* unserialize(string data);
};
#endif