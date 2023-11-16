#pragma once // Does the same thing as indef, define, and endif, but all in one line
#include "types.h"
#include <memory> // unique_ptr
    /// TODO: Remove memory include after removing HP class


class PointWell {
    // Member variables
    welltype points_;
    welltype max_points;

public:
    // Constructors
    PointWell() : max_points(0u), points_(0u) {}
    PointWell(welltype max) : max_points(max), points_(max) {}
    PointWell(welltype max, welltype points) : max_points(max), points_(points) {}

    // Getters
    welltype points() const { return points_; }
    welltype max() const { return max_points; }
    bool is_full() const { return points_ == max_points; }

    // Setters
    void set_max(welltype new_max) {
        max_points = new_max;
        if (points_ > max_points) {
            points_ = max_points;
        }
    }
    void increase(welltype amount, bool allow_overfill = false) {
        if (!allow_overfill && (amount > (max_points - points_))) {
            points_ = max_points;
        }
        else if (amount > (MAX_WELLTYPE - points_)) {
            points_ = MAX_WELLTYPE;
        }
        else {
            points_ += amount;
        }
    }
    void decrease(welltype amount) {
        if (amount > points_) {
            points_ = 0u;
        }
        else {
            points_ -= amount;
        }
    }
};


// I decided to make HP its own derived class of PointWell because the shield complicates things
class HP : public PointWell {
    std::unique_ptr<PointWell> shield_;
    /// TODO: Reimplement shield
    /*  I think I know what I want to do with the shield:
            Before, I claimed it was too similar to a buff. But after looking at the Buff
            class, I just realized that buffs can't even affect HP. So I'm thinking about
            making the shield a consumable instead. Characters no longer start out with a
            shield nor do they have a max shield. Instead, players will have to pick up shields
            that disappear once depleted. Essentially, it's a "buff" but for your health.
        When a shield is set to 0, it should become a nullptr
            Although we might remove the pointer altogether; see below
        I do not think shields should be healable. Keep it simple.
            In fact, I might change shield_ to just an unsigned int rather than a PointWell ptr
        Alternatively, remove the shield member altogether and just allow the HP class to
        increase its points above its max using a special increase function
            We might implement this special increase function in PointWell instead and just
            remove the HP class altogether. That way, we could do something similar with MP.
            Essentially, it would be equivalent to temporary hearts in Zelda BotW.
        We'll need to go through PlayerCharacter and main to fix any errors cause by these changes
    */

public:
    HP(welltype max_hp = 0u, welltype max_shield = 0u, welltype shield_points = 0u) :
        PointWell(max_hp, max_hp) {
        if (max_shield) {
            give_shield(max_shield, shield_points);
        }
        else {
            shield_ = nullptr;
        }
    }

    // Getters
    welltype shield() const {
        if (shield_) {
            return shield_->points();
        } else {
            return 0u;
        }
    }
    welltype shield_max() const {
        if (shield_) {
            return shield_->max();
        }
        return 0u;
    }

    // Setters
    void give_shield(welltype max_shield = 1u, welltype shield_points = 0u) {
        shield_ = std::make_unique<PointWell>(max_shield, shield_points);
    }
    bool set_shield_max(welltype new_max) { // Returns true if set successfully
        if (!shield_) {
            return false;
        }
        else {
            shield_->set_max(new_max);
            return true;
        }
    }
    bool increase_shield(welltype amount) { // Returns true if shield exists
        // In PlayerCharacter, this function is essentially changed to heal_shield
        // We write as increase_shield here to keep the name consistent with the rest of the HP class
        if (!shield_) {
            return false;
        }
        else {
            shield_->increase(amount);
            return true;
        }
    }
    void decrease(welltype damage) { // Overrides the decrease function derived from PointWell
        if (shield_) {
            
            // Case for when damage taken is more than both hp and shield combined
            if (damage > points() + shield_->points()) {
                shield_->decrease(shield_->points()); // Set shield to zero
                PointWell::decrease(points()); // Set health to zero
            }

            // Case for when damage taken is just greater than the shield
            else if (damage > shield_->points()) {
                damage -= shield_->points();
                shield_->decrease(shield_->points()); // Set shield to zero
                PointWell::decrease(damage);
            }

            // Case for when damage taken is less than shield
            else {
                shield_->decrease(damage);
            }
        }
        else {
            PointWell::decrease(damage);
        }
    }
};
