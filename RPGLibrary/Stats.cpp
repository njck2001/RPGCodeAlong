#include "Stats.h"


// Constructor
Stats::Stats(stattype strength, stattype intellect, stattype agility,
stattype armor, stattype resistance) :
    strength(strength), intellect(intellect), agility(agility),
    armor(armor), resistance(resistance)
{}

// Overloaded Operators
Stats& Stats::operator+=(const Stats& rhs) {
    /*  Ternary Operator: (condition) ? epression1 : expression2;
            if condition is true, execute expression1
            else, execute expression2
    */
    (this->strength > (MAX_STATTYPE - rhs.strength)) ? this->strength = MAX_STATTYPE : this->strength += rhs.strength;
    (this->intellect > (MAX_STATTYPE - rhs.intellect)) ? this->intellect = MAX_STATTYPE : this->intellect += rhs.intellect;
    (this->agility > (MAX_STATTYPE - rhs.agility)) ? this->agility = MAX_STATTYPE : this->agility += rhs.agility;
    (this->armor > (MAX_STATTYPE - rhs.armor)) ? this->armor = MAX_STATTYPE : this->armor += rhs.armor;
    (this->resistance > (MAX_STATTYPE - rhs.resistance)) ? this->resistance = MAX_STATTYPE : this->resistance += rhs.resistance;
    return *this;
}
Stats& Stats::operator-=(const Stats& rhs) {
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
