#pragma once
#include "types.h"


struct Stats {
    stattype strength; 
    stattype intellect;
    stattype agility;
    stattype armor;
        /// TODO: Change "armor" to "defense" to make it less confusing
    stattype resistance;

    Stats(stattype strength = 0u, stattype intellect = 0u, stattype agility = 0u,
        stattype armor = 0u, stattype resistance = 0u);

    // Overloaded operators
    Stats& operator+=(const Stats& rhs);
    Stats& operator-=(const Stats& rhs);
};
