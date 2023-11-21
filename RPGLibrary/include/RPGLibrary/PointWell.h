#pragma once // Does the same thing as indef, define, and endif, but all in one line
#include "types.h"


class PointWell {
    // Member variables
    welltype points_;
    welltype max_points;

public:
    // Constructors
    PointWell();
    PointWell(welltype max);
    PointWell(welltype max, welltype points);

    // Getters
    welltype points() const;
    welltype max() const;
    bool is_full() const;

    // Setters
    void set_max(welltype new_max);
    void increase(welltype amount, bool allow_overfill = false);
    void decrease(welltype amount);
};
