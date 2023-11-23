#pragma once
#include "Buff.h"
#include <string>
#include <iostream>



class ItemDelegate {
    std::string name_;
public:
    const std::string& name() const;
    /*  The first const ensures that name() returns a constant reference - a reference to name_
        that cannot be modified. The second const allows const objects to call this functions.
        Otherwise, trying to call name() using a const object would cause an error.
    */
protected:
    ItemDelegate(std::string name);
    virtual ~ItemDelegate() = 0 {}//; // Derived classes must define deconstructor
        // See PlayerCharacter.h to learn more about virtual functions
    friend class Item; // Friend classes are allowed to access all of a class's members and methods
};



class Potion final : public ItemDelegate {
    Buff* buff_;
    welltype hp_heal_;
    welltype mp_heal_;
    uint16_t quantity_;
    bool overheal;

public:
    // Deconstructor
    ~Potion();
    
    const Buff* buff() const;
    welltype hp_heal() const;
    welltype mp_heal() const;
    uint16_t quantity() const;

    // Deleted Constructors
    Potion() = delete;
    Potion(const Potion&) = delete; // Removes copy constructor
    Potion(const Potion&&) = delete; // Removes move constructor

private:
    Potion(std::string name, Buff* buff, uint16_t quantity = 1u, welltype hp_heal = 0u,
        welltype mp_heal = 0u, bool overheal = false);
        // Private so that only the ItemManager class can use this constructor

    friend class ItemManager;
};



class EquipmentDelegate : public ItemDelegate {
    const uint32_t id = 0u;
    Stats stats;
public:
    stattype strength() const;
    stattype intellect() const;
    stattype agility() const;
    stattype armor() const;
    stattype resistance() const;
protected:
    EquipmentDelegate(std::string name, Stats stats);
    virtual ~EquipmentDelegate() = 0 {}//; // This virtual deconstructor might not actually be necessary
};



enum class ARMORSLOT { HELMET, CHEST, LEGS, BOOTS, GLOVES, RING1, RING2, NECKLACE, NUM_SLOTS };
    // NUM_SLOTS is strictly only used for making arrays of ARMORSLOTs

class Armor final : public EquipmentDelegate {
    ARMORSLOT slot_;

public:
    ARMORSLOT slot() const;

    // Deleted Constructors
    Armor() = delete;
    Armor(const Armor&) = delete;
    Armor(const Armor&&) = delete;

private:
    Armor(std::string name, Stats stats, ARMORSLOT slot);

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
    WEAPONSLOT slot();
    stattype min_damage();
    stattype max_damage();

    // Deleted Constructors
    Weapon() = delete;
    Weapon(const Weapon&) = delete;
    Weapon(const Weapon&&) = delete;

private:
    Weapon(std::string name, Stats stats, WEAPONSLOT slot, stattype min_damage,
        stattype max_damage, bool is_two_handed = false);

    friend class ItemManager;
};



// Use this class in your runtime code
class Item final {
    // No getters for, only used by friend classes
    ItemDelegate* data;
    bool marked_for_deletion;

private:
    Item(ItemDelegate* item_data);
    ~Item();
    /// TODO: If we end up implementing Item such that it points to whoever possesses it,
        // we will redefine the deconstructor and make it public again.

    friend class ItemManager;
    friend class PlayerCharacter;

    friend std::ostream& operator<<(std::ostream& os, const Item& item);
};
