/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MODELS_CHAT_TYPE_H
#define TOS_MODELS_CHAT_TYPE_H

enum ChatType {
    CHAT_TYPE_MIXED = 1,
    CHAT_TYPE_LOCAL = 2,
    CHAT_TYPE_GLOBAL = 4,
    CHAT_TYPE_PLAYER = 8,
    CHAT_TYPE_GROUP = 16,
    CHAT_TYPE_GUILD = 32,
    CHAT_TYPE_AUCTION_HOUSE = 64,
    CHAT_TYPE_TRADE = 128,
    CHAT_TYPE_PRIVATE = 256, // e.g. direct messages, also used in raid finders etc.
    CHAT_TYPE_FRIENDS = 512,
    CHAT_TYPE_CHAT_ROOM = 1024
};

#endif