#pragma once
#include "types.h"
#include <string>
#include <vector>

struct Buff {
    bool is_debuff;

    std::string name;
    uint16_t duration;

    stat_type strength; 
    stat_type intellect;
    stat_type agility;
    stat_type armor;
    stat_type resistance;

    Buff() = delete; // Removes the default constructor so that it cannot be used
    Buff(std::string name, uint16_t duration, stat_type strength = 0, stat_type intellect = 0,
    stat_type agility = 0, stat_type armor = 0, stat_type resistance = 0, bool is_debuff = false) :
        name(name), duration(duration), strength(strength), intellect(intellect),
        agility(agility), armor(armor), resistance(resistance), is_debuff(is_debuff)
    {}
};
