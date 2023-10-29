#pragma once
#include "Buff.h"
#include "types.h"
#include <vector>

// For learning purposes, access has been organized from most restrictive to least restrictive
    // Conventional class formatting orders sections like so: public, protected, and private
    // Typically, I like to keep private members at the top, before the public section
// Note: Derived classes will inherit as private by default unless otherwise specified
class StatBlock {
private:
/*  Members and methods cannot be accessed by anyone (including derived classes) except by the base class
        Note: This is regardless of whether a derived class inherits as private, protected, or public
*/
    
    stat_type strength_; 
    stat_type intellect_;
    stat_type agility_;
    stat_type armor_;
    stat_type resistance_;

    std::vector<Buff> buffs_;
    stat_type total_strength_from_buffs = 0; 
    stat_type total_intellect_from_buffs = 0;
    stat_type total_agility_from_buffs = 0;
    stat_type total_armor_from_buffs = 0;
    stat_type total_resistance_from_buffs = 0;

    void recalculate_buffs() {
        int16_t tsfb = 0;   // total strength from buffs
        int16_t tifb = 0;   // total intellect from buffs
        int16_t tagfb = 0;  // total agility from buffs
        int16_t tarfb = 0;  // total armor from buffs
        int16_t trfb = 0;   // total resistance from buffs

        for (auto buff : buffs_) {
            if (buff.is_debuff) {
                tsfb -= buff.strength;
                tifb -= buff.intellect;
                tagfb -= buff.agility;
                tarfb -= buff.armor;
                trfb -= buff.resistance;
            }
            else {
                tsfb += buff.strength;
                tifb += buff.intellect;
                tagfb += buff.agility;
                tarfb += buff.armor;
                trfb += buff.resistance;
            }
        }

        if (tsfb < 0) { tsfb = 0; }
        if (tifb < 0) { tifb = 0; }
        if (tagfb < 0) { tagfb = 0; }
        if (tarfb < 0) { tarfb = 0; }
        if (trfb < 0) { trfb = 0; }

        total_strength_from_buffs = tsfb; 
        total_intellect_from_buffs = tifb;
        total_agility_from_buffs = tagfb;
        total_armor_from_buffs = tarfb;
        total_resistance_from_buffs = trfb;
    }

protected: 
/*  Members and methods can be accessed by the base class and derived classes
    If inherited as private, derived classes will inherit protected members and methods as private members and methods
        Note: This means that derived classes of derived classes cannot access these members and methods
    If inherited as protected or public, derived classes will inherit protected members and methods as protected members and methods
        Note: This means that derived classes of derived classes can access these members and methods
*/
    
    void increase_stats(stat_type strength = 0, stat_type intellect = 0, stat_type agility = 0,
    stat_type armor = 0, stat_type resistance = 0) {
        strength_ += strength;
        intellect_ += intellect;
        agility_ += agility;
        armor_ += armor;
        resistance_ += resistance;
    }

    const std::vector<Buff>& buffs() { return buffs_; }
    void add_buff(Buff buff) {
        for (auto& active_buff : buffs_) {
            if (buff.name == active_buff.name) {
                if (active_buff.duration < buff.duration)
                active_buff.duration = buff.duration;
                return;
            }
        }
        buffs_.push_back(buff);
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
    
    // "explicit" ensures that only stat_type (or std::uint16_t) variables are allowed to be passed into the constructor
    explicit StatBlock(stat_type strength = (stat_type)0u, stat_type intellect = (stat_type)0u,
    stat_type agility = (stat_type)0u, stat_type armor = (stat_type)0u, stat_type resistance = (stat_type)0u) :
        strength_(strength), intellect_(intellect), agility_(agility), armor_(armor), resistance_(resistance)
    {}

    stat_type strength() { return strength_ + total_strength_from_buffs; }
    stat_type intellect() { return intellect_ + total_intellect_from_buffs; }
    stat_type agility() { return agility_ + total_agility_from_buffs; }
    stat_type armor() { return armor_ + total_armor_from_buffs; }
    stat_type resistance() { return resistance_ + total_resistance_from_buffs; }
};
