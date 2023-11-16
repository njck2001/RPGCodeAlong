#pragma once
#include "Stats.h"
#include <string>


struct Buff {
    std::string name;
    uint16_t duration;
    Stats stats;
    bool is_debuff;

    Buff() = delete; // Removes the default constructor so that it cannot be used
    Buff(std::string name, uint16_t duration, Stats stats, bool is_debuff = false) :
        name(name), duration(duration), stats(stats), is_debuff(is_debuff)
    {}
};
