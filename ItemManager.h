#pragma once
#include "Item.h"
#include "Buff.h"
//#include <iostream>


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

    static Item* make_potion(std::string name, well_type hp_heal = 0u, well_type mp_heal = 0u,
    Buff* buff = nullptr, item_count quantity = 1u) {
        // if (quantity == 0) {
        //     quantity = 1;

        //     // Ideally, I'd like to somehow output the messages below without including iostream
        //     std::cerr << "Quantity of potion " << name << "must be greater than 0" << std::endl;
        //     std::cerr << "Quantity has been changed to 1" << std::endl;
        // }
        Item* potion = new Item(new Potion(name, hp_heal, mp_heal, buff, (quantity == 0) ? 1 : quantity));
        return potion;
    }
};
