#pragma once
#include "item.h"


class ItemManager {
public:
    static Item* make_armor(std::string name, Stats stats, ARMORSLOT slot) {
        /*  While it would be simpler and more convenient to return a unique_ptr, we decided
            to return a raw pointer here to better learn how to use and manage them
        */
       Item* armor = new Item(new Armor(name, stats, slot));
       return armor;
    }

    static Item* make_weapon(std::string name, Stats stats, WEAPONSLOT slot, uint16_t min_damage,
    uint16_t max_damage, bool is_two_handed = false) {
        Item* weapon = new Item(new Weapon(name, stats, slot, min_damage, max_damage, is_two_handed));
       return weapon;
    }
};
