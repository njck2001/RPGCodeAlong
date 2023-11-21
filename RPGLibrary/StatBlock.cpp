#include "StatBlock.h"


// Constructors
StatBlock::StatBlock() : base_stats(Stats()) {}
StatBlock::StatBlock(Stats stats) : base_stats(stats) {}

// Getters
stattype StatBlock::strength() const {
    if (total_buff_stats.strength < total_debuff_stats.strength) {
        stattype leftover_debuff_strength = total_debuff_stats.strength - total_buff_stats.strength;
        if (base_stats.strength < leftover_debuff_strength) {
            return 0u;
        }
        else {
            return base_stats.strength - leftover_debuff_strength;
        }
    }
    else {
        stattype leftover_buff_strength = total_buff_stats.strength - total_debuff_stats.strength;
        if (base_stats.strength > (MAX_STATTYPE - leftover_buff_strength)) {
            return MAX_STATTYPE;
        }
        else {
            return base_stats.strength + leftover_buff_strength;
        }
    }
}
stattype StatBlock::intellect() const {
    if (total_buff_stats.intellect < total_debuff_stats.intellect) {
        stattype leftover_debuff_intellect = total_debuff_stats.intellect - total_buff_stats.intellect;
        if (base_stats.intellect < leftover_debuff_intellect) {
            return 0u;
        }
        else {
            return base_stats.intellect - leftover_debuff_intellect;
        }
    }
    else {
        stattype leftover_buff_intellect = total_buff_stats.intellect - total_debuff_stats.intellect;
        if (base_stats.intellect > (MAX_STATTYPE - leftover_buff_intellect)) {
            return MAX_STATTYPE;
        }
        else {
            return base_stats.intellect + leftover_buff_intellect;
        }
    }
}
stattype StatBlock::agility() const {
    if (total_buff_stats.agility < total_debuff_stats.agility) {
        stattype leftover_debuff_agility = total_debuff_stats.agility - total_buff_stats.agility;
        if (base_stats.agility < leftover_debuff_agility) {
            return 0u;
        }
        else {
            return base_stats.agility - leftover_debuff_agility;
        }
    }
    else {
        stattype leftover_buff_agility = total_buff_stats.agility - total_debuff_stats.agility;
        if (base_stats.agility > (MAX_STATTYPE - leftover_buff_agility)) {
            return MAX_STATTYPE;
        }
        else {
            return base_stats.agility + leftover_buff_agility;
        }
    }
}
stattype StatBlock::armor() const {
    if (total_buff_stats.armor < total_debuff_stats.armor) {
        stattype leftover_debuff_armor = total_debuff_stats.armor - total_buff_stats.armor;
        if (base_stats.armor < leftover_debuff_armor) {
            return 0u;
        }
        else {
            return base_stats.armor - leftover_debuff_armor;
        }
    }
    else {
        stattype leftover_buff_armor = total_buff_stats.armor - total_debuff_stats.armor;
        if (base_stats.armor > (MAX_STATTYPE - leftover_buff_armor)) {
            return MAX_STATTYPE;
        }
        else {
            return base_stats.armor + leftover_buff_armor;
        }
    }
}
stattype StatBlock::resistance() const {
    if (total_buff_stats.resistance < total_debuff_stats.resistance) {
        stattype leftover_debuff_resistance = total_debuff_stats.resistance - total_buff_stats.resistance;
        if (base_stats.resistance < leftover_debuff_resistance) {
            return 0u;
        }
        else {
            return base_stats.resistance - leftover_debuff_resistance;
        }
    }
    else {
        stattype leftover_buff_resistance = total_buff_stats.resistance - total_debuff_stats.resistance;
        if (base_stats.resistance > (MAX_STATTYPE - leftover_buff_resistance)) {
            return MAX_STATTYPE;
        }
        else {
            return base_stats.resistance + leftover_buff_resistance;
        }
    }
}
const std::vector<Buff>& StatBlock::buffs() const { return buffs_; }
const std::vector<Buff>& StatBlock::debuffs() const { return debuffs_; }

// Setters
void StatBlock::add_buff(Buff buff) {
    if (buff.is_debuff) {
        for (auto& active_debuff : debuffs_) {
            if (buff.name == active_debuff.name) {
                if (active_debuff.duration < buff.duration) {
                    active_debuff.duration = buff.duration;
                }
                return;
            }
        }
        debuffs_.push_back(buff);
    }
    else {
        for (auto& active_buff : buffs_) {
            if (buff.name == active_buff.name) {
                if (active_buff.duration < buff.duration) {
                    active_buff.duration = buff.duration;
                }
                return;
            }
        }
        buffs_.push_back(buff);
    }
    recalculate_buffs();
}
void StatBlock::increase_stats(Stats amount) { base_stats += amount; }

// Helper Function
void StatBlock::recalculate_buffs() {
    total_buff_stats = Stats(); // All buff stats are set to 0
    total_debuff_stats = Stats(); // All debuff stats are set to 0
    for (auto buff : buffs_) {
        total_buff_stats += buff.stats;
    }
    for (auto debuff : debuffs_) {
        total_debuff_stats += debuff.stats;
    }
}
