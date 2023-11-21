#pragma once
#include "Item.h"
#include "Buff.h"
#include "PlayerCharacter.h"
#include <iostream>


class ItemManager {
public:
    static Item* make_armor(std::string name, Stats stats, ARMORSLOT slot);
    static Item* make_weapon(std::string name, Stats stats, WEAPONSLOT slot, uint16_t min_damage,
        uint16_t max_damage, bool is_two_handed = false);
    static Item* make_potion(std::string name, Buff* buff, uint16_t quantity = 1,
        welltype hp_heal = 0, welltype mp_heal = 0, bool overheal = false);
    
    static bool equip(Item* item, PlayerCharacter* pc); // Returns true if item is successfully equipped
    static bool use(Item* item, PlayerCharacter* pc); // Returns true if potion is used
    static bool move_to_backpack(Item* item, PlayerCharacter* pc); // Returns true if item is successfully moved to backpack
};
