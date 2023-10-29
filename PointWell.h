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
    /// TODO: Figure out what to do with shield
        /*  I'm thinking about removing to because imo, it just acts more like a health Buff
            If we do remove it, we might as well remove the entire HP class and and just make HP
                a standard PointWell
            I was considering making special attacks like spells that can ignore shields
            Either way, I definitely think I want to remove heal_shield as well as removing the shield's max
            In fact, I don't think shield should be a PointWell nor something that characters start
                with that the max for increases as they level up
            I feel like it should instead be a consumable, and different shield consumables will give more shield than others
            Alternatively, keep shield as it is, but actually let characters start battles with full shield
            This might be OP at first but it'll make it such that once their shield is destroyed, they lose
                it for the rest of the battle
            I kinda prefer making it a consumable more
        */
    std::unique_ptr<PointWell> shield_;

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
