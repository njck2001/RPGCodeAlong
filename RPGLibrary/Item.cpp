#include "Item.h"



/// ITEM DELEGATE

// Constructor
ItemDelegate::ItemDelegate(std::string name) : name_(name) {}

// Getter
const std::string& ItemDelegate::name() const { return name_; } 



/// POTION

// Constructor
Potion::Potion(std::string name, Buff* buff, uint16_t quantity, welltype hp_heal,
welltype mp_heal, bool overheal) :
    ItemDelegate(name), buff_(buff), quantity_(quantity), hp_heal_(hp_heal),
    mp_heal_(mp_heal), overheal(overheal)
{}

// Deconstructor
Potion::~Potion() {
    if (buff_) {
        delete buff_;
        buff_ = nullptr;
    }
}

// Getters
const char* Potion::type() const { return "Potion"; }
const Buff* Potion::buff() const { return buff_; }
welltype Potion::hp_heal() const { return hp_heal_; }
welltype Potion::mp_heal() const { return mp_heal_; }
uint16_t Potion::quantity() const { return quantity_; }



/// EQUIPMENT DELEGATE

// Constructor
static std::uint32_t EQUIPMENT_ID_ITERATOR = 1u;
    // Don't change this, only the EquipmentDelegate constuctor should use this
EquipmentDelegate::EquipmentDelegate(std::string name, Stats stats) :
    ItemDelegate(name), stats(stats), id(EQUIPMENT_ID_ITERATOR++) {
    /// TODO: Confirm that id == 1 for the first equipment created
    if (EQUIPMENT_ID_ITERATOR == 0u) {
        /// TODO: Throw error
        std::cerr << "\033[1;31m" << "Error:" << "\033[0m" << " max number of equipment has already been created" << std::endl;
    }
}

// Getters
stattype EquipmentDelegate::strength() const { return stats.strength; }
stattype EquipmentDelegate::intellect() const { return stats.intellect; }
stattype EquipmentDelegate::agility() const { return stats.agility; }
stattype EquipmentDelegate::armor() const { return stats.armor; }
stattype EquipmentDelegate::resistance() const { return stats.resistance; }



/// ARMOR

// Constructor
Armor::Armor(std::string name, Stats stats, ARMORSLOT slot) : EquipmentDelegate(name, stats), slot_(slot) {}

// Getters
const char* Armor::type() const { return "Armor"; }
ARMORSLOT Armor::slot() const { return slot_; }



/// WEAPON

// Constructor
Weapon::Weapon(std::string name, Stats stats, WEAPONSLOT slot, stattype min_damage,
stattype max_damage, bool is_two_handed) :
    EquipmentDelegate(name, stats), slot_(slot), min_damage_(min_damage),
    max_damage_(max_damage), is_two_handed(is_two_handed)
{}

// Getters
const char* Weapon::type() const { return "Weapon"; }
WEAPONSLOT Weapon::slot() { return slot_; }
stattype Weapon::min_damage() { return min_damage_; }
stattype Weapon::max_damage() { return max_damage_; }



/// ITEM

// Constructor
Item::Item(ItemDelegate* item_data) : data_(item_data), marked_for_deletion(false) {}

// Overloaded Operator
std::ostream& operator<<(std::ostream& os, const Item& item) {
    Armor* armor = dynamic_cast<Armor*>(item.data_);
    if (armor) {
        return os << armor->name() << " (Armor: " << armor->armor() << ", Resist: " << armor->resistance() << ')';
    }
    Weapon* weapon = dynamic_cast<Weapon*>(item.data_);
    if (weapon) {
        return  os << weapon->name() << " (Damage: " << weapon->min_damage() << '-' << weapon->max_damage() << ')';
    }
    Potion* potion = dynamic_cast<Potion*>(item.data_);
    if (potion) {
        return os << potion->name() << " (Quantity: " << potion->quantity() << ')';
    }
    return os;
}
