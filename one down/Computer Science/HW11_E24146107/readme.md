# HW11 RPG Item System Implementation

This update introduces a comprehensive item and inventory system to the RPG game, including weapons, armor, consumables, and a backpack management system.

## 1. Weapon Item Class
The `WeaponItem` class extends the base `Item` class to provide offensive bonuses to the player.
*   **Sword**: A basic starting weapon.
    *   **ATK Increment**: <font color="#00ff00">+10</font>
    *   **Weight**: 5
    *   **Level Required**: 1
*   **Axe**: A heavy and powerful weapon for experienced warriors.
    *   **ATK Increment**: <font color="#ff0000">+20</font>
    *   **Weight**: 15
    *   **Level Required**: 5

## 2. Armor Item Class
The `ArmorItem` class provides defensive bonuses to reduce damage taken from monsters.
*   **Tunic**: Basic protection.
    *   **DEF Increment**: <font color="#00ff00">+5</font>
    *   **Weight**: 3
    *   **Level Required**: 1
*   **Shield**: Sturdy protection against heavy hits.
    *   **DEF Increment**: <font color="#00ff00">+15</font>
    *   **Weight**: 10
    *   **Level Required**: 3

## 3. Consumable Item Class
Consumables can be used during exploration or battle to restore vital stats.
*   **Life Potion**: Restores <font color="#ff0000">50 HP</font> upon use. Useful for surviving tough encounters.
*   **Magic Potion**: Restores <font color="#0000ff">20 MP</font> upon use. Essential for casting more special skills.
*   **Weight**: Both potions have a weight of 1.

## 4. Additional Feature: Backpack System
A robust inventory system has been implemented to manage items.
*   **Weight & Slot Limits**: Each player class has different capacities:
    *   **Knight**: 15 slots, 100 weight capacity.
    *   **Magician**: 20 slots, 30 weight capacity.
    *   **Orc**: 10 slots, 150 weight capacity.
    *   **Novice**: 10 slots, 50 weight capacity.
*   **Equipment Weight**: When an item is equipped, it is removed from the backpack, thus its weight no longer counts towards the `backpack_weight` limit. If you unequip or replace an item, the old item will attempt to return to the backpack if there is enough space and weight capacity.

## 5. How to use
### Exploration Mode
*   Press **`[I]`** to open the Inventory menu.
*   From the menu, you can choose to:
    1.  **Equip Weapon**: Select an item from the backpack to use as your primary weapon.
    2.  **Equip Armor**: Select an item from the backpack to wear as armor.
    3.  **Use Consumable**: Instantly use a potion to restore HP or MP.
    4.  **Back**: Return to the map.

### Battle Mode
*   A new action choice **`3. Use Item`** has been added to the battle menu.
*   Selecting this will display your backpack.
*   Enter the index of a consumable item (e.g., Life Potion) to use it during your turn. Note: Only consumable items can be used during battle.

## 6. Directory Structure
The project is organized as follows:
```
HW11_E24146107/
├── main.cpp                (Entry point, handles exploration and inventory input)
├── common/
│   └── map1.txt            (Game map data)
├── doc/
│   └── README.md           (Project documentation)
└── src/
    ├── items/              (Item system implementation)
    │   ├── Item.h          (Base class for all items)
    │   ├── WeaponItem.h    (Derived class for weapons)
    │   ├── ArmorItem.h     (Derived class for armors)
    │   ├── ConsumableItem.h(Base class for useable items)
    │   ├── SwordWeapon.h/cpp, AxeWeapon.h/cpp
    │   ├── TunicArmor.h/cpp, ShieldArmor.h/cpp
    │   └── LifePotion.h/cpp, MagicPotion.h/cpp
    ├── players/            (Player classes: Knight, Magician, etc.)
    ├── monsters/           (Monster classes: Goblin, Zombie, etc.)
    ├── battle/             (Battle system logic)
    └── field/              (Field exploration and map logic)
```
