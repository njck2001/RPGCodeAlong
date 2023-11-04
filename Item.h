#pragma once
#include "Buff.h"
#include <string>


class ItemDelegate {
    std::string name_;
public:
    // Getters
    const std::string& name() const { return name_; } 
    /*  First const ensures the name() returns a const reference - a reference to name_ that
        cannot be modified. The second const allows const objects to call this functions.
        Otherwise, trying to call name() using a const object would cause an error.
    */
    virtual const char* type() = 0;
protected:
    ItemDelegate(std::string name) : name_(name) {}
};


class Potion final : public ItemDelegate {
public:
    Buff* buff;
    well_type hp_heal;
    well_type mp_heal;
    item_count quantity;

    ~Potion() {
        if (buff) {
            delete buff;
            buff = nullptr;
        }
    }

    // Getters
    const char* type() { return "Potion"; }

private:
    Potion(std::string name, well_type hp_heal, well_type mp_heal, Buff* buff, item_count quantity) :
        ItemDelegate(name), hp_heal(hp_heal), mp_heal(mp_heal), buff(buff), quantity(quantity)
    {}

    friend class ItemManager;
};


class EquipmentDelegate : public ItemDelegate {
    const uint32_t ID;
    Stats stats_;
public:
    // Getters
    /// TODO: Implement getters for each stat rather than a getter for the whole Stat struct
    const Stats& stats() const { return stats_; }
protected:
    EquipmentDelegate(std::string name, Stats stats);
};


enum class ARMORSLOT { HELMET, CHEST, LEGS, BOOTS, GLOVES, RING1, RING2, NECKLACE, NUM_SLOTS };
    // NUM_SLOTS is strictly only used for making arrays of ARMORSLOTs

class Armor final : public EquipmentDelegate {
    ARMORSLOT slot_;

public:
    // Getters
    const char* type() { return "Armor"; }
    ARMORSLOT slot() { return slot_; }

private:
    Armor(std::string name, Stats stats, ARMORSLOT slot) : EquipmentDelegate(name, stats), slot_(slot) {}

    friend class ItemManager;

    // Deleted Constructors
    Armor() = delete; // Removes the default constructor so that it cannot be used
    Armor(const Armor&) = delete; // Removes copy constructor
    Armor(const Armor&&) = delete; // Removes move constructor
};


enum class WEAPONSLOT { MELEE, RANGED, NUM_SLOTS };
    // NUM_SLOTS is strictly only used for making arrays of WEAPONSLOTs

class Weapon final : public EquipmentDelegate {
    WEAPONSLOT slot_;
    uint16_t min_damage_;
    uint16_t max_damage_;
    
    bool is_two_handed;

public:
    // Getters
    const char* type() { return "Weapon"; }
    WEAPONSLOT slot() { return slot_; }
    uint16_t min_damage() { return min_damage_; }
    uint16_t max_damage() { return max_damage_; }

private:
    Weapon(std::string name, Stats stats, WEAPONSLOT slot, uint16_t min_damage,
    uint16_t max_damage, bool is_two_handed = false) :
        EquipmentDelegate(name, stats), slot_(slot), min_damage_(min_damage),
        max_damage_(max_damage), is_two_handed(is_two_handed)
    {}

    friend class ItemManager;

    // Deleted Constructors
    Weapon() = delete; // Removes the default constructor so that it cannot be used
    Weapon(const Weapon&) = delete; // Removes copy constructor
    Weapon(const Weapon&&) = delete; // Removes move constructor
};


// Use this one in your runtime code
class Item {
    ItemDelegate* data_;

public:
    const ItemDelegate* data() const { return data_; }
    ~Item() {
        if (data_) {
            delete data_;
            data_ = nullptr;
        }
    }

private:
    Item(ItemDelegate* item_data) : data_(item_data) {}
    friend class ItemManager;
    friend class PlayerCharacter;
};
