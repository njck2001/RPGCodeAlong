#include "Buff.h"


// Constructor
Buff::Buff(std::string name, uint16_t duration, Stats stats, bool is_debuff) :
    name(name), duration(duration), stats(stats), is_debuff(is_debuff)
{}