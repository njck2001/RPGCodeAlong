#include "PlayerCharacter.h"
#include "ItemManager.h"
#include <iostream>


/// TODO: Implement error messages and error handling
/// TODO: Create .cpp files for rest of .h files
/// TODO: Do another mass refactoring after completing the code-along series


int main() {
    PlayerCharacter p1(new Wizard());
    
    Item* plate_armor = ItemManager::make_armor("Shiny Plate Armor", Stats(0, 0, 0, 5, 3), ARMORSLOT::CHEST);
    Item* leather_armor = ItemManager::make_armor("Plain Leather Armor", Stats(0, 0, 0, 2, 1), ARMORSLOT::CHEST);
    Item* long_sword = ItemManager::make_weapon("Long Sword", Stats(), WEAPONSLOT::MELEE, 3, 9);
    Item* hand_axe = ItemManager::make_weapon("Rusty Hand Axe", Stats(), WEAPONSLOT::MELEE, 2, 4);
    Item* heal_potion = ItemManager::make_potion("Minor Healing", nullptr, 3, 3);

    p1.equip(leather_armor);
    p1.equip(long_sword);
    p1.pick_up(heal_potion);

    for (size_t i = 0; i < 8; i++) {
        std::cout
            << "Level " << p1.level() << ' ' << p1.class_name() << '\n'
            << " -EXP: " << p1.exp() << '/' << p1.etnl() << '\n'
            << " -HP: " << p1.hp() << '/' << p1.max_hp() << '\n'
            << " -MP: " << p1.mp() << '/' << p1.max_mp() << '\n'
            << " -Strength: " << p1.strength() << '\n'
            << " -Intellect: " << p1.intellect() << '\n'
            << " -Agility: " << p1.agility() << '\n'
            << " -Armor: " << p1.armor() << '\n'
            << " -Resistance: " << p1.resistance() << '\n';

        std::cout << "Buffs:\n";
        auto buffs = p1.buffs();
        for (auto& buff : buffs) {
        std::cout << " -" << buff.name << '\n';
        }

        std::cout << "Debuffs:\n";
        auto debuffs = p1.debuffs();
        for (auto& debuff : debuffs) {
        std::cout << " -" << debuff.name << '\n';
        }

        std::cout << "Abilities:\n";
        auto abilities = p1.abilities();
        for (auto& abil : abilities) {
            std::cout << " -" << abil.name << '\n';
        }

        std::cout << "Armor:\n";
        for (int i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
            const Armor* armor = dynamic_cast<Armor*>(p1.equipped_armor(i));

            if (armor) {
                std::cout << " -" << armor->name() << ": armor(" << armor->armor()
                            << ") resistance(" << armor->resistance() << ')' << std::endl;
            }
        }

        std::cout << "Weapons:\n";
        for (int i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
            Weapon* weapon = dynamic_cast<Weapon*>(p1.equipped_weapon(i));

            if (weapon) {
                std::cout << " -" << weapon->name() << ": damage(" << weapon->min_damage()
                            << '-' << weapon->max_damage() << ')' << std::endl;
            }
        }

        std::cout << "Inventory:\n";
        auto inv = p1.backpack();
        for (auto item : inv) {
            std::cout << " -" << *item << '\n';
        }

        std::cout << "--------------------" << std::endl;

        p1.gain_exp(100u);
        if (i == 0) {
            Buff armor_buff("Thick Skin", 10, Stats(0, 0, 0, 2, 2));
            p1.add_buff(armor_buff);
            p1.damage(5u);
            std::cout << "Damaged by 5\n" << std::endl;
            p1.equip(plate_armor);
            std::cout << "Equipped Shiny Plate Armor" << std::endl;
        }
        else if (i == 1) {
            Buff rubber_debuff("Rubber Legs", 5, Stats(5, 0, 5), true);
            p1.add_buff(rubber_debuff);
            p1.use(heal_potion);
            std::cout << "Used Minor Healing Potion\n" << std::endl;
            p1.equip(hand_axe);
            std::cout << "Equipped Rusty Hand Axe" << std::endl;
        }
        else if (i == 2) {
            p1.use(heal_potion);
            p1.use(heal_potion);
            std::cout << "Tried to use remaining Minor Healing Potions\n" << std::endl;
        }
        else if (i == 3) {
            p1.damage(1u);
            p1.use(heal_potion);
            std::cout << "Used remaining Minor Healing Potions\n" << std::endl;
        }
        else {
            p1.gain_exp(MAX_EXPTYPE);
        }
    }

    return 0;
}
