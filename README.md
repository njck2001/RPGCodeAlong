*This program was created by following along with this YouTube series: [Demo RPG C++ Code-Along](https://www.youtube.com/watch?v=ssJde6l7mrM&list=PLalVdRk2RC6pqOVxRNj5Uui7FN4r-WorM)*<br>
*While this version includes my own features, implementations, and comments, credit for the overall idea and core design goes to Matt Early, the creator of the YouTube channel [Code, Tech, and Tutorials](https://www.youtube.com/@CodeTechandTutorials)*

# Demo RPG

Coding some basic rpg systems in C++ for practice and fun.

## Design Ideas

Some quick theoretical lists that we will iterate on.

### Character Classes

- Warrior: Highest HP
- Wizard: Highest Magic Defense
- Cleric: Best Support Abilities
- Rogue: Highest Physical Damage

### Leveling System

- Gain stats and abilities based on class and levels.

#### Stats

- Strength: Involved in Physical Calculations
- Intelligence: Involved in Magic Calculations
- Agility: Involved in Nimbleness Calculations
- Armor: Involved in Physical Reduction Calculations
- Resistance: Involved in Element Damage Reduction Calculations

#### Levels

- Level Trees

### Items

- Shop System
- Equipment
- Consumables

### Game Loop

- Battle System: Turn Based
- World Levels
- Different Monsters

## Educational Comments

Throughout the code are comments meant to help beginner C++ programmers learn C++ concepts, as well as assist in my own learning. The order in which files are meant to be viewed is as follows:
- [types.h](/RPGLibrary/include/RPGLibrary/types.h)
- [PointWell.h](/RPGLibrary/include/RPGLibrary/PointWell.h)
- [PointWell.cpp](/RPGLibrary/PointWell.cpp)
- [Stats.h](/RPGLibrary/include/RPGLibrary/Stats.h)
- [Stats.cpp](/RPGLibrary/Stats.cpp)
- [Buff.h](/RPGLibrary/include/RPGLibrary/Buff.h)
- [Buff.cpp](/RPGLibrary/Buff.cpp)
- [StatBlock.h](/RPGLibrary/include/RPGLibrary/StatBlock.h)
- [StatBlock.cpp](/RPGLibrary/StatBlock.cpp)
- [Ability.h](/RPGLibrary/include/RPGLibrary/Ability.h)
- [Ability.cpp](/RPGLibrary/Ability.cpp)
- [Item.h](/RPGLibrary/include/RPGLibrary/Item.h)
- [Item.cpp](/RPGLibrary/Item.cpp)
- [PlayerCharacter.h](/RPGLibrary/include/RPGLibrary/PlayerCharacter.h)
- [PlayerCharacter.cpp](/RPGLibrary/PlayerCharacter.cpp)
- [ItemManager.h](/RPGLibrary/include/RPGLibrary/ItemManager.h)
- [ItemManager.cpp](/RPGLibrary/ItemManager.cpp)