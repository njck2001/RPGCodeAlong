#include "PointWell.h"


// Constructors
PointWell::PointWell() : max_points(0u), points_(0u) {}
PointWell::PointWell(welltype max) : max_points(max), points_(max) {}
PointWell::PointWell(welltype max, welltype points) : max_points(max), points_(points) {}

// Getters
welltype PointWell::points() const { return points_; }
welltype PointWell::max() const { return max_points; }
bool PointWell::is_full() const { return points_ == max_points; }

// Setters
void PointWell::set_max(welltype new_max) {
    max_points = new_max;
    if (points_ > max_points) {
        points_ = max_points;
    }
}
void PointWell::increase(welltype amount, bool allow_overfill) {
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
void PointWell::decrease(welltype amount) {
    if (amount > points_) {
        points_ = 0u;
    }
    else {
        points_ -= amount;
    }
}
