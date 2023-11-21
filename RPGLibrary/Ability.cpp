#include "Ability.h"


// Constructor
Ability::Ability(std::string name, uint32_t cost, uint32_t cooldown, ABILITY_TARGET target,
uint32_t hp_effect, ABILITY_SCALAR scalar) :
    name(name), cost(cost), cooldown(cooldown), target(target),
    hp_effect(hp_effect), scalar(scalar)
{}