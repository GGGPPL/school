#include "MagicianPlayer.h"
#include <cmath>

MagicianPlayer::MagicianPlayer() : NovicePlayer() {
    setLevel(1);
    setHp(getMaxHP());
    setMp(getMaxMP());
}

MagicianPlayer::MagicianPlayer(int l) : NovicePlayer(l) {
    setLevel(l);
    setHp(getMaxHP());
    setMp(getMaxMP());
}

MagicianPlayer::MagicianPlayer(int l, string n) : NovicePlayer(l, n) {
    setLevel(l);
    setHp(getMaxHP());
    setMp(getMaxMP());
}

MagicianPlayer::MagicianPlayer(const MagicianPlayer& other) : NovicePlayer(other) {}

void MagicianPlayer::setLevel(int l) {
    level = l;
    // MagicianPlayer 專屬公式 [cite: 290, 388]
    maxhp = 120 + 15 * level;
    maxmp = 100 + 15 * level;
    attack = 30 + 8 * level;
    defense = 20 + 7 * level;
    lvupexp = ceil(pow(10, log2(level + 1)));

    if (getHp() > maxhp) setHp(maxhp);
    if (getMp() > maxmp) setMp(maxmp);
    if (getExp() > lvupexp) setExp(lvupexp);
}

// pray()：消耗 HP 回復 MP [cite: 384]
void MagicianPlayer::pray() {
    if (getHp() >= level * 5) {
        setHp(getHp() - level * 5);
        setMp(getMp() + level * 10);
    }
}