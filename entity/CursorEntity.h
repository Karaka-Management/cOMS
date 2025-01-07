/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_ENTITY_CURSOR_H
#define TOS_ENTITY_CURSOR_H

#include "Entity.h"
#include "AnimationEntityComponent.h"

struct EntityCursor {
    Entity* general;
    AnimationEntityComponent* anim;
};

#endif