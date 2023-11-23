#include "PlayerCharacter.h"
#include "ItemManager.h"



/// PLAYER CHARACTER DELEGATE

static const exptype BASE_ETNL = 100u;

// Constructor
PlayerCharacterDelegate::PlayerCharacterDelegate() :
    StatBlock(),
    level_(1u), exp_(0u), etnl(BASE_ETNL),
    hp_(std::make_unique<PointWell>()),
    mp_(nullptr)
{}

// Getters
leveltype PlayerCharacterDelegate::level() const { return level_; }
exptype PlayerCharacterDelegate::exp() const { return exp_; }
exptype PlayerCharacterDelegate::exp_to_next_level() const { return etnl; }
const std::unique_ptr<PointWell>& PlayerCharacterDelegate::hp() const { return hp_; }
const std::unique_ptr<PointWell>& PlayerCharacterDelegate::mp() const { return mp_; }
const std::vector<Ability>& PlayerCharacterDelegate::abilities() const { return abilities_; }

// Setters
void PlayerCharacterDelegate::gain_exp(exptype gained_exp) {
    if (exp_ > (MAX_EXPTYPE - gained_exp)) {
        exp_ = MAX_EXPTYPE;
    } 
    else {
        exp_ += gained_exp;
    }
    while (check_if_leveled()) { // While-loop in case we level up multiple times
        level_up();
    }
}
void PlayerCharacterDelegate::make_mp_well(welltype max_points, welltype points) {
    mp_ = std::make_unique<PointWell>(max_points, points);
}
void PlayerCharacterDelegate::add_ability(std::string name, uint32_t cost, uint32_t cooldown, ABILITY_TARGET target,
uint32_t hp_effect, ABILITY_SCALAR scalar) {
    abilities_.emplace_back(name, cost, cooldown, target, hp_effect, scalar);
}

// Helper Function
bool PlayerCharacterDelegate::check_if_leveled() {
    bool etnl_grows_polynomially = false; // Else, etnl grows exponentially
    if (exp_ >= etnl) { 
        if (level_ < MAX_LEVELTYPE) {
            level_++;
            if (etnl_grows_polynomially) {
                if (BASE_ETNL > (MAX_EXPTYPE / (exptype)level_)) {
                    etnl = MAX_EXPTYPE;
                }
                else if (etnl > (MAX_EXPTYPE - (BASE_ETNL * (exptype)level_))) {
                    etnl = MAX_EXPTYPE;
                }
                else {
                    etnl += BASE_ETNL * (exptype)level_;
                }
            }
            else { // etnl grows exponentially
                static const exptype ETNL_SCALAR = 2u;
                /*  Declared here, because unlike BASE_ETNL, which is used in the constructor
                    if etnl_grows_polynomially is true, we do not want to declare this at all.
                    This is just a minor optimization that saves an insignificant amount of memory.
                */
                if (etnl > (MAX_EXPTYPE / ETNL_SCALAR)) {
                    etnl = MAX_EXPTYPE;
                }
                else {
                    etnl *= ETNL_SCALAR;
                }
            }
            return true;
        }
        exp_ = etnl; // EXP is upper-bounded by MAX_LEVELTYPE instead of MAX_EXPTYPE
    }
    return false;
}



/// PLAYER CHARACTER

// Constructor
PlayerCharacter::PlayerCharacter(PlayerCharacterDelegate* pc_class) : pc_class(pc_class) {
    int i;
    for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
        equipped_armor_[i] = nullptr;
    }
    for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
        equipped_weapons_[i] = nullptr;
    }
}

// Destructor
PlayerCharacter::~PlayerCharacter() {
    delete pc_class;
    pc_class = nullptr;
    int i;
    for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
        if (equipped_armor_[i]) {
            delete equipped_armor_[i];
            equipped_armor_[i] = nullptr;
        }
    }
    for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
        if (equipped_weapons_[i]) {
            delete equipped_weapons_[i];
            equipped_weapons_[i] = nullptr;
        }
    }
}

// Getters
std::string PlayerCharacter::class_name() const { return pc_class->class_name(); }
leveltype PlayerCharacter::level() const { return pc_class->level(); }
exptype PlayerCharacter::exp() const { return pc_class->exp(); }
exptype PlayerCharacter::etnl() const { return pc_class->exp_to_next_level(); }
welltype PlayerCharacter::hp() const { return pc_class->hp()->points(); }
welltype PlayerCharacter::max_hp() const { return pc_class->hp()->max(); }
welltype PlayerCharacter::mp() const {
    if (pc_class->mp()) {
        return pc_class->mp()->points();
    }
    else {
        return 0u;
    }
}
welltype PlayerCharacter::max_mp() const {
    if (pc_class->mp()) {
        return pc_class->mp()->max();
    }
    else {
        return 0u;
    }
}
stattype PlayerCharacter::strength() const {
    stattype total_strength = pc_class->strength();
    int i;
    for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
        if (equipped_armor_[i]) {
            Armor* armor = dynamic_cast<Armor*>(equipped_armor_[i]->data);
            if (total_strength > (MAX_STATTYPE - armor->strength())) {
                return MAX_STATTYPE;
            }
            else {
                total_strength += armor->strength();
            }
        }
    }
    for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
        if (equipped_weapons_[i]) {
            Weapon* weapon = dynamic_cast<Weapon*>(equipped_weapons_[i]->data);
            if (total_strength > (MAX_STATTYPE - weapon->strength())) {
                return MAX_STATTYPE;
            }
            else {
                total_strength += weapon->strength();
            }
        }
    }
    return total_strength;
}
stattype PlayerCharacter::intellect() const {
    stattype total_intellect = pc_class->intellect();
    int i;
    for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
        if (equipped_armor_[i]) {
            Armor* armor = dynamic_cast<Armor*>(equipped_armor_[i]->data);
            if (total_intellect > (MAX_STATTYPE - armor->intellect())) {
                return MAX_STATTYPE;
            }
            else {
                total_intellect += armor->intellect();
            }
        }
    }
    for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
        if (equipped_weapons_[i]) {
            Weapon* weapon = dynamic_cast<Weapon*>(equipped_weapons_[i]->data);
            if (total_intellect > (MAX_STATTYPE - weapon->intellect())) {
                return MAX_STATTYPE;
            }
            else {
                total_intellect += weapon->intellect();
            }
        }
    }
    return total_intellect;
}
stattype PlayerCharacter::agility() const {
    stattype total_agility = pc_class->agility();
    int i;
    for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
        if (equipped_armor_[i]) {
            Armor* armor = dynamic_cast<Armor*>(equipped_armor_[i]->data);
            if (total_agility > (MAX_STATTYPE - armor->agility())) {
                return MAX_STATTYPE;
            }
            else {
                total_agility += armor->agility();
            }
        }
    }
    for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
        if (equipped_weapons_[i]) {
            Weapon* weapon = dynamic_cast<Weapon*>(equipped_weapons_[i]->data);
            if (total_agility > (MAX_STATTYPE - weapon->agility())) {
                return MAX_STATTYPE;
            }
            else {
                total_agility += weapon->agility();
            }
        }
    }
    return total_agility;
}
stattype PlayerCharacter::armor() const {
    stattype total_armor = pc_class->armor();
    int i;
    for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
        if (equipped_armor_[i]) {
            Armor* armor = dynamic_cast<Armor*>(equipped_armor_[i]->data);
            if (total_armor > (MAX_STATTYPE - armor->armor())) {
                return MAX_STATTYPE;
            }
            else {
                total_armor += armor->armor();
            }
        }
    }
    for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
        if (equipped_weapons_[i]) {
            Weapon* weapon = dynamic_cast<Weapon*>(equipped_weapons_[i]->data);
            if (total_armor > (MAX_STATTYPE - weapon->armor())) {
                return MAX_STATTYPE;
            }
            else {
                total_armor += weapon->armor();
            }
        }
    }
    return total_armor;
}
stattype PlayerCharacter::resistance() const {
    stattype total_resistance = pc_class->resistance();
    int i;
    for (i = 0; i < (int)ARMORSLOT::NUM_SLOTS; i++) {
        if (equipped_armor_[i]) {
            Armor* armor = dynamic_cast<Armor*>(equipped_armor_[i]->data);
            if (total_resistance > (MAX_STATTYPE - armor->resistance())) {
                return MAX_STATTYPE;
            }
            else {
                total_resistance += armor->resistance();
            }
        }
    }
    for (i = 0; i < (int)WEAPONSLOT::NUM_SLOTS; i++) {
        if (equipped_weapons_[i]) {
            Weapon* weapon = dynamic_cast<Weapon*>(equipped_weapons_[i]->data);
            if (total_resistance > (MAX_STATTYPE - weapon->resistance())) {
                return MAX_STATTYPE;
            }
            else {
                total_resistance += weapon->resistance();
            }
        }
    }
    return total_resistance;
}
const std::vector<Ability> PlayerCharacter::abilities() const { return pc_class->abilities(); }
const std::vector<Buff>& PlayerCharacter::buffs() const { return pc_class->buffs(); }
const std::vector<Buff>& PlayerCharacter::debuffs() const { return pc_class->debuffs(); }
const std::vector<Item*> PlayerCharacter::backpack() const { return backpack_; }
Armor* PlayerCharacter::equipped_armor(ARMORSLOT slot) const { 
    if (!equipped_armor_[(int)slot]) {
        return nullptr;
    }
    else {
        return dynamic_cast<Armor*>(equipped_armor_[(int)slot]->data);
    }
}
Armor* PlayerCharacter::equipped_armor(int slot) const {
    if (!equipped_armor_[slot]) {
        return nullptr;
    }
    else {
        return dynamic_cast<Armor*>(equipped_armor_[slot]->data);
    }
}
Weapon* PlayerCharacter::equipped_weapon(WEAPONSLOT slot) const { 
    if (!equipped_weapons_[(int)slot]) {
        return nullptr;
    }
    else {
        return dynamic_cast<Weapon*>(equipped_weapons_[(int)slot]->data);
    }
}
Weapon* PlayerCharacter::equipped_weapon(int slot) const {
    if (!equipped_weapons_[slot]) {
        return nullptr;
    }
    else {
        return dynamic_cast<Weapon*>(equipped_weapons_[slot]->data);
    }
}

// Setters
void PlayerCharacter::gain_exp(exptype amount) { pc_class->gain_exp(amount); }
void PlayerCharacter::damage(welltype amount) { pc_class->hp()->decrease(amount); }
void PlayerCharacter::heal(welltype amount, bool allow_overheal) { pc_class->hp()->increase(amount, allow_overheal); }
void PlayerCharacter::heal_hp(welltype amount, bool allow_overheal) { pc_class->hp()->increase(amount, allow_overheal); }
bool PlayerCharacter::heal_mp(welltype amount, bool allow_overheal) {
    if (pc_class->mp()) {
        pc_class->mp()->increase(amount, allow_overheal);
        return true;
    }
    else {
        return false;
    }
}
void PlayerCharacter::add_buff(Buff buff) { pc_class->add_buff(buff); }
bool PlayerCharacter::equip(Item* item) { return ItemManager::equip(item, this); }
bool PlayerCharacter::use(Item* item) { return ItemManager::use(item, this); }
bool PlayerCharacter::pick_up(Item* item) { return ItemManager::move_to_backpack(item, this); }

// Helper Function
void PlayerCharacter::cleanup_backpack() {
    const auto to_remove = std::stable_partition(backpack_.begin(), backpack_.end(),
        [](const Item* item) -> bool { return !item->marked_for_deletion; }
    );
    std::for_each(to_remove, backpack_.end(), [](Item* item) { delete item; item = nullptr; });
    backpack_.erase(to_remove, backpack_.end());
}
