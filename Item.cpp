#include "Item.h"


// Don't change this, only the EquipmentDelegate constuctor should use this
static std::uint32_t EQUIPMENT_ID_ITERATOR = 0u;

EquipmentDelegate::EquipmentDelegate(std::string name, Stats stats) :
    ItemDelegate(name), stats_(stats), id(EQUIPMENT_ID_ITERATOR++)
{}
