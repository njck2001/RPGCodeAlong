#include "PlayerCharacter.h"
#include "ItemManager.h"
#include <iostream>


/*  For the rest of this code along series, I intend to implement everything the same way
    as shown in the videos. I will make notes of what I would like to do differently through
    creating TODOs, and then work through these TODOs after finishing the series. This should
    save a lot more time and cause a lot less confusion. I will, however, continue to create
    and use my own variable names
*/

/// TODO: Restrict access to class members and methods and create getters and setters
/// TODO: Reorganize classes
/// TODO: Create .cpp files for rest of .h files
/// TODO: Refactor such that the code is consistent and easy to understand
/// TODO: Remove unnecessary includes
/// TODO: Implement error messages and error handling
/// TODO: Go through and edit/reformat comments to make them more clear and consistent

int main() {
    PlayerCharacter p1(new Warrior());
    p1.heal_shield(1u);
    
    Item* plate_armor = ItemManager::make_armor("Shiny Plate Armor", Stats(0, 0, 0, 5, 3), ARMORSLOT::CHEST);
    Item* leather_armor = ItemManager::make_armor("Plain Leather Armor", Stats(0, 0, 0, 2, 1), ARMORSLOT::CHEST);
    Item* long_sword = ItemManager::make_weapon("Long Sword", Stats(), WEAPONSLOT::MELEE, 3, 9);
    Item* hand_axe = ItemManager::make_weapon("Rusty Hand Axe", Stats(), WEAPONSLOT::MELEE, 2, 4);
    Item* heal_potion = ItemManager::make_potion("Minor Healing", 3u, 0u, nullptr, 3u);

    ItemManager::equip(leather_armor, &p1);
    ItemManager::equip(long_sword, &p1);
    ItemManager::move_to_backpack(heal_potion, &p1);

    for (size_t i = 0; i < 5; i++) {
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
            const Armor* armor = dynamic_cast<Armor*>(p1.equipped_armor(i));

            if (armor) {
                std::cout << "  -" << armor->name() << ": armor(" << armor->stats().armor
                            << ") resistance(" << armor->stats().resistance << ')' << std::endl;
            }
        }

        std::cout << " -Weapons:\n";
        for (int i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            Weapon* weapon = dynamic_cast<Weapon*>(p1.equipped_weapon(i));

            if (weapon) {
                std::cout << "  -" << weapon->name() << ": damage(" << weapon->min_damage()
                            << '-' << weapon->max_damage() << ')' << std::endl;
            }
        }

        std::cout << "Inventory: ";
        auto inv = p1.backpack();
        for (auto item : inv) {
            std::cout << *item << ", ";
        }
        std::cout << std::endl;

        std::cout << "--------------------" << std::endl;

        p1.gain_exp(100u);
        if (i == 0) {
            Buff armor_buff("Thick Skin", 10, Stats(0, 0, 0, 2, 2));
            p1.add_buff(armor_buff);
            p1.damage(5u);
            std::cout << "Damaged by 5\n" << std::endl;
            ItemManager::equip(plate_armor, &p1);
            std::cout << "Equipped Shiny Plate Armor" << std::endl;
        }
        if (i == 1) {
            Buff rubber_debuff("Rubber Legs", 5, Stats(5, 0, 5), true);
            p1.add_buff(rubber_debuff);
            ItemManager::use(heal_potion, &p1);
            std::cout << "Used Minor Healing Potion\n" << std::endl;
            ItemManager::equip(hand_axe, &p1);
            std::cout << "Equipped Rusty Hand Axe" << std::endl;
        }
        if (i == 2) {
            ItemManager::use(heal_potion, &p1);
            ItemManager::use(heal_potion, &p1);
            std::cout << "Tried to use remaining Minor Healing Potions\n" << std::endl;
        }
        if (i == 3) {
            p1.damage(1u);
            ItemManager::use(heal_potion, &p1);
            std::cout << "Used remaining Minor Healing Potions\n" << std::endl;
        }
    }

    return 0;
}
