/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MODELS_BRACKET_MATCH_H
#define TOS_MODELS_BRACKET_MATCH_H

#include "BracketTeam.h"

struct BracketMatch {
    BracketTeam* teams[2];
    BracketTeam* winner;
    BracketTeam* loser;
};

#endif