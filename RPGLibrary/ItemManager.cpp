#include "ItemManager.h"


// Armor
Item* ItemManager::make_armor(std::string name, Stats stats, ARMORSLOT slot) {
    /*  While it would be simpler and more convenient to return a unique_ptr, we decided
        to return a raw pointer here to better learn how to use and manage them
    */
    Item* armor = new Item(new Armor(name, stats, slot));
    return armor;
}
void ItemManager::cast_item_to_armor(const Item* item, Armor*& armor) {
    armor = dynamic_cast<Armor*>(item->data);
}
bool ItemManager::is_item_armor(const Item* item) {
    if (dynamic_cast<Armor*>(item->data)) {
        return true;
    }
    else {
        return false;
    }
}

// Weapons
Item* ItemManager::make_weapon(std::string name, Stats stats, WEAPONSLOT slot, uint16_t min_damage,
uint16_t max_damage, bool is_two_handed) {
    Item* weapon = new Item(new Weapon(name, stats, slot, min_damage, max_damage, is_two_handed));
    return weapon;
}
void ItemManager::cast_item_to_weapon(const Item* item, Weapon*& weapon) {
    weapon = dynamic_cast<Weapon*>(item->data);
}
bool ItemManager::is_item_weapon(const Item* item) {
    if (dynamic_cast<Weapon*>(item->data)) {
        return true;
    }
    else {
        return false;
    }
}

// Potions
Item* ItemManager::make_potion(std::string name, Buff* buff, uint16_t quantity,
welltype hp_heal, welltype mp_heal, bool overheal) {
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
void ItemManager::cast_item_to_potion(const Item* item, Potion*& potion) {
    potion = dynamic_cast<Potion*>(item->data);
}
bool ItemManager::is_item_potion(const Item* item) {
    if (dynamic_cast<Potion*>(item->data)) {
        return true;
    }
    else {
        return false;
    }
}

// Player Character Functions
bool ItemManager::equip(Item* item, PlayerCharacter* pc) {
    if (!pc || !item || !item->data) {
        return false;
    }

    Armor* armor = dynamic_cast<Armor*>(item->data);
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
    
    Weapon* weapon = dynamic_cast<Weapon*>(item->data);
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
bool ItemManager::use(Item* item, PlayerCharacter* pc) {
    if (!pc || !item || !item->data) {
        return false;
    }
    
    bool used_potion = false;
    Potion* potion = dynamic_cast<Potion*>(item->data);
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
bool ItemManager::move_to_backpack(Item* item, PlayerCharacter* pc) {
    if (!pc || !item || !item->data) {
        return false;
    }
    else {
        pc->backpack_.push_back(item);
        return true;
    }
}
