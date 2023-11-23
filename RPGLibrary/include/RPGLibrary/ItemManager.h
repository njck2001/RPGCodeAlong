#pragma once
#include "Item.h"
#include "Buff.h"
#include "PlayerCharacter.h"
#include <iostream>


class ItemManager {
public:
    // Armor
    static Item* make_armor(std::string name, Stats stats, ARMORSLOT slot);
    static void cast_item_to_armor(const Item* item, Armor*& armor); // armor will be a nullptr if casting fails
    static bool is_item_armor(const Item* item);

    // Weapons
    static Item* make_weapon(std::string name, Stats stats, WEAPONSLOT slot, uint16_t min_damage,
        uint16_t max_damage, bool is_two_handed = false);
    static void cast_item_to_weapon(const Item* item, Weapon*& weapon); // weapon will be a nullptr if casting fails
    static bool is_item_weapon(const Item* item);
    
    // Potions
    static Item* make_potion(std::string name, Buff* buff, uint16_t quantity = 1,
        welltype hp_heal = 0, welltype mp_heal = 0, bool overheal = false);
    static void cast_item_to_potion(const Item* item, Potion*& potion); // potion will be a nullptr if casting fails
    static bool is_item_potion(const Item* item);

    // Player Character Functions
    static bool equip(Item* item, PlayerCharacter* pc); // Returns true if item is successfully equipped
    static bool use(Item* item, PlayerCharacter* pc); // Returns true if potion is used
    static bool move_to_backpack(Item* item, PlayerCharacter* pc); // Returns true if item is successfully moved to backpack
};
