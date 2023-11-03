#pragma once // does the same thing as indef, define, and endif, but all in one line
#include "types.h"
#include <cstdint> // uint16_t
#include <memory> // unique_ptr


class PointWell {
    well_type points_; // well_type is basically the same as an unsigned int
    well_type max_points;

public:
    PointWell() : max_points(0), points_(0) {}
    PointWell(well_type max) : max_points(max), points_(max) {}
    PointWell(well_type max, well_type points) : max_points(max), points_(points) {}

    // returns true if set successfully
    bool set_max(well_type new_max) {
        if (new_max == 0)
            return false;

        max_points = new_max;

        if (points_ > max_points)
            points_ = max_points;

        return true;
    }

    void decrease(well_type amount) {
        if (amount > points_) {
            points_ = 0;
            return;
        }

        points_ -= amount;
    }

    void increase(well_type amount) {
        if (amount + points_ > max_points) {
            points_ = max_points;
            return;
        }

        points_ += amount;
    }

    well_type max() { return max_points; }
    well_type points() { return points_; }
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
    HP(well_type max_hp = 1, well_type max_shield = 0, well_type shield_points = 0) :
        PointWell(max_hp, max_hp)
    {
        if (max_shield) {
            give_shield(max_shield, shield_points);
        }
        else {
            shield_ = nullptr;
        }
    }

    void give_shield(well_type max_shield = 1, well_type shield_points = 0) {
        shield_ = std::make_unique<PointWell>(max_shield, shield_points);
    }

    // returns true if set successfully
    bool set_shield_max(well_type new_max) {
        if (!shield_) {
            return false;
        }
        return shield_->set_max(new_max);
    }

    // This function overrides the decrease function derived from the parent class PointWell
    void decrease(well_type damage) {

        if (shield_) {
            // Case for when damage taken is more than both hp and shield combined
            if (damage > points() + shield_->points()) {
                shield_->decrease(shield_->points()); // set shield to zero
                PointWell::decrease(points()); // set health to zero
                return;
            }

            // Case for when damage taken is just greater than the shield
            if (damage > shield_->points()) {
                damage -= shield_->points();
                shield_->decrease(shield_->points()); // set shield to zero
                PointWell::decrease(damage);
                return;
            }

            // Case for when damage taken is less than shield
            shield_->decrease(damage);
        }
        else {
            PointWell::decrease(damage);
        }
    }

    // In PlayerCharacter, this function is essentially changed to heal_shield()
    // We write as increase_shield here to keep the name consistent with the rest of the HP class
    bool increase_shield(well_type amount) {
        if (!shield_) {
            return false;
        }
        shield_->increase(amount);
        return true;
    }

    well_type shield() {
        if (shield_) {
            return shield_->points();
        }
        return 0u;
    }
    well_type shield_max() {
        if (shield_) {
            return shield_->max();
        }
        return 0u;
    }
};
