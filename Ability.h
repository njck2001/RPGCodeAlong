#pragma once
#include <string>

enum class ABILITY_TARGET { SELF, ENEMY, ALLY, };
enum class ABILITY_SCALAR { NONE, STR, INT, AGI };

struct Ability {
    std::string name;
    uint32_t cost; // for now, assume mp if there is a cost
    uint32_t cooldown; // number of rounds that you have to wait before using again
    ABILITY_TARGET target;
    uint32_t hp_effect;
    ABILITY_SCALAR scalar;

    Ability() = delete; // Removes the default constructor so that it cannot be used
    Ability(std::string name, uint32_t cost, uint32_t cooldown, ABILITY_TARGET target,
    uint32_t hp_effect, ABILITY_SCALAR scalar = ABILITY_SCALAR::NONE) :
        name(name), cost(cost), cooldown(cooldown), target(target),
        hp_effect(hp_effect), scalar(scalar)
    {}
};
