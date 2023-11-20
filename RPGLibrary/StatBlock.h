#pragma once
#include "Stats.h"
#include "Buff.h"
#include <vector>
#include <iostream>


/*  For educational purposes, this class contains comments for explaining access specifiers and class inheritance
    Depending on the IDE you are using, you may be able to minimize block comments to make the code easier to read
    Conventional class formatting orders access specifiers from least restictive to most restrictive (public, protected, private)
    Derived classes will inherit as private by default (e.g. Derived : Base) unless otherwise specified (e.g. Derived : public Base)
*/
class StatBlock {
    /*  Typically, I like to keep private members at the top, before access specifiers
            Note: Access is set to private by default in classes
        For the sake of clarity, I've refained from doing so here
        All private members and methods can be found underneath the private access specifier
    */

public: 
/*  Members and methods can be accessed by the base class, by base class objects (e.g. statblock_obj.strength()), and by derived classes
    If inherited as public, derived classes will inherit public members and methods as public members and methods
        Note: This means that these members can be accessed by derived class objects that use public inheritance
            as well as derived classes of derived classes
    If inherited as protected, derived classes will inherit public members and methods as protected members and methods
        Note: This means that these members and methods cannot be accessed by derived class objects that use protected inheritance,
            but they can still be accessed by derived classes of derived classes
    If inherited as private, derived classes will inherit public members and methods as private members and methods
        Note: This means that these members and methods cannot be accessed by derived class objects that use private inheritance,
            nor can they be accessed by derived classes of derived classes
*/
    
    StatBlock() : base_stats(Stats()) {}
    StatBlock(Stats stats) : base_stats(stats) {}

    stattype strength() const {
        if (total_buff_stats.strength < total_debuff_stats.strength) {
            stattype leftover_debuff_strength = total_debuff_stats.strength - total_buff_stats.strength;
            if (base_stats.strength < leftover_debuff_strength) {
                return 0u;
            }
            else {
                return base_stats.strength - leftover_debuff_strength;
            }
        }
        else {
            stattype leftover_buff_strength = total_buff_stats.strength - total_debuff_stats.strength;
            if (base_stats.strength > (MAX_STATTYPE - leftover_buff_strength)) {
                return MAX_STATTYPE;
            }
            else {
                return base_stats.strength + leftover_buff_strength;
            }
        }
    }
    stattype intellect() const {
        if (total_buff_stats.intellect < total_debuff_stats.intellect) {
            stattype leftover_debuff_intellect = total_debuff_stats.intellect - total_buff_stats.intellect;
            if (base_stats.intellect < leftover_debuff_intellect) {
                return 0u;
            }
            else {
                return base_stats.intellect - leftover_debuff_intellect;
            }
        }
        else {
            stattype leftover_buff_intellect = total_buff_stats.intellect - total_debuff_stats.intellect;
            if (base_stats.intellect > (MAX_STATTYPE - leftover_buff_intellect)) {
                return MAX_STATTYPE;
            }
            else {
                return base_stats.intellect + leftover_buff_intellect;
            }
        }
    }
    stattype agility() const {
        if (total_buff_stats.agility < total_debuff_stats.agility) {
            stattype leftover_debuff_agility = total_debuff_stats.agility - total_buff_stats.agility;
            if (base_stats.agility < leftover_debuff_agility) {
                return 0u;
            }
            else {
                return base_stats.agility - leftover_debuff_agility;
            }
        }
        else {
            stattype leftover_buff_agility = total_buff_stats.agility - total_debuff_stats.agility;
            if (base_stats.agility > (MAX_STATTYPE - leftover_buff_agility)) {
                return MAX_STATTYPE;
            }
            else {
                return base_stats.agility + leftover_buff_agility;
            }
        }
    }
    stattype armor() const {
        if (total_buff_stats.armor < total_debuff_stats.armor) {
            stattype leftover_debuff_armor = total_debuff_stats.armor - total_buff_stats.armor;
            if (base_stats.armor < leftover_debuff_armor) {
                return 0u;
            }
            else {
                return base_stats.armor - leftover_debuff_armor;
            }
        }
        else {
            stattype leftover_buff_armor = total_buff_stats.armor - total_debuff_stats.armor;
            if (base_stats.armor > (MAX_STATTYPE - leftover_buff_armor)) {
                return MAX_STATTYPE;
            }
            else {
                return base_stats.armor + leftover_buff_armor;
            }
        }
    }
    stattype resistance() const {
        if (total_buff_stats.resistance < total_debuff_stats.resistance) {
            stattype leftover_debuff_resistance = total_debuff_stats.resistance - total_buff_stats.resistance;
            if (base_stats.resistance < leftover_debuff_resistance) {
                return 0u;
            }
            else {
                return base_stats.resistance - leftover_debuff_resistance;
            }
        }
        else {
            stattype leftover_buff_resistance = total_buff_stats.resistance - total_debuff_stats.resistance;
            if (base_stats.resistance > (MAX_STATTYPE - leftover_buff_resistance)) {
                return MAX_STATTYPE;
            }
            else {
                return base_stats.resistance + leftover_buff_resistance;
            }
        }
    }
    const std::vector<Buff>& buffs() const { return buffs_; }
    const std::vector<Buff>& debuffs() const { return debuffs_; }

    void add_buff(Buff buff) {
        if (buff.is_debuff) {
            for (auto& active_debuff : debuffs_) {
                if (buff.name == active_debuff.name) {
                    if (active_debuff.duration < buff.duration) {
                        active_debuff.duration = buff.duration;
                    }
                    return;
                }
            }
            debuffs_.push_back(buff);
        }
        else {
            for (auto& active_buff : buffs_) {
                if (buff.name == active_buff.name) {
                    if (active_buff.duration < buff.duration) {
                        active_buff.duration = buff.duration;
                    }
                    return;
                }
            }
            buffs_.push_back(buff);
        }
        recalculate_buffs();
    }

protected: 
/*  Members and methods can be accessed by the base class and derived classes
    If inherited as public or protected, derived classes will inherit protected members and methods as protected members and methods
        Note: This means that derived classes of derived classes can also access these members and methods
    If inherited as private, derived classes will inherit protected members and methods as private members and methods
        Note: This means that derived classes of derived classes cannot access these members and methods
*/

    // Protected Setter - only used by derived classes
    void increase_stats(Stats amount) { base_stats += amount; }

private:
/*  Members and methods cannot be accessed by anyone (including derived classes) except by the base class
        Note: This is regardless of whether a derived class inherits as private, protected, or public
*/
    
    // Normally, I would have put these member variables at the top of the class
    Stats base_stats;
    std::vector<Buff> buffs_;
    Stats total_buff_stats;
    std::vector<Buff> debuffs_;
    Stats total_debuff_stats;

    // Helper function (helper functions are typically always private)
    void recalculate_buffs() {
        total_buff_stats = Stats(); // All buff stats are set to 0
        total_debuff_stats = Stats(); // All debuff stats are set to 0
        for (auto buff : buffs_) {
            total_buff_stats += buff.stats;
        }
        for (auto debuff : debuffs_) {
            total_debuff_stats += debuff.stats;
        }
    }
};
