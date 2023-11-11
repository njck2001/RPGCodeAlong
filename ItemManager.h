#pragma once
#include "Item.h"
#include "Buff.h"
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

    static Item* make_potion(std::string name, well_type hp_heal = 0u, well_type mp_heal = 0u,
    Buff* buff = nullptr, item_count quantity = 1u) {
        if (quantity == 0) {
            quantity = 1;

            //  \033[1;35m      1 = bold, 35 = magenta
            //  \033[0m         resets everything back to default
            //  \033[1;31m      if you want bold red for errors
            std::cerr << "\033[1;35m" << "Warning:" << "\033[0m" << " 0 is not a valid argument for parameter 'quantity' in function 'ItemManager::make_potion'\n"
                      << "  'quantity' has been changed to 1 instead" << std::endl;
        }
        Item* potion = new Item(new Potion(name, hp_heal, mp_heal, buff, quantity));
        return potion;
    }

    // Equips an armor or weapon and returns true, returns false if the action failed
    // If an item was previously equipped in that slot, the item will be moved to the player character's backpack
    static bool equip(Item* item, PlayerCharacter* pc) {
        if (!pc || !item || !item->data_) {
            return false;
        }

        Armor* armor = dynamic_cast<Armor*>(item->data_);
        if (armor) {
            // Equip armor
            int slot_num = (int)armor->slot();
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
            int slot_num = (int)weapon->slot();
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

    /// TODO: Refactor this function to make the logic clearer
    static bool use(Item* item, PlayerCharacter* pc) {
        if (!pc || !item || !item->data_) {
            return false;
        }
        
        Potion* potion = dynamic_cast<Potion*>(item->data_);
        if (potion) {
            // Apply buff if it has any
            if (potion->buff) {
                pc->add_buff(*potion->buff);
            }

            // If max health and trying to use a heal potion, don't
            if (pc->pc_class->hp()->is_full() && !potion->buff) {
                return false;
            }

            // Increase hp by heal amount (could be 0, and that's fine!)
            pc->heal(potion->hp_heal);

            // We used the potion, so reduce quantity
            potion->quantity--;
            if (potion->quantity == 0) {
                item->marked_for_deletion_ = true;
                pc->cleanup_backpack();
                /// TODO: Why not just do "delete item;" instead? We'll experiment with this
            }

            return true;
        }

        return false;
    }

    static bool move_to_backpack(Item* item, PlayerCharacter* pc) {
        if (!pc || !item || !item->data_) {
            return false;
        }
        
        pc->backpack_.push_back(item);
        return true;
    }
};
