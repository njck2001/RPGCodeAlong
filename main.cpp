#include "PlayerCharacter.h"
#include <iostream>

int main() {
    PlayerCharacter p1(new Warrior());
    p1.heal_shield(1u);

    std::cout << std::endl;
    for (size_t i = 0; i < 3; i++) {
        std::cout
            << "Level " << p1.level() << ' ' << p1.class_name() << '\n'
            << " -EXP: " << p1.exp() << '/' << p1.etnl() << '\n'
            << " -HP: " << p1.hp() << '/' << p1.max_hp() << '\n'
            << "  -Shield: " << p1.shield() << '/' << p1.max_shield() << '\n'
            << " -MP: " << p1.mp() << '/' << p1.max_mp() << '\n'
            << " -Strength: " << p1.strength() << '\n'
            << " -Intellect: " << p1.intellect() << '\n'
            << " -Agility: " << p1.agility() << '\n'
            << " -Armor: " << p1.armor() << '\n'
            << " -Resistance: " << p1.resistance() << '\n';

        std::cout << " -Abilities:\n";
        auto abilities = p1.abilities();
        for (auto& abil : abilities) {
            std::cout << "  -" << abil.name << '\n';
        }
        std::cout << "--------------------" << std::endl;

        p1.gain_exp(100u);
        if (i == 0) {
            Buff armor_buff("StoneShield", 10, 0, 0, 0, 5);
            p1.add_buff(armor_buff);
        }
        if (i == 1) {
            Buff armor_buff("RubberLegs", 5, 5, 0, 5, 0, 0, true);
            p1.add_buff(armor_buff);
        }
    }

    return 0;
}
