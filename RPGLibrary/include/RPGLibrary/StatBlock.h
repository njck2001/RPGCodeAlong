#pragma once
#include "Stats.h"
#include "Buff.h"
#include <vector>
#include <iostream>


/*  For educational purposes, this class contains comments for explaining access specifiers and class inheritance
    Depending on the IDE you are using, you may be able to minimize block comments to make the code easier to read
    Conventional class formatting orders access specifiers from least restictive to most restrictive (public, protected, private)
    Derived classes will inherit as private by default (e.g. Derived : Base) unless otherwise specified (e.g. Derived : public Base)
*/
class StatBlock {
    /*  Typically, I like to keep private members at the top, before access specifiers
            Note: Access is set to private by default in classes
        For the sake of clarity, I've refained from doing so here
        All private members and methods can be found underneath the private access specifier
    */

public: 
/*  Members and methods can be accessed by the base class, by base class objects (e.g. statblock_obj.strength()), and by derived classes
    If inherited as public, derived classes will inherit public members and methods as public members and methods
        Note: This means that these members can be accessed by derived class objects that use public inheritance
            as well as derived classes of derived classes
    If inherited as protected, derived classes will inherit public members and methods as protected members and methods
        Note: This means that these members and methods cannot be accessed by derived class objects that use protected inheritance,
            but they can still be accessed by derived classes of derived classes
    If inherited as private, derived classes will inherit public members and methods as private members and methods
        Note: This means that these members and methods cannot be accessed by derived class objects that use private inheritance,
            nor can they be accessed by derived classes of derived classes
*/
    
    StatBlock();
    StatBlock(Stats stats);

    stattype strength() const;
    stattype intellect() const;
    stattype agility() const;
    stattype armor() const;
    stattype resistance() const;
    const std::vector<Buff>& buffs() const;
    const std::vector<Buff>& debuffs() const;
        // See Item.h to learn the difference between the first and second consts

    void add_buff(Buff buff);

protected: 
/*  Members and methods can be accessed by the base class and derived classes
    If inherited as public or protected, derived classes will inherit protected members and methods as protected members and methods
        Note: This means that derived classes of derived classes can also access these members and methods
    If inherited as private, derived classes will inherit protected members and methods as private members and methods
        Note: This means that derived classes of derived classes cannot access these members and methods
*/

    // Protected Setter - only used by derived classes
    void increase_stats(Stats amount);

private:
/*  Members and methods cannot be accessed by anyone (including derived classes) except by the base class
        Note: This is regardless of whether a derived class inherits as private, protected, or public
*/
    
    // Normally, I would have put these member variables at the top of the class
    Stats base_stats;
    std::vector<Buff> buffs_;
    Stats total_buff_stats;
    std::vector<Buff> debuffs_;
    Stats total_debuff_stats;

    // Helper Function (helper functions are typically always private)
    void recalculate_buffs();
};
