#pragma once // Does the same thing as indef, define, and endif, but all in one line
#include "types.h"


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
