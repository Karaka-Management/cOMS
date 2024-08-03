/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MODELS_EQUIPMENT_TYPE_H
#define TOS_MODELS_EQUIPMENT_TYPE_H

#include "../../stdlib/Types.h"

struct EquipmentType {
    byte id;
    byte slot;
    bool dual;
    bool throwing;
    bool projectile;
    bool damage;
    bool armor;
    bool supporting;
    bool beam;
};

#endif