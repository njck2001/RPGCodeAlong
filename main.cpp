#include "PlayerCharacter.h"
#include <iostream>


int main() {
    PlayerCharacter p1(new Warrior());
    p1.heal_shield(1u);
    
    {
        Armor* plate_armor = new Armor("Shiny Plate Armor", Stats(0, 0, 0, 5, 3), ARMORSLOT::CHEST);
        if (p1.equip(plate_armor)) {
            std::cout << "equip succeeded\n";
        }
        else {
            std::cout << "equip failed\n";
        }
    }

    {
        Armor* leather_helmet = new Armor("Leather Helmet", Stats(0, 0, 0, 1, 1), ARMORSLOT::HELMET);
        if (p1.equip(leather_helmet)) {
            std::cout << "equip succeeded\n";
        }
        else {
            std::cout << "equip failed\n";
        }
    }

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

        std::cout << " -Armor:\n";
        for (int i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            Armor* armor = dynamic_cast<Armor*>(p1.equipped_armor(i));

            if (armor) {
                std::cout << "  -" << armor->name() << ": armor(" << armor->stats().armor
                            << ") resistance(" << armor->stats().resistance << ')' << std::endl;
            }
        }

        std::cout << "--------------------" << std::endl;

        p1.gain_exp(100u);
        if (i == 0) {
            Buff armor_buff("Thick Skin", 10, Stats(0, 0, 0, 2, 2));
            p1.add_buff(armor_buff);
        }
        if (i == 1) {
            Buff rubber_debuff("Rubber Legs", 5, Stats(5, 0, 5), true);
            p1.add_buff(rubber_debuff);
        }
    }

    return 0;
}
