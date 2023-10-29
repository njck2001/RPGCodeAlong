#pragma once
#include "Stats.h"
#include "Buff.h"
#include <vector>
#include <iostream>

// For learning purposes, access has been organized from most restrictive to least restrictive
    // Conventional class formatting orders sections like so: public, protected, and private
    // Typically, I like to keep private members at the top, before the public section
// Note: Derived classes will inherit as private by default unless otherwise specified
class StatBlock {
private:
/*  Members and methods cannot be accessed by anyone (including derived classes) except by the base class
        Note: This is regardless of whether a derived class inherits as private, protected, or public
*/
    
    Stats base_stats;

    std::vector<Buff> buffs_;
    Stats buff_stats;

    std::vector<Buff> debuffs_;
    Stats debuff_stats;

    void recalculate_buffs() {
        buff_stats = Stats();
        debuff_stats = Stats();
        
        for (auto buff : buffs_) {
            buff_stats += buff.stats;
        }
        for (auto debuff : debuffs_) {
            debuff_stats += debuff.stats;
        }
    }

protected: 
/*  Members and methods can be accessed by the base class and derived classes
    If inherited as private, derived classes will inherit protected members and methods as private members and methods
        Note: This means that derived classes of derived classes cannot access these members and methods
    If inherited as protected or public, derived classes will inherit protected members and methods as protected members and methods
        Note: This means that derived classes of derived classes can access these members and methods
*/
    
    void increase_stats(Stats amount) { base_stats += amount; }

    const std::vector<Buff>& buffs() { return buffs_; }
    const std::vector<Buff>& debuffs() { return debuffs_; }
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

public: 
/*  Members and methods can be accessed by anyone from the base class, by the base class, and by inherited classes
        Note: "from" the base class, meaning you can access these when using a base class object
    If inherited as private, derived classes will inherit public members and methods as private members and methods
        Note: This means that these members and methods cannot be accessed using a derived class object that uses private inheritance,
            nor can they be accessed by derived classes of derived classes
    If inherited as protected, derived classes will inherit public members and methods as protected members and methods
        Note: This means that these members and methods cannot be accessed using a derived class object that uses protected inheritance,
            but they can be accessed by derived classes of derived classes
    If inherited as public, derived classes will inherit public members and methods as public members and methods
        Note: This means that these members can be accessed using a derived class object that uses public inheritance,
            as well as derived classes of derived classes
*/
    StatBlock() : base_stats(Stats()) {}
    StatBlock(Stats stats) : base_stats(stats) {}

    stat_type strength() {
        stat_type sum = base_stats.strength + buff_stats.strength;
        return (sum < debuff_stats.strength) ? 0u : sum - debuff_stats.strength;
    }
    stat_type intellect() {
        stat_type sum = base_stats.intellect + buff_stats.intellect;
        return (sum < debuff_stats.intellect) ? 0u : sum - debuff_stats.intellect;
    }
    stat_type agility() {
        stat_type sum = base_stats.agility + buff_stats.agility;
        return (sum < debuff_stats.agility) ? 0u : sum - debuff_stats.agility;
    }
    stat_type armor() {
        stat_type sum = base_stats.armor + buff_stats.armor;
        return (sum < debuff_stats.armor) ? 0u : sum - debuff_stats.armor;
    }
    stat_type resistance() {
        stat_type sum = base_stats.resistance + buff_stats.resistance;
        return (sum < debuff_stats.resistance) ? 0u : sum - debuff_stats.resistance;
    }
};
