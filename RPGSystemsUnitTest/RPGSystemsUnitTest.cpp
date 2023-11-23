#include "pch.h"
#include "CppUnitTest.h"
#include "RPGLibrary/types.h"
#include "RPGLibrary/PlayerCharacter.h"
#include "RPGLibrary/Item.h"
#include "RPGLibrary/ItemManager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


/////////////////
/// TODO LIST ///
/////////////////

/// TODO: Test deleting an Item
	/*	If an item is in a PC's backpack, and the item is deleted,
		how does that affect the backpack? If we tried to iterate
		through all the items in the backpack, would we find something
		funny? How does it compare to calling cleanup backpack, like we
		should? Would it be better to have items track who has it (set to
		nullptr if no one has it) and then do something similar to
		cleanup_backpack() in Item's deconstructor?
	*/
/// TODO: Look at Tutorial's GitHub Repo and implement tests
/// TODO: Create .cpp and .h files for different character classes
	/// TODO: Add cpp and header files to bottom of README after doing so
/// TODO: Determine if overloaded operator should be declared inside the class
	// or outside the class, and then make the code consistent with whatever
	// option you have determined is best
/// TODO: Implement error messages and error handling
/// TODO: Do another mass refactoring after completing the code-along series
/// TODO: Change structs that probably should be classes into classes
	// Stats can stay a stuct
	// Ability should probably be a class
	// Buff should also probably be a class
/// TODO: Create very thorough tests in RPGSystemsUnitTest.cpp
	// Test edge cases like a character reaching MAX_LEVEL
/// TODO: Compare our GitHub Repo to Tutorial's GitHub Repo


namespace RPGSystemsUnitTest
{
	TEST_CLASS(RPGSystemsUnitTest)
	{
	public:
		
		TEST_METHOD(WarriorTest)
		{
			PlayerCharacter p1(new Warrior());
			Assert::AreEqual(p1.level(), (leveltype)1);
			Assert::AreEqual(p1.max_hp(), (welltype)18);

			p1.gain_exp(100u);
			Assert::AreEqual(p1.level(), (leveltype)2);
			Assert::AreEqual(p1.abilities().front().name, std::string("PowerAttack"));
			Assert::AreEqual(p1.max_hp(), (welltype)27);

			// Test reaching max level
			while (p1.level() < MAX_LEVELTYPE) {
				p1.gain_exp(MAX_EXPTYPE);
			}
			Assert::AreEqual(p1.level(), MAX_LEVELTYPE);
			p1.gain_exp(100);
			Assert::AreEqual(p1.level(), MAX_LEVELTYPE);
			/// TODO: Test if all stats have reached MAX_STATTYPE
		}

		TEST_METHOD(EquipTest) {
			PlayerCharacter rogue(new Rogue());

			// Method 1 for equipping items
			Assert::IsTrue(ItemManager::equip(
				ItemManager::make_weapon("Dagger", Stats(), WEAPONSLOT::MELEE, 1, 3, false),
				&rogue
			));
			// Method 2 for equipping items
			Item* crossbow = ItemManager::make_weapon("Crossbow", Stats(), WEAPONSLOT::RANGED, 1, 3, false);
			Assert::IsTrue(rogue.equip(crossbow));
			// Assert that both methods were successful
			Assert::AreEqual(rogue.equipped_weapon(WEAPONSLOT::MELEE)->name(), std::string("Dagger"));
			Assert::AreEqual(rogue.equipped_weapon(WEAPONSLOT::RANGED)->name(), std::string("Crossbow"));

			crossbow = nullptr;
			Assert::IsNotNull(rogue.equipped_weapon(WEAPONSLOT::RANGED));

			Assert::IsFalse(ItemManager::equip(
				ItemManager::make_potion("ArmorPot", new Buff("ArmorBuff", 5, Stats(0, 0, 0, 3, 0))),
				&rogue
			));
		}
	};
}
