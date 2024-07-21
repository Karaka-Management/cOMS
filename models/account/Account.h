/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MODELS_ACCOUNT_H
#define TOS_MODELS_ACCOUNT_H

#include "../../stdlib/Types.h"

#ifndef MAX_CHAR_NAME_LENGTH
    #define MAX_CHAR_NAME_LENGTH 32
#endif

struct Account {
    uint64 id;
    char name[MAX_CHAR_NAME_LENGTH];

    // relative memory position
    int64 index;

    // @question Maybe add pointers to Player, PacketCache?
};

/**
 * Whenever a user connects to the game server we immediately need to allocate a fixed amount of data.
 * Instead of putting this data willy-nilly into memory we can put all the user/account data always at the
 * same offset respective to the memory area of that data type.
 *
 * e.g. this means that if the account with the id X is found at position 12 in memory (not in the hashmap)
 *      the player and packet cache are also all located at position 12 in memory in their respective buffers
 *      this means we only have to find the memory position ONCE and we know where all the other data is of that account
 *      this also means we don't have to chase too many pointers
 *
 * @performance It might be faster to make Player and PacketCache part of the Account?
 *      It really depends on how we perform the game loop
 *          Are we handling account by account OR (probably should be inside account for L1 cache)
 *          Are we handling one data type by data type (then this is correct)
 */
void account_init_game_connect(Account* accounts, Player* players, PacketCache* packet_cache)
{
    int64 index = -1;

    if (index < 0) {
        return;
    }

    // init account
    Account* temp_acc = accounts[index];
    temp_acc->index = index;

    // init player
    Player* temp_player = players[index];

    // init packet cache
    PacketCache* temp_packets = packet_cache[index];
}

void account_free_game_disconnect()
{

}

#endif