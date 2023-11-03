#pragma once
#include "Stats.h"
#include <string>
//#include <typeinfo>


class Equipment {
    std::string name_;
    Stats stats_;

public:
    // Getters
    const std::string& name() { return name_; }
    const Stats& stats() { return stats_; }
    virtual const char* type() = 0;

//protected:
    Equipment(std::string name, Stats stats) : name_(name), stats_(stats) {}
};


enum class ARMORSLOT { HELMET, CHEST, LEGS, BOOTS, GLOVES, RING1, RING2, NECKLACE, NUM_SLOTS };
// NUM_SLOTS is strictly only used for making arrays of ARMORSLOTs

class Armor final : public Equipment {
    ARMORSLOT slot_;

public:
    Armor(std::string name, Stats stats, ARMORSLOT slot) : Equipment(name, stats), slot_(slot) {}

    // Getters
    //const char* type() { return typeid(*this).name(); } // returns "class Armor"
    const char* type() override { return "Armor"; }
    ARMORSLOT slot() { return slot_; }

    // Deleted Constructors
    Armor() = delete; // Removes the default constructor so that it cannot be used
    Armor(const Armor&) = delete; // Removes copy constructor
    Armor(const Armor&&) = delete; // Removes move constructor
};


enum class WEAPONSLOT { MELEE, RANGED, NUM_SLOTS };
// NUM_SLOTS is strictly only used for making arrays of WEAPONSLOTs

class Weapon final : public Equipment {
    WEAPONSLOT slot_;
    uint16_t min_damage;
    uint16_t max_damage;
    bool is_two_handed;

public:
    Weapon(std::string name, Stats stats, WEAPONSLOT slot, uint16_t min_damage,
    uint16_t max_damage, bool is_two_handed = false) :
        Equipment(name, stats), slot_(slot), min_damage(min_damage), max_damage(max_damage),
        is_two_handed(is_two_handed)
    {}

    // Getters
    //const char* type() { return typeid(*this).name(); } // returns "class Weapon"
    const char* type() override { return "Weapon"; }
    WEAPONSLOT slot() { return slot_; }

    // Deleted Constructors
    Weapon() = delete; // Removes the default constructor so that it cannot be used
    Weapon(const Weapon&) = delete; // Removes copy constructor
    Weapon(const Weapon&&) = delete; // Removes move constructor
};
