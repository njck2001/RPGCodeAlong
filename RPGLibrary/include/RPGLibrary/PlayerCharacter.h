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
    leveltype level() const { return level_; }
    exptype exp() { return exp_; }
    exptype exp_to_next_level() { return etnl; }
    const std::unique_ptr<PointWell>& hp() { return hp_; }
    const std::unique_ptr<PointWell>& mp() { return mp_; }
    const std::vector<Ability>& abilities() { return abilities_; }

    void gain_exp(exptype gained_exp) {
        if (exp_ > (MAX_EXPTYPE - gained_exp)) {
            exp_ = MAX_EXPTYPE;
        } 
        else {
            exp_ += gained_exp;
        }
        while (check_if_leveled()) { // While-loop in case we level up multiple times
            level_up();
        }
    }

protected:
    PlayerCharacterDelegate() :
        StatBlock(),
        level_(1u), exp_(0u), etnl(BASE_ETNL),
        hp_(std::make_unique<PointWell>()),
        mp_(nullptr)
    {} // Protected so that only derived classes can use this constructor
    
    void make_mp_well(welltype max_points, welltype points) {
        mp_ = std::make_unique<PointWell>(max_points, points);
    }
    void add_ability(std::string name, uint32_t cost, uint32_t cooldown, ABILITY_TARGET target,
    uint32_t hp_effect, ABILITY_SCALAR scalar = ABILITY_SCALAR::NONE) {
        abilities_.emplace_back(name, cost, cooldown, target, hp_effect, scalar);
    }

private:
    static const exptype BASE_ETNL = 100u;

    bool check_if_leveled() {
        bool etnl_grows_polynomially = false; // Else, etnl grows exponentially
        if (exp_ >= etnl) { 
            if (level_ < MAX_LEVELTYPE) {
                level_++;
                if (etnl_grows_polynomially) {
                    if (BASE_ETNL > (MAX_EXPTYPE / (exptype)level_)) {
                        etnl = MAX_EXPTYPE;
                    }
                    else if (etnl > (MAX_EXPTYPE - (BASE_ETNL * (exptype)level_))) {
                        etnl = MAX_EXPTYPE;
                    }
                    else {
                        etnl += BASE_ETNL * (exptype)level_;
                    }
                }
                else { // etnl grows exponentially
                    static const exptype ETNL_SCALAR = 2u;
                    if (etnl > (MAX_EXPTYPE / ETNL_SCALAR)) {
                        etnl = MAX_EXPTYPE;
                    }
                    else {
                        etnl *= ETNL_SCALAR;
                    }
                }
                return true;
            }
            exp_ = etnl; // EXP is upper-bounded by MAX_LEVELTYPE instead of MAX_EXPTYPE
        }
        return false;
    }

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



class PlayerCharacter {
    PlayerCharacterDelegate* pc_class;
    Item* equipped_armor_[(int)ARMORSLOT::NUM_SLOTS];
    Item* equipped_weapons_[(int)WEAPONSLOT::NUM_SLOTS];
    std::vector<Item*> backpack_;
        /// TODO: change backpack_ to inventory_

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
    std::string class_name() const { return pc_class->class_name(); }
    leveltype level() const { return pc_class->level(); }
    exptype exp() const { return pc_class->exp(); }
    exptype etnl() const { return pc_class->exp_to_next_level(); }
    welltype hp() const { return pc_class->hp()->points(); }
    welltype max_hp() const { return pc_class->hp()->max(); }
    welltype mp() const {
        if (pc_class->mp()) {
            return pc_class->mp()->points();
        }
        else {
            return 0u;
        }
    }
    welltype max_mp() const {
        if (pc_class->mp()) {
            return pc_class->mp()->max();
        }
        else {
            return 0u;
        }
    }
    stattype strength() const {
        stattype total_strength = pc_class->strength();
        int i;
        for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            if (equipped_armor_[i]) {
                Armor* armor = dynamic_cast<Armor*>(equipped_armor_[i]->data_);
                if (total_strength > (MAX_STATTYPE - armor->strength())) {
                    return MAX_STATTYPE;
                }
                else {
                    total_strength += armor->strength();
                }
            }
        }
        for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            if (equipped_weapons_[i]) {
                Weapon* weapon = dynamic_cast<Weapon*>(equipped_weapons_[i]->data_);
                if (total_strength > (MAX_STATTYPE - weapon->strength())) {
                    return MAX_STATTYPE;
                }
                else {
                    total_strength += weapon->strength();
                }
            }
        }
        return total_strength;
    }
    stattype intellect() const {
        stattype total_intellect = pc_class->intellect();
        int i;
        for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            if (equipped_armor_[i]) {
                Armor* armor = dynamic_cast<Armor*>(equipped_armor_[i]->data_);
                if (total_intellect > (MAX_STATTYPE - armor->intellect())) {
                    return MAX_STATTYPE;
                }
                else {
                    total_intellect += armor->intellect();
                }
            }
        }
        for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            if (equipped_weapons_[i]) {
                Weapon* weapon = dynamic_cast<Weapon*>(equipped_weapons_[i]->data_);
                if (total_intellect > (MAX_STATTYPE - weapon->intellect())) {
                    return MAX_STATTYPE;
                }
                else {
                    total_intellect += weapon->intellect();
                }
            }
        }
        return total_intellect;
    }
    stattype agility() const {
        stattype total_agility = pc_class->agility();
        int i;
        for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            if (equipped_armor_[i]) {
                Armor* armor = dynamic_cast<Armor*>(equipped_armor_[i]->data_);
                if (total_agility > (MAX_STATTYPE - armor->agility())) {
                    return MAX_STATTYPE;
                }
                else {
                    total_agility += armor->agility();
                }
            }
        }
        for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            if (equipped_weapons_[i]) {
                Weapon* weapon = dynamic_cast<Weapon*>(equipped_weapons_[i]->data_);
                if (total_agility > (MAX_STATTYPE - weapon->agility())) {
                    return MAX_STATTYPE;
                }
                else {
                    total_agility += weapon->agility();
                }
            }
        }
        return total_agility;
    }
    stattype armor() const {
        stattype total_armor = pc_class->armor();
        int i;
        for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            if (equipped_armor_[i]) {
                Armor* armor = dynamic_cast<Armor*>(equipped_armor_[i]->data_);
                if (total_armor > (MAX_STATTYPE - armor->armor())) {
                    return MAX_STATTYPE;
                }
                else {
                    total_armor += armor->armor();
                }
            }
        }
        for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            if (equipped_weapons_[i]) {
                Weapon* weapon = dynamic_cast<Weapon*>(equipped_weapons_[i]->data_);
                if (total_armor > (MAX_STATTYPE - weapon->armor())) {
                    return MAX_STATTYPE;
                }
                else {
                    total_armor += weapon->armor();
                }
            }
        }
        return total_armor;
    }
    stattype resistance() const {
        stattype total_resistance = pc_class->resistance();
        int i;
        for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            if (equipped_armor_[i]) {
                Armor* armor = dynamic_cast<Armor*>(equipped_armor_[i]->data_);
                if (total_resistance > (MAX_STATTYPE - armor->resistance())) {
                    return MAX_STATTYPE;
                }
                else {
                    total_resistance += armor->resistance();
                }
            }
        }
        for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            if (equipped_weapons_[i]) {
                Weapon* weapon = dynamic_cast<Weapon*>(equipped_weapons_[i]->data_);
                if (total_resistance > (MAX_STATTYPE - weapon->resistance())) {
                    return MAX_STATTYPE;
                }
                else {
                    total_resistance += weapon->resistance();
                }
            }
        }
        return total_resistance;
    }
    const std::vector<Ability> abilities() const { return pc_class->abilities(); }
    const std::vector<Buff>& buffs() const { return pc_class->buffs(); }
    const std::vector<Buff>& debuffs() const { return pc_class->debuffs(); }
    const std::vector<Item*> backpack() const { return backpack_; }
    Armor* equipped_armor(ARMORSLOT slot) const { 
        if (!equipped_armor_[(int)slot]) {
            return nullptr;
        }
        else {
            return dynamic_cast<Armor*>(equipped_armor_[(int)slot]->data_);
        }
    }
    Armor* equipped_armor(int slot) const {
        if (!equipped_armor_[slot]) {
            return nullptr;
        }
        else {
            return dynamic_cast<Armor*>(equipped_armor_[slot]->data_);
        }
    }
    Weapon* equipped_weapon(WEAPONSLOT slot) const { 
        if (!equipped_weapons_[(int)slot]) {
            return nullptr;
        }
        else {
            return dynamic_cast<Weapon*>(equipped_weapons_[(int)slot]->data_);
        }
    }
    Weapon* equipped_weapon(int slot) const {
        if (!equipped_weapons_[slot]) {
            return nullptr;
        }
        else {
            return dynamic_cast<Weapon*>(equipped_weapons_[slot]->data_);
        }
    }
    
    // Setters
    void gain_exp(exptype amount) { pc_class->gain_exp(amount); }
    void damage(welltype amount) { pc_class->hp()->decrease(amount); }
    void heal(welltype amount, bool allow_overheal = false) { pc_class->hp()->increase(amount, allow_overheal); }
    void heal_hp(welltype amount, bool allow_overheal = false) { pc_class->hp()->increase(amount, allow_overheal); }
        // Does the same thing as heal(), but the name of this function makes its purpose more clear
    bool heal_mp(welltype amount, bool allow_overheal = false) {
        if (pc_class->mp()) {
            pc_class->mp()->increase(amount, allow_overheal);
            return true;
        }
        else {
            return false;
        }
    }
    void add_buff(Buff buff) { pc_class->add_buff(buff); }

    // Setters defined at bottom of ItemManager.h
    bool equip(Item* item);
    bool use(Item* item);
    bool pick_up(Item* item);

    // Deleted Constructors
    PlayerCharacter() = delete; // Removes the default constructor so that it cannot be used
    PlayerCharacter(const PlayerCharacter&) = delete; // Removes copy constructor
    PlayerCharacter(const PlayerCharacter&&) = delete; // Removes move constructor

private:
    void cleanup_backpack() {
        const auto to_remove = std::stable_partition(backpack_.begin(), backpack_.end(),
            [](const Item* item) -> bool { return !item->marked_for_deletion; }
        );
        std::for_each(to_remove, backpack_.end(), [](Item* item) { delete item; item = nullptr; });
        backpack_.erase(to_remove, backpack_.end());
    }

    friend class ItemManager;
};
