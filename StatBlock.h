#pragma once
#include <cstdint>

typedef std::uint16_t    stat_type;

// For learning purposes, access has been organized from most restrictive to least restrictive
    // Conventional class formatting orders sections like so: public, protected, and private
    // Typically, I like to keep private members at the top, before the public section
// Note: Derived classes will inherit as private by default unless otherwise specified
class StatBlock {
private:
/*  Members and methods cannot be accessed by anyone (including derived classes) except by the base class
        Note: This is regardless of whether a derived class inherits as private, protected, or public
*/
    
    stat_type strength_; 
    stat_type intellect_;
    stat_type agility_;
    stat_type armor_;
    stat_type resistance_;

protected: 
/*  Members and methods can be accessed by the base class and derived classes
    If inherited as private, derived classes will inherit protected members and methods as private members and methods
        Note: This means that derived classes of derived classes cannot access these members and methods
    If inherited as protected or public, derived classes will inherit protected members and methods as protected members and methods
        Note: This means that derived classes of derived classes can access these members and methods
*/
    
    void increase_stats(stat_type strength = 0, stat_type intellect = 0, stat_type agility = 0,
    stat_type armor = 0, stat_type resistance = 0) {
        strength_ += strength;
        intellect_ += intellect;
        agility_ += agility;
        armor_ += armor;
        resistance_ += resistance;
    }

public: 
/*  Members and methods can be accessed by anyone from the base class, by the base class, and by inherited classes
        Note: "from" the base class, meaning you can access these when using a base class object
    If inherited as private, derived classes will inherit public members and methods as private members and methods
        Note: This means that these members and methods cannot be accessed using a derived class object that uses private inheritance,
            nor can they be accessed by derived classes of derived classes
    If inherited as protected, derived classes will inherit public members and methods as protected members and methods
        Note: This means that these members and methods cannot be accessed using a derived class object that uses protected inheritance,
            but they can be accessed by derived classes of derived classes
    If inherited as public, derived classes will inherit public members and methods as public members and methods
        Note: This means that these members can be accessed using a derived class object that uses public inheritance,
            as well as derived classes of derived classes
*/
    
    // "explicit" ensures that only stat_type (or std::uint16_t) variables are allowed to be passed into the constructor
    explicit StatBlock(stat_type strength = (stat_type)0u, stat_type intellect = (stat_type)0u,
    stat_type agility = (stat_type)0u, stat_type armor = (stat_type)0u, stat_type resistance = (stat_type)0u) :
        strength_(strength), intellect_(intellect), agility_(agility), armor_(armor), resistance_(resistance)
    {}

    stat_type strength() { return strength_; }
    stat_type intellect() { return intellect_; }
    stat_type agility() { return agility_; }
    stat_type armor() { return armor_; }
    stat_type resistance() { return resistance_; }
};