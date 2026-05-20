#include "MagicPotion.h"
#include "../players/NovicePlayer.h"
MagicPotion::MagicPotion() : ConsumableItem(1, "Magic Potion", "MP+20", "Restores 20 MP.", 1) {}
void MagicPotion::use(NovicePlayer* player) {
    player->setMp(player->getMp() + 20);
}
