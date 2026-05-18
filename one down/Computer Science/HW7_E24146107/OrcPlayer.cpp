#include "OrcPlayer.h"
#include <cmath>

OrcPlayer::OrcPlayer() : NovicePlayer() {
    setLevel(1);
    setHp(getMaxHP());
    setMp(getMaxMP());
}

OrcPlayer::OrcPlayer(int l) : NovicePlayer(l) {
    setLevel(l);
    setHp(getMaxHP());
    setMp(getMaxMP());
}

OrcPlayer::OrcPlayer(int l, string n) : NovicePlayer(l, n) {
    setLevel(l);
    setHp(getMaxHP());
    setMp(getMaxMP());
}

OrcPlayer::OrcPlayer(const OrcPlayer& other) : NovicePlayer(other) {}

void OrcPlayer::setLevel(int l) {
    level = l;
    // OrcPlayer 專屬公式 [cite: 288, 388]
    maxhp = 200 + 20 * level;
    maxmp = 50 + 5 * level;
    attack = 50 + 12 * level;
    defense = 30 + 10 * level;
    lvupexp = ceil(pow(10, log2(level + 1)));

    if (getHp() > maxhp) setHp(maxhp);
    if (getMp() > maxmp) setMp(maxmp);
    if (getExp() > lvupexp) setExp(lvupexp);
}