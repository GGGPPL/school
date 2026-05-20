#include "LifePotion.h"
#include "../players/NovicePlayer.h"
LifePotion::LifePotion() : ConsumableItem(1, "Life Potion", "HP+50", "Restores 50 HP.", 1) {}
void LifePotion::use(NovicePlayer* player) {
    player->setHp(player->getHp() + 50);
}
