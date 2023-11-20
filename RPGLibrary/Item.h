#pragma once
#include "Buff.h"
#include <string>
#include <iostream>



class ItemDelegate {
    std::string name_;
public:
    const std::string& name() const { return name_; } 
    /*  The first const ensures that name() returns a constant reference - a reference to name_
        that cannot be modified. The second const allows const objects to call this functions.
        Otherwise, trying to call name() using a const object would cause an error.
    */
    virtual const char* type() const = 0; // See PlayerCharacter.h to learn about virtual functions
protected:
    ItemDelegate(std::string name) : name_(name) {}
};



class Potion final : public ItemDelegate {
    Buff* buff_;
    welltype hp_heal_;
    welltype mp_heal_;
    uint16_t quantity_;
    bool overheal;

public:
    // Deconstructor
    ~Potion() {
        if (buff_) {
            delete buff_;
            buff_ = nullptr;
        }
    }

    const char* type() const { return "Potion"; }
    const Buff* buff() const { return buff_; }
    welltype hp_heal() const { return hp_heal_; }
    welltype mp_heal() const { return mp_heal_; }
    uint16_t quantity() const { return quantity_; }

    // Deleted Constructors
    Potion() = delete;
    Potion(const Potion&) = delete; // Removes copy constructor
    Potion(const Potion&&) = delete; // Removes move constructor

private:
    Potion(std::string name, Buff* buff, uint16_t quantity = 1u, welltype hp_heal = 0u,
    welltype mp_heal = 0u, bool overheal = false) :
        ItemDelegate(name), buff_(buff), quantity_(quantity), hp_heal_(hp_heal),
        mp_heal_(mp_heal), overheal(overheal)
    {} // Private so that only the ItemManager class can use this constructor

    friend class ItemManager; // Friend classes are allowed to access all of a class's members and methods
};



class EquipmentDelegate : public ItemDelegate {
    const uint32_t id = 0u;
    Stats stats;
public:
    stattype strength() const { return stats.strength; }
    stattype intellect() const { return stats.intellect; }
    stattype agility() const { return stats.agility; }
    stattype armor() const { return stats.armor; }
    stattype resistance() const { return stats.resistance; }
protected:
    EquipmentDelegate(std::string name, Stats stats); // Defined in Item.cpp
};



enum class ARMORSLOT { HELMET, CHEST, LEGS, BOOTS, GLOVES, RING1, RING2, NECKLACE, NUM_SLOTS };
    // NUM_SLOTS is strictly only used for making arrays of ARMORSLOTs

class Armor final : public EquipmentDelegate {
    ARMORSLOT slot_;

public:
    const char* type() const { return "Armor"; }
    ARMORSLOT slot() { return slot_; }

    // Deleted Constructors
    Armor() = delete;
    Armor(const Armor&) = delete;
    Armor(const Armor&&) = delete;

private:
    Armor(std::string name, Stats stats, ARMORSLOT slot) : EquipmentDelegate(name, stats), slot_(slot) {}

    friend class ItemManager;
};



enum class WEAPONSLOT { MELEE, RANGED, NUM_SLOTS };
    // NUM_SLOTS is strictly only used for making arrays of WEAPONSLOTs
    
class Weapon final : public EquipmentDelegate {
    WEAPONSLOT slot_;
    stattype min_damage_;
    stattype max_damage_;
    bool is_two_handed;

public:
    const char* type() const { return "Weapon"; }
    WEAPONSLOT slot() { return slot_; }
    stattype min_damage() { return min_damage_; }
    stattype max_damage() { return max_damage_; }

    // Deleted Constructors
    Weapon() = delete;
    Weapon(const Weapon&) = delete;
    Weapon(const Weapon&&) = delete;

private:
    Weapon(std::string name, Stats stats, WEAPONSLOT slot, stattype min_damage,
    stattype max_damage, bool is_two_handed = false) :
        EquipmentDelegate(name, stats), slot_(slot), min_damage_(min_damage),
        max_damage_(max_damage), is_two_handed(is_two_handed)
    {}

    friend class ItemManager;
};



// Use this class in your runtime code
class Item {
    ItemDelegate* data_;
    bool marked_for_deletion; // No getter for, only used by friend classes

public:
    ~Item() {
        if (data_) {
            delete data_;
            data_ = nullptr;
        }
    }

    const ItemDelegate* data() const { return data_; }

private:
    Item(ItemDelegate* item_data) : data_(item_data), marked_for_deletion(false) {}

    friend class ItemManager;
    friend class PlayerCharacter;

    friend std::ostream& operator<<(std::ostream& os, const Item& item) {
        Armor* armor = dynamic_cast<Armor*>(item.data_);
        if (armor) {
            return os << armor->name() << " (Armor: " << armor->armor() << ", Resist: " << armor->resistance() << ')';
        }
        Weapon* weapon = dynamic_cast<Weapon*>(item.data_);
        if (weapon) {
            return  os << weapon->name() << " (Damage: " << weapon->min_damage() << '-' << weapon->max_damage() << ')';
        }
        Potion* potion = dynamic_cast<Potion*>(item.data_);
        if (potion) {
            return os << potion->name() << " (Quantity: " << potion->quantity() << ')';
        }
        return os;
  }
};
