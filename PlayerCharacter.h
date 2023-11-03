#pragma once
#include "StatBlock.h"
#include "PointWell.h"
#include "Ability.h"
#include "Item.h"
#include "types.h"
#include <memory> // unique_ptr
#include <string>
#include <vector>


class PlayerCharacterDelegate : public StatBlock {
    level_type level_;
    exp_type exp_;
    exp_type etnl; // exp to next level

    std::unique_ptr<HP> hp_;
    std::unique_ptr<PointWell> mp_;
    std::vector<Ability> abilities_;

    static const exp_type BASE_ETNL = 100u;

public:
    void gain_exp(exp_type gained_exp) {
        exp_ += gained_exp;
        while (check_if_leveled()) {} // while loop in case we level up multiple times
    }
    void add_buff(const Buff& buff) { StatBlock::add_buff(buff); }

    // Getters
    level_type level() { return level_; }
    exp_type exp() { return exp_; }
    exp_type exp_to_next_level() { return etnl; }
    const std::unique_ptr<HP>& hp() { return hp_; }
    const std::unique_ptr<PointWell>& mp() { return mp_; }
    const std::vector<Ability>& abilities() { return abilities_; }

protected:
    PlayerCharacterDelegate() :
        StatBlock(),
        level_(1u), exp_(0u), etnl(BASE_ETNL),
        hp_(std::make_unique<HP>()),
        mp_(nullptr)
    {}
    
    void make_mp_well(well_type max_points, well_type points) {
        mp_ = std::make_unique<PointWell>(max_points, points);
    }
    void add_ability(std::string name, uint32_t cost, uint32_t cooldown, ABILITY_TARGET target,
    uint32_t hp_effect, ABILITY_SCALAR scalar = ABILITY_SCALAR::NONE) {
        abilities_.emplace_back(name, cost, cooldown, target, hp_effect, scalar);
    }

private:
    bool check_if_leveled() {
        bool etnl_grows_polynomially = false; // else, etnl grows exponentially

        if (exp_ >= etnl) { 
            level_++;
            level_up();

            if (etnl_grows_polynomially) {
                etnl += BASE_ETNL * (exp_type)level_;
            }
            else { // etnl grows exponentially
                static const exp_type ETNL_SCALAR = 2u;
                etnl *= ETNL_SCALAR;
            }

            return true;
        }

        return false;
    }

// Virtual functions (stored together at the bottom for educational purposes)
public:
    virtual std::string class_name() = 0; // public because we want to call this outside of the base class
private:
    virtual void level_up() = 0; // private because we do not expect this to be called outside of the base class
/*  Unlike functions such as gain_exp() and add_buff(), level_up() and class_name()
    work differently depending on the character-class. For example, for Warrior,
    we want class_name() to return "Warrior", but for Cleric, we want class_name()
    to return "Cleric". Therefore, we want each character-class to define these
    functions differently. By making these virtual functions, we force
    character-classes to override the functions with their own definitions.
*/
};


#define PC_CONSTRUCT \
hp()->set_max(BASE_HP); \
hp()->increase(BASE_HP); \
hp()->give_shield(); \
if (mp()) { \
    mp()->set_max(BASE_MP); \
    mp()->increase(BASE_MP); \
} \
increase_stats(Stats(BASE_STR, BASE_INT, BASE_AGI));

#define LEVEL_UP \
hp()->set_max(hp()->max() + HP_GROWTH); \
hp()->increase(HP_GROWTH); \
hp()->set_shield_max(hp()->shield_max() + 1u); \
if (mp()) { \
    mp()->set_max(mp()->max() + MP_GROWTH); \
    mp()->increase(MP_GROWTH); \
} \
increase_stats(Stats(STR_GROWTH, INT_GROWTH, AGI_GROWTH));

class Warrior : public PlayerCharacterDelegate {
public:
    /// TODO: Needs balancing
    static const well_type BASE_HP = 18u;   // original value was 18
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
    static const well_type BASE_MP = 0u;    // original value was 0
    static const stat_type BASE_STR = 6u;   // original value was 6
    static const stat_type BASE_INT = 2u;   // original value was 2
    static const stat_type BASE_AGI = 2u;   // original value was 2

    static const well_type HP_GROWTH = 9u;  // original value was 9
    static const well_type MP_GROWTH = 0u;  // original value was 0
    static const stat_type STR_GROWTH = 3u; // original value was 3
    static const stat_type INT_GROWTH = 1u; // original value was 1
    static const well_type AGI_GROWTH = 1u; // original value was 1

    Warrior() : PlayerCharacterDelegate() {
        PC_CONSTRUCT
    }

    std::string class_name() { return "Warrior"; }

private:
    void level_up() override {
        LEVEL_UP
        if (level() == 2) {
            add_ability("PowerAttack", 0u, 3u, ABILITY_TARGET::ENEMY, 4u, ABILITY_SCALAR::STR);

            hp()->set_shield_max(hp()->shield_max() + 1u);
        }
    }
};

class Wizard : public PlayerCharacterDelegate {
public:
    /// TODO: Needs balancing
    static const well_type BASE_HP = 10u;   // original value was 10
    static const well_type BASE_MP = 14u;   // original value was 14
    static const stat_type BASE_STR = 1u;   // original value was 1
    static const stat_type BASE_INT = 8u;   // original value was 8
    static const stat_type BASE_AGI = 2u;   // original value was 2

    static const well_type HP_GROWTH = 5u;  // original value was 5
    static const well_type MP_GROWTH = 7u;  // original value was 7
    static const stat_type STR_GROWTH = 1u; // original value was 1
    static const stat_type INT_GROWTH = 4u; // original value was 4
    static const well_type AGI_GROWTH = 1u; // original value was 1

    Wizard() : PlayerCharacterDelegate() {
        make_mp_well(BASE_MP, BASE_MP); // be sure to init before PCCONSTRUCT MACRO
        PC_CONSTRUCT
        add_ability("Firebolt", 2u, 1u, ABILITY_TARGET::ENEMY, 4u, ABILITY_SCALAR::INT);
    }

    std::string class_name() { return "Wizard"; }

private:
    void level_up() override {
        LEVEL_UP
        if (level() == 2) {
            add_ability("Icebolt", 3u, 1u, ABILITY_TARGET::ENEMY, 6u, ABILITY_SCALAR::INT);

            mp()->set_max(mp()->max() + 1u);
            mp()->increase(1u);

            increase_stats(Stats(0, 1));
        } 
    }
};

class Cleric : public PlayerCharacterDelegate {
public:
    /// TODO: Needs balancing
    static const well_type BASE_HP = 14u;   // original value was 14
    static const well_type BASE_MP = 10u;   // original value was 10
    static const stat_type BASE_STR = 3u;   // original value was 3
    static const stat_type BASE_INT = 5u;   // original value was 5
    static const stat_type BASE_AGI = 1u;   // original value was 1

    static const well_type HP_GROWTH = 7u;  // original value was 7
    static const well_type MP_GROWTH = 5u;  // original value was 5
    static const stat_type STR_GROWTH = 2u; // original value was 2
    static const stat_type INT_GROWTH = 3u; // original value was 3
    static const well_type AGI_GROWTH = 1u; // original value was 1

    Cleric() : PlayerCharacterDelegate() {
        make_mp_well(BASE_MP, BASE_MP); // be sure to init before PCCONSTRUCT MACRO
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
public:
    /// TODO: Needs balancing
    static const well_type BASE_HP = 12u;   // original value was 12
    static const well_type BASE_MP = 0u;    // original value was 0
    static const stat_type BASE_STR = 3u;   // original value was 3
    static const stat_type BASE_INT = 3u;   // original value was 3
    static const stat_type BASE_AGI = 6u;   // original value was 6

    static const well_type HP_GROWTH = 6u;  // original value was 6
    static const well_type MP_GROWTH = 0u;  // original value was 0
    static const stat_type STR_GROWTH = 2u; // original value was 2
    static const stat_type INT_GROWTH = 2u; // original value was 2
    static const well_type AGI_GROWTH = 3u; // original value was 3

    Rogue() : PlayerCharacterDelegate() {
        PC_CONSTRUCT
    }

    std::string class_name() { return "Rouge"; }

private:
    void level_up() override {
        LEVEL_UP
    }
};


class PlayerCharacter {
    PlayerCharacterDelegate* pc_class;
    Armor* equipped_armor_[(int)ARMORSLOT::NUM_SLOTS];
    Weapon* equipped_weapons_[(int)WEAPONSLOT::NUM_SLOTS];

public:
    PlayerCharacter(PlayerCharacterDelegate* pc_class) : pc_class(pc_class) {
        int i;
        for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            equipped_armor_[i] = nullptr;
        }
        for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            equipped_weapons_[i] = nullptr;
        }
    }

    ~PlayerCharacter() {
        delete pc_class;
        pc_class = nullptr;
        
        int i;
        for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            if (equipped_armor_[i]) {
                delete equipped_armor_[i];
                equipped_armor_[i] = nullptr;
            }
        }
        for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            if (equipped_weapons_[i]) {
                delete equipped_weapons_[i];
                equipped_weapons_[i] = nullptr;
            }
        }
    }

    // Getters
    std::string class_name() { return pc_class->class_name(); }
    level_type level() { return pc_class->level(); }
    exp_type exp() { return pc_class->exp(); }
    exp_type etnl() { return pc_class->exp_to_next_level(); }
    
    well_type hp() { return pc_class->hp()->points(); }
    well_type max_hp() { return pc_class->hp()->max(); }
    well_type shield() { return pc_class->hp()->shield(); }
    well_type max_shield() { return pc_class->hp()->shield_max(); }

    well_type mp() {
        if (pc_class->mp()) {
            return pc_class->mp()->points();
        }
        return 0u;
    }
    well_type max_mp() {
        if (pc_class->mp()) {
            return pc_class->mp()->max();
        }
        return 0u;
    }

    stat_type strength() {
        stat_type total_strength = pc_class->strength();
        int i;
        for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            if (equipped_armor_[i]) {
                total_strength += equipped_armor_[i]->stats().strength;
            }
        }
        for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            if (equipped_weapons_[i]) {
                total_strength += equipped_weapons_[i]->stats().strength;
            }
        }
        return total_strength;
    }
    stat_type intellect() {
        stat_type total_intellect = pc_class->intellect();
        int i;
        for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            if (equipped_armor_[i]) {
                total_intellect += equipped_armor_[i]->stats().intellect;
            }
        }
        for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            if (equipped_weapons_[i]) {
                total_intellect += equipped_weapons_[i]->stats().intellect;
            }
        }
        return total_intellect;
    }
    stat_type agility() {
        stat_type total_agility = pc_class->agility();
        int i;
        for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            if (equipped_armor_[i]) {
                total_agility += equipped_armor_[i]->stats().agility;
            }
        }
        for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            if (equipped_weapons_[i]) {
                total_agility += equipped_weapons_[i]->stats().agility;
            }
        }
        return total_agility;
    }
    stat_type armor() {
        stat_type total_armor = pc_class->armor();
        int i;
        for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            if (equipped_armor_[i]) {
                total_armor += equipped_armor_[i]->stats().armor;
            }
        }
        for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            if (equipped_weapons_[i]) {
                total_armor += equipped_weapons_[i]->stats().armor;
            }
        }
        return total_armor;
    }
    stat_type resistance() {
        stat_type total_resistance = pc_class->resistance();
        int i;
        for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            if (equipped_armor_[i]) {
                total_resistance += equipped_armor_[i]->stats().resistance;
            }
        }
        for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            if (equipped_weapons_[i]) {
                total_resistance += equipped_weapons_[i]->stats().resistance;
            }
        }
        return total_resistance;
    }
    std::vector<Ability> abilities() { return pc_class->abilities(); }

    EquipmentDelegate* equipped_armor(ARMORSLOT slot) { return equipped_armor_[(int)slot]; }
    EquipmentDelegate* equipped_armor(int slot) { return equipped_armor_[slot]; }
    EquipmentDelegate* equipped_weapon(WEAPONSLOT slot) { return equipped_weapons_[(int)slot]; }
    EquipmentDelegate* equipped_weapon(int slot) { return equipped_weapons_[slot]; }
    
    // Modifiers
    void gain_exp(exp_type amount) { pc_class->gain_exp(amount); }
    void damage(well_type amount) { pc_class->hp()->decrease(amount); }
    void heal(well_type amount) { pc_class->hp()->increase(amount); }
    bool heal_shield(well_type amount) { return pc_class->hp()->increase_shield(amount); }
    void add_buff(Buff buff) { pc_class->add_buff(buff); }

    /// TODO: Update once we implement an inventory
    bool equip(Item* item) {
        if (!item) {
            return false;
        }
        if (!item->data()) {
            return false;
        }

        Armor* armor = dynamic_cast<Armor*>(item->data_);
        if (armor) {
            // Equip armor
            int slot_num = (int)armor->slot();
            if (equipped_armor_[slot_num]) {
                delete equipped_armor_[slot_num];
                /// TODO: move to inventory instead of delete
                equipped_armor_[slot_num] = armor;
            }
            else {
                equipped_armor_[slot_num] = armor;
            }

            return true;
        }
        
        Weapon* weapon = dynamic_cast<Weapon*>(item->data_);
        if (weapon) {
            // Equip weapon
            int slot_num = (int)weapon->slot();
            if (equipped_weapons_[slot_num]) {
                delete equipped_weapons_[slot_num];
                /// TODO: move to inventory instead of delete
                equipped_weapons_[slot_num] = weapon;
            }
            else {
                equipped_weapons_[slot_num] = weapon;
            }

            return true;
        }

        return false;
    }

    // Deleted Constructors
    PlayerCharacter() = delete; // Removes the default constructor so that it cannot be used
    PlayerCharacter(const PlayerCharacter&) = delete; // Removes copy constructor
    PlayerCharacter(const PlayerCharacter&&) = delete; // Removes move constructor
};
