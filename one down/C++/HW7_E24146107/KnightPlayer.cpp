#include "KnightPlayer.h"
#include <cmath>

KnightPlayer::KnightPlayer() : NovicePlayer() {
    setLevel(1);
    setHp(getMaxHP());
    setMp(getMaxMP());
}

KnightPlayer::KnightPlayer(int l) : NovicePlayer(l) {
    setLevel(l);
    setHp(getMaxHP());
    setMp(getMaxMP());
}

KnightPlayer::KnightPlayer(int l, string n) : NovicePlayer(l, n) {
    setLevel(l);
    setHp(getMaxHP());
    setMp(getMaxMP());
}

KnightPlayer::KnightPlayer(const KnightPlayer& other) : NovicePlayer(other) {}

void KnightPlayer::setLevel(int l) {
    level = l;
    // KnightPlayer 專屬公式 [cite: 288, 388]
    maxhp = 150 + 25 * level;
    maxmp = 70 + 10 * level;
    attack = 40 + 10 * level;
    defense = 20 + 12 * level;
    lvupexp = ceil(pow(10, log2(level + 1)));

    if (getHp() > maxhp) setHp(maxhp);
    if (getMp() > maxmp) setMp(maxmp);
    if (getExp() > lvupexp) setExp(lvupexp);
}

// heal()：消耗 MP 回復 HP [cite: 371]
void KnightPlayer::heal() {
    if (getMp() >= level * 5) {
        setMp(getMp() - level * 5);
        setHp(getHp() + level * 10);
    }
}