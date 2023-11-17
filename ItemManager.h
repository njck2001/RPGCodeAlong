#pragma once
#include "Item.h"
#include "Buff.h"
#include "PlayerCharacter.h"
#include <iostream>


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

    static Item* make_potion(std::string name, Buff* buff, uint16_t quantity = 1,
    welltype hp_heal = 0, welltype mp_heal = 0, bool overheal = false) {
        if (quantity == 0) {
            quantity = 1;
            //  \033[1;35m      1 = bold, 35 = magenta
            //  \033[0m         resets everything back to default
            //  \033[1;31m      if you want bold red for errors
            std::cerr << "\033[1;35m" << "Warning:" << "\033[0m" << " 0 is not a valid argument for parameter 'quantity' in function 'ItemManager::make_potion'\n"
                      << "  'quantity' has been changed to 1 instead" << std::endl;
        }
        Item* potion = new Item(new Potion(name, buff, quantity, hp_heal, mp_heal, overheal));
        return potion;
    }

    // Returns true if item is successfully equipped
    static bool equip(Item* item, PlayerCharacter* pc) {
        if (!pc || !item || !item->data_) {
            /*  Notice that we are able to use item->data_ instead of item->data().
                ItemManager is a friend class of Item, so it has access to all of Item's memebers.
                Rather than calling the getter, we can just access the member variable directly.
                This is a very negligible optimization, and it is generally best practice to always
                use getters when possible to avoid making any unwanted changes to member variables.
            */
            return false;
        }

        Armor* armor = dynamic_cast<Armor*>(item->data_);
        if (armor) {
            // Equip armor
            int slot_num = (int)armor->slot_;
            if (pc->equipped_armor_[slot_num]) {
                move_to_backpack(pc->equipped_armor_[slot_num], pc);
                pc->equipped_armor_[slot_num] = item;
            }
            else {
                pc->equipped_armor_[slot_num] = item;
            }
            return true;
        }
        
        Weapon* weapon = dynamic_cast<Weapon*>(item->data_);
        if (weapon) {
            // Equip weapon
            int slot_num = (int)weapon->slot_;
            if (pc->equipped_weapons_[slot_num]) {
                move_to_backpack(pc->equipped_weapons_[slot_num], pc);
                pc->equipped_weapons_[slot_num] = item;
            }
            else {
                pc->equipped_weapons_[slot_num] = item;
            }
            return true;
        }

        return false;
    }

    // Returns true if potion is used
    static bool use(Item* item, PlayerCharacter* pc) {
        if (!pc || !item || !item->data_) {
            return false;
        }
        
        bool used_potion = false;
        Potion* potion = dynamic_cast<Potion*>(item->data_);
        if (potion) {

            // If potion has buff, use potion
            if (potion->buff_) {
                pc->add_buff(*potion->buff_);
                pc->heal_hp(potion->hp_heal_, potion->overheal);
                pc->heal_mp(potion->mp_heal_, potion->overheal);
                used_potion = true;
            }

            // If potion increases hp and character's hp is not max, use potion
            else if (potion->overheal || ((potion->hp_heal_ > 0) && !pc->pc_class->hp()->is_full())) {
                pc->heal_hp(potion->hp_heal_, potion->overheal);
                pc->heal_mp(potion->mp_heal_, potion->overheal);
                used_potion = true;
            }

            // If potion increases mp and character's mp is not max, use potion
            else if (potion->overheal || ((potion->mp_heal_ > 0) && pc->pc_class->mp() && !pc->pc_class->mp()->is_full())) {
                pc->heal_mp(potion->mp_heal_, potion->overheal);
                used_potion = true;
            }

            // If potion was used, decrease quantity
            if (used_potion) {
                potion->quantity_--;
                if (potion->quantity_ == 0) {
                    item->marked_for_deletion = true;
                    pc->cleanup_backpack();
                    /// TODO: Why not just do "delete item;" instead? We'll experiment with this
                };
            }
        }
        return used_potion;
    }

    // Returns true if item is successfully moved to backpack
    static bool move_to_backpack(Item* item, PlayerCharacter* pc) {
        if (!pc || !item || !item->data_) {
            return false;
        }
        else {
            pc->backpack_.push_back(item);
            return true;
        }
    }
};


// Definitions for setters used in PlayerCharacter class
bool PlayerCharacter::equip(Item* item) { return ItemManager::equip(item, this); }
bool PlayerCharacter::use(Item* item) { return ItemManager::use(item, this); }
bool PlayerCharacter::pick_up(Item* item) { return ItemManager::move_to_backpack(item, this); }
