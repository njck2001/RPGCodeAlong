#pragma once
#include "StatBlock.h"
#include "PointWell.h"
#include "Ability.h"
#include "Item.h"
#include "types.h"
#include <memory> // unique_ptr
#include <string>
#include <vector>
#include <algorithm> // stable_partition, for_each



class PlayerCharacterDelegate : public StatBlock {
    leveltype level_;
    exptype exp_;
    exptype etnl; // EXP to next level
    std::unique_ptr<PointWell> hp_;
    std::unique_ptr<PointWell> mp_;
    std::vector<Ability> abilities_;

public:
    leveltype level() const;
    exptype exp() const;
    exptype exp_to_next_level() const;
    const std::unique_ptr<PointWell>& hp() const;
    const std::unique_ptr<PointWell>& mp() const;
    const std::vector<Ability>& abilities() const;

    void gain_exp(exptype gained_exp);

protected:
    PlayerCharacterDelegate(); // Protected so that only derived classes can use this constructor
    
    void make_mp_well(welltype max_points, welltype points);
    void add_ability(std::string name, uint32_t cost, uint32_t cooldown, ABILITY_TARGET target,
        uint32_t hp_effect, ABILITY_SCALAR scalar = ABILITY_SCALAR::NONE);

private:
    bool check_if_leveled();

// Virtual functions (stored together at the bottom for educational purposes)
public:
    virtual std::string class_name() = 0; // Public because we want to call this outside of the base class
private:
    virtual void level_up() = 0; // Private because we do not expect this to be called outside of the base class
/*  Unlike functions such as gain_exp() and add_buff(), level_up() and class_name()
    work differently depending on the character-class. For example, for Warrior,
    we want class_name() to return "Warrior", but for Cleric, we want class_name()
    to return "Cleric". Therefore, we want each character-class to define these
    functions differently. By making these virtual functions, we force
    character-classes to override the functions with their own definitions.
*/
};



/// Leave all of this here for now
/// We'll create files for Macros and Character Classes later
///////////////////////////////////////////////////////////////////////////////////////////////
#define PC_CONSTRUCT \
hp()->set_max(BASE_HP); \
hp()->increase(BASE_HP); \
if (mp()) { \
    mp()->set_max(BASE_MP); \
    mp()->increase(BASE_MP); \
} \
increase_stats(Stats(BASE_STR, BASE_INT, BASE_AGI));

#define LEVEL_UP \
if (hp()->max() > (MAX_WELLTYPE - HP_GROWTH)) { \
    hp()->set_max(MAX_WELLTYPE); \
} \
else { \
    hp()->set_max(hp()->max() + HP_GROWTH); \
} \
hp()->increase(HP_GROWTH); \
if (mp()) { \
    if (mp()->max() > (MAX_WELLTYPE - MP_GROWTH)) { \
        mp()->set_max(MAX_WELLTYPE); \
    } \
    else { \
        mp()->set_max(mp()->max() + MP_GROWTH); \
    } \
    mp()->increase(MP_GROWTH); \
} \
increase_stats(Stats(STR_GROWTH, INT_GROWTH, AGI_GROWTH));


class Warrior : public PlayerCharacterDelegate {
    /// TODO: Needs balancing
    static const welltype BASE_HP = 18u;   // Original value was 18
    /*  ^ static within a class means this variable will only
        be instantiated once, regardless of how many Warrior
        objects are created; in other words, every Warrior
        object uses the same BASE_HP variable in memory
        rather than create a new variable for every Warrior
        object, each taking up four bytes of memory; every
        Warrior object shares these exact same four bytes in
        memory, which represents the BASE_HP for each of
        those Warrior objects
    */
    static const welltype BASE_MP = 0u;    // Original value was 0
    static const stattype BASE_STR = 6u;   // Original value was 6
    static const stattype BASE_INT = 2u;   // Original value was 2
    static const stattype BASE_AGI = 2u;   // Original value was 2

    static const welltype HP_GROWTH = 9u;  // Original value was 9
    static const welltype MP_GROWTH = 0u;  // Original value was 0
    static const stattype STR_GROWTH = 3u; // Original value was 3
    static const stattype INT_GROWTH = 1u; // Original value was 1
    static const welltype AGI_GROWTH = 1u; // Original value was 1

public:
    Warrior() : PlayerCharacterDelegate() {
        PC_CONSTRUCT
    }

    std::string class_name() { return "Warrior"; }

private:
    void level_up() override {
        LEVEL_UP
        if (level() == 2) {
            add_ability("PowerAttack", 0u, 3u, ABILITY_TARGET::ENEMY, 4u, ABILITY_SCALAR::STR);
        }
    }
};


class Wizard : public PlayerCharacterDelegate {
    /// TODO: Needs balancing
    static const welltype BASE_HP = 10u;   // Original value was 10
    static const welltype BASE_MP = 14u;   // Original value was 14
    static const stattype BASE_STR = 1u;   // Original value was 1
    static const stattype BASE_INT = 8u;   // Original value was 8
    static const stattype BASE_AGI = 2u;   // Original value was 2

    static const welltype HP_GROWTH = 5u;  // Original value was 5
    static const welltype MP_GROWTH = 7u;  // Original value was 7
    static const stattype STR_GROWTH = 1u; // Original value was 1
    static const stattype INT_GROWTH = 4u; // Original value was 4
    static const welltype AGI_GROWTH = 1u; // Original value was 1

public:
    Wizard() : PlayerCharacterDelegate() {
        make_mp_well(BASE_MP, BASE_MP); // Be sure to initialize before PCCONSTRUCT MACRO
        PC_CONSTRUCT
        add_ability("Firebolt", 2u, 1u, ABILITY_TARGET::ENEMY, 4u, ABILITY_SCALAR::INT);
    }

    std::string class_name() { return "Wizard"; }

private:
    void level_up() override {
        LEVEL_UP
        if (level() == 2) {
            add_ability("Icebolt", 3u, 1u, ABILITY_TARGET::ENEMY, 6u, ABILITY_SCALAR::INT);
            if (mp()->max() < MAX_WELLTYPE) {
                mp()->set_max(mp()->max() + 1u);
                mp()->increase(1u);
            }
            increase_stats(Stats(0u, 1u));
        } 
    }
};


class Cleric : public PlayerCharacterDelegate {
    /// TODO: Needs balancing
    static const welltype BASE_HP = 14u;   // Original value was 14
    static const welltype BASE_MP = 10u;   // Original value was 10
    static const stattype BASE_STR = 3u;   // Original value was 3
    static const stattype BASE_INT = 5u;   // Original value was 5
    static const stattype BASE_AGI = 1u;   // Original value was 1

    static const welltype HP_GROWTH = 7u;  // Original value was 7
    static const welltype MP_GROWTH = 5u;  // Original value was 5
    static const stattype STR_GROWTH = 2u; // Original value was 2
    static const stattype INT_GROWTH = 3u; // Original value was 3
    static const welltype AGI_GROWTH = 1u; // Original value was 1

public:
    Cleric() : PlayerCharacterDelegate() {
        make_mp_well(BASE_MP, BASE_MP); // Be sure to initialize before PCCONSTRUCT MACRO
        PC_CONSTRUCT
        add_ability("Heal", 2u, 0u, ABILITY_TARGET::ALLY, 2u, ABILITY_SCALAR::INT);
    }

    std::string class_name() { return "Cleric"; }

private:
    void level_up() override {
        LEVEL_UP
        if (level() == 2) {
            add_ability("Smite", 2u, 1u, ABILITY_TARGET::ENEMY, 2u, ABILITY_SCALAR::INT);
        } 
    }
};


class Rogue : public PlayerCharacterDelegate {
    /// TODO: Needs balancing
    static const welltype BASE_HP = 12u;   // Original value was 12
    static const welltype BASE_MP = 0u;    // Original value was 0
    static const stattype BASE_STR = 3u;   // Original value was 3
    static const stattype BASE_INT = 3u;   // Original value was 3
    static const stattype BASE_AGI = 6u;   // Original value was 6

    static const welltype HP_GROWTH = 6u;  // Original value was 6
    static const welltype MP_GROWTH = 0u;  // Original value was 0
    static const stattype STR_GROWTH = 2u; // Original value was 2
    static const stattype INT_GROWTH = 2u; // Original value was 2
    static const welltype AGI_GROWTH = 3u; // Original value was 3

public:
    Rogue() : PlayerCharacterDelegate() {
        PC_CONSTRUCT
    }

    std::string class_name() { return "Rouge"; }

private:
    void level_up() override {
        LEVEL_UP
    }
};
///////////////////////////////////////////////////////////////////////////////////////////////



class PlayerCharacter final {
    PlayerCharacterDelegate* pc_class;
    Item* equipped_armor_[(int)ARMORSLOT::NUM_SLOTS];
    Item* equipped_weapons_[(int)WEAPONSLOT::NUM_SLOTS];
    std::vector<Item*> backpack_;
        /// TODO: change backpack_ to inventory_

public:
    PlayerCharacter(PlayerCharacterDelegate* pc_class);

    ~PlayerCharacter();

    // Getters
    std::string class_name() const;
    leveltype level() const;
    exptype exp() const;
    exptype etnl() const;
    welltype hp() const;
    welltype max_hp() const;
    welltype mp() const;
    welltype max_mp() const;
    stattype strength() const;
    stattype intellect() const;
    stattype agility() const;
    stattype armor() const;
    stattype resistance() const;
    const std::vector<Ability> abilities() const;
    const std::vector<Buff>& buffs() const;
    const std::vector<Buff>& debuffs() const;
    const std::vector<Item*> backpack() const;
    Armor* equipped_armor(ARMORSLOT slot) const;
    Armor* equipped_armor(int slot) const;
    Weapon* equipped_weapon(WEAPONSLOT slot) const;
    Weapon* equipped_weapon(int slot) const;
    
    // Setters
    void gain_exp(exptype amount);
    void damage(welltype amount);
    void heal(welltype amount, bool allow_overheal = false);
    void heal_hp(welltype amount, bool allow_overheal = false);
        // Does the same thing as heal(), but the name of this function makes its purpose more clear
    bool heal_mp(welltype amount, bool allow_overheal = false);
    void add_buff(Buff buff);
    bool equip(Item* item);
    bool use(Item* item);
    bool pick_up(Item* item);

    // Deleted Constructors
    PlayerCharacter() = delete; // Removes the default constructor so that it cannot be used
    PlayerCharacter(const PlayerCharacter&) = delete; // Removes copy constructor
    PlayerCharacter(const PlayerCharacter&&) = delete; // Removes move constructor

private:
    void cleanup_backpack();

    friend class ItemManager;
};
