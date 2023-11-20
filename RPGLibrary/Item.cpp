#include "Item.h"


// Don't change this, only the EquipmentDelegate constuctor should use this
static std::uint32_t EQUIPMENT_ID_ITERATOR = 1u;

EquipmentDelegate::EquipmentDelegate(std::string name, Stats stats) :
    ItemDelegate(name), stats(stats), id(EQUIPMENT_ID_ITERATOR++) {
    /// TODO: Confirm that id == 1 for the first equipment created
    if (EQUIPMENT_ID_ITERATOR == 0u) {
        /// TODO: Throw error
        std::cerr << "\033[1;31m" << "Error:" << "\033[0m" << " max number of equipment has already been created" << std::endl;
    }
}
