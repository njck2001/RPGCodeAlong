#pragma once
#include "types.h"


struct Stats {
    stat_type strength; 
    stat_type intellect;
    stat_type agility;
    stat_type armor;
    stat_type resistance;

    Stats(stat_type strength = 0, stat_type intellect = 0, stat_type agility = 0,
    stat_type armor = 0, stat_type resistance = 0) :
        strength(strength), intellect(intellect), agility(agility),
        armor(armor), resistance(resistance)
    {}

    Stats& operator+=(const Stats& rhs) {
        this->strength += rhs.strength;
        this->intellect += rhs.intellect;
        this->agility += rhs.agility;
        this->armor += rhs.armor;
        this->resistance += rhs.resistance;

        return *this;
    }

    Stats& operator-=(const Stats& rhs) {
        /*  Ternary Operator: (condition) ? epression1 : expression2;
                if condition is true, execute expression1
                else, execute expression2
        */
        (this->strength < rhs.strength) ? this->strength = 0u : this->strength -= rhs.strength;
        (this->intellect < rhs.intellect) ? this->intellect = 0u : this->intellect -= rhs.intellect;
        (this->agility < rhs.agility) ? this->agility = 0u : this->agility -= rhs.agility;
        (this->armor < rhs.armor) ? this->armor = 0u : this->armor -= rhs.armor;
        (this->resistance < rhs.resistance) ? this->resistance = 0u : this->resistance -= rhs.resistance;

        return *this;
    }
};
