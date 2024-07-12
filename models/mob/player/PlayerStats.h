/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MODELS_MOB_PLAYER_STATS_H
#define TOS_MODELS_MOB_PLAYER_STATS_H

#include "../../../stdlib/Types.h"

#if SERVER
    struct SPlayerStats {
        byte stat_str; // strength      : effects health + base damage
        byte stat_int; // inteligence   : effects resource + base demage
        byte stat_acc; // accuracy      : effects critical chance + base damage + miss chance
        byte stat_agi; // agility       : effects resource + base damage + dodge change
        byte stat_def; // defense       : effects resource + base defense + dodge change
        byte stat_sta; // stamina       : effects health regen + resource regen

        // Naming conventions
        //              : total
        //      _base   : character stats (see stat_ above)
        //      _equip  : character equipment
        //      _item   : other item effects (consumables, e.g. potions)
        //      _effect : external aoe/skill effect
        //      _skill  : own skill

        // Damage types
        unsigned int dmg;
        unsigned int dmg_base;
        unsigned int dmg_equip;
        unsigned int dmg_item;
        unsigned int dmg_effect;
        unsigned int dmg_skill;

        unsigned int dmg_pircing;
        unsigned int dmg_pircing_base;
        unsigned int dmg_pircing_equip;
        unsigned int dmg_pircing_item;
        unsigned int dmg_pircing_effect;
        unsigned int dmg_pircing_skill;

        unsigned int dmg_slashing;
        unsigned int dmg_slashing_base;
        unsigned int dmg_slashing_equip;
        unsigned int dmg_slashing_item;
        unsigned int dmg_slashing_effect;
        unsigned int dmg_slashing_skill;

        unsigned int dmg_bludgeoning;
        unsigned int dmg_bludgeoning_base;
        unsigned int dmg_bludgeoning_equip;
        unsigned int dmg_bludgeoning_item;
        unsigned int dmg_bludgeoning_effect;
        unsigned int dmg_bludgeoning_skill;

        unsigned int dmg_fire;
        unsigned int dmg_fire_base;
        unsigned int dmg_fire_equip;
        unsigned int dmg_fire_item;
        unsigned int dmg_fire_effect;
        unsigned int dmg_fire_skill;

        unsigned int dmg_water;
        unsigned int dmg_water_base;
        unsigned int dmg_water_equip;
        unsigned int dmg_water_item;
        unsigned int dmg_water_effect;
        unsigned int dmg_water_skill;

        unsigned int dmg_ice;
        unsigned int dmg_ice_base;
        unsigned int dmg_ice_equip;
        unsigned int dmg_ice_item;
        unsigned int dmg_ice_effect;
        unsigned int dmg_ice_skill;

        unsigned int dmg_earth;
        unsigned int dmg_earth_base;
        unsigned int dmg_earth_equip;
        unsigned int dmg_earth_item;
        unsigned int dmg_earth_effect;
        unsigned int dmg_earth_skill;

        unsigned int dmg_wind;
        unsigned int dmg_wind_base;
        unsigned int dmg_wind_equip;
        unsigned int dmg_wind_item;
        unsigned int dmg_wind_effect;
        unsigned int dmg_wind_skill;

        unsigned int dmg_poison;
        unsigned int dmg_poison_base;
        unsigned int dmg_poison_equip;
        unsigned int dmg_poison_item;
        unsigned int dmg_poison_effect;
        unsigned int dmg_poison_skill;

        unsigned int dmg_lightning;
        unsigned int dmg_lightning_base;
        unsigned int dmg_lightning_equip;
        unsigned int dmg_lightning_item;
        unsigned int dmg_lightning_effect;
        unsigned int dmg_lightning_skill;

        unsigned int dmg_holy;
        unsigned int dmg_holy_base;
        unsigned int dmg_holy_equip;
        unsigned int dmg_holy_item;
        unsigned int dmg_holy_effect;
        unsigned int dmg_holy_skill;

        unsigned int dmg_arcane;
        unsigned int dmg_arcane_base;
        unsigned int dmg_arcane_equip;
        unsigned int dmg_arcane_item;
        unsigned int dmg_arcane_effect;
        unsigned int dmg_arcane_skill;

        unsigned int dmg_corrupted;
        unsigned int dmg_corrupted_base;
        unsigned int dmg_corrupted_equip;
        unsigned int dmg_corrupted_item;
        unsigned int dmg_corrupted_effect;
        unsigned int dmg_corrupted_skill;

        unsigned int dmg_crit;
        unsigned int dmg_crit_base;
        unsigned int dmg_crit_equip;
        unsigned int dmg_crit_item;
        unsigned int dmg_crit_effect;
        unsigned int dmg_crit_skill;

        float dmg_crit_chance;
        float dmg_crit_chance_base;
        float dmg_crit_chance_equip;
        float dmg_crit_chance_item;
        float dmg_crit_chance_effect;
        float dmg_crit_chance_skill;

        // Health & Resource
        unsigned int health;
        unsigned int health_base;
        unsigned int health_equip;
        unsigned int health_item;
        unsigned int health_effect;
        unsigned int health_skill;

        unsigned int health_regen;
        unsigned int health_regen_base;
        unsigned int health_regen_equip;
        unsigned int health_regen_item;
        unsigned int health_regen_effect;
        unsigned int health_regen_skill;

        unsigned int resource;
        unsigned int resource_base;
        unsigned int resource_equip;
        unsigned int resource_item;
        unsigned int resource_effect;
        unsigned int resource_skill;

        unsigned int resource_regen;
        unsigned int resource_regen_base;
        unsigned int resource_regen_equip;
        unsigned int resource_regen_item;
        unsigned int resource_regen_effect;
        unsigned int resource_regen_skill;

        // Defense types
        //      think about it as armor and/or resistence if it helps
        unsigned int defense;
        unsigned int defense_base;
        unsigned int defense_equip;
        unsigned int defense_item;
        unsigned int defense_effect;
        unsigned int defense_skill;

        unsigned int defense_pircing;
        unsigned int defense_pircing_base;
        unsigned int defense_pircing_equip;
        unsigned int defense_pircing_item;
        unsigned int defense_pircing_effect;
        unsigned int defense_pircing_skill;

        unsigned int defense_slashing;
        unsigned int defense_slashing_base;
        unsigned int defense_slashing_equip;
        unsigned int defense_slashing_item;
        unsigned int defense_slashing_effect;
        unsigned int defense_slashing_skill;

        unsigned int defense_bludgeoning;
        unsigned int defense_bludgeoning_base;
        unsigned int defense_bludgeoning_equip;
        unsigned int defense_bludgeoning_item;
        unsigned int defense_bludgeoning_effect;
        unsigned int defense_bludgeoning_skill;

        unsigned int defense_fire;
        unsigned int defense_fire_base;
        unsigned int defense_fire_equip;
        unsigned int defense_fire_item;
        unsigned int defense_fire_effect;
        unsigned int defense_fire_skill;

        unsigned int defense_water;
        unsigned int defense_water_base;
        unsigned int defense_water_equip;
        unsigned int defense_water_item;
        unsigned int defense_water_effect;
        unsigned int defense_water_skill;

        unsigned int defense_ice;
        unsigned int defense_ice_base;
        unsigned int defense_ice_equip;
        unsigned int defense_ice_item;
        unsigned int defense_ice_effect;
        unsigned int defense_ice_skill;

        unsigned int defense_earth;
        unsigned int defense_earth_base;
        unsigned int defense_earth_equip;
        unsigned int defense_earth_item;
        unsigned int defense_earth_effect;
        unsigned int defense_earth_skill;

        unsigned int defense_wind;
        unsigned int defense_wind_base;
        unsigned int defense_wind_equip;
        unsigned int defense_wind_item;
        unsigned int defense_wind_effect;
        unsigned int defense_wind_skill;

        unsigned int defense_poison;
        unsigned int defense_poison_base;
        unsigned int defense_poison_equip;
        unsigned int defense_poison_item;
        unsigned int defense_poison_effect;
        unsigned int defense_poison_skill;

        unsigned int defense_lightning;
        unsigned int defense_lightning_base;
        unsigned int defense_lightning_equip;
        unsigned int defense_lightning_item;
        unsigned int defense_lightning_effect;
        unsigned int defense_lightning_skill;

        unsigned int defense_holy;
        unsigned int defense_holy_base;
        unsigned int defense_holy_equip;
        unsigned int defense_holy_item;
        unsigned int defense_holy_effect;
        unsigned int defense_holy_skill;

        unsigned int defense_arcane;
        unsigned int defense_arcane_base;
        unsigned int defense_arcane_equip;
        unsigned int defense_arcane_item;
        unsigned int defense_arcane_effect;
        unsigned int defense_arcane_skill;

        unsigned int defense_corrupted;
        unsigned int defense_corrupted_base;
        unsigned int defense_corrupted_equip;
        unsigned int defense_corrupted_item;
        unsigned int defense_corrupted_effect;
        unsigned int defense_corrupted_skill;

        // Accuracy
        float dodge_chance;
        float dodge_chance_base;
        float dodge_chance_equip;
        float dodge_chance_item;
        float dodge_chance_effect;
        float dodge_chance_skill;

        float root_protection;
        float root_protection_base;
        float root_protection_equip;
        float root_protection_item;
        float root_protection_effect;
        float root_protection_skill;

        float miss_chance;
        float miss_chance_base;
        float miss_chance_equip;
        float miss_chance_item;
        float miss_chance_effect;
        float miss_chance_skill;

        // Movement
        // Additional speeds may be defined for players
        float speed_walk1; // normal/fast
        float speed_walk1_base;
        float speed_walk1_equip;
        float speed_walk1_item;
        float speed_walk1_effect;
        float speed_walk1_skill;

        float speed_walk2; // casual/slow

        float speed_swim1; // normal/fast
        float speed_swim1_base;
        float speed_swim1_equip;
        float speed_swim1_item;
        float speed_swim1_effect;
        float speed_swim1_skill;

        float speed_swim2; // casual/slow

        float speed_fly1; // normal/fast
        float speed_fly1_base;
        float speed_fly1_equip;
        float speed_fly1_item;
        float speed_fly1_effect;
        float speed_fly1_skill;

        float speed_fly2; // casual/slow

        float speed_jump;
        float speed_dodge;
        float speed_turn;

        // Fighting speed
        float speed_cast;
        float speed_cast_base;
        float speed_cast_equip;
        float speed_cast_item;
        float speed_cast_effect;
        float speed_cast_skill;

        float speed_attack;
        float speed_attack_base;
        float speed_attack_equip;
        float speed_attack_item;
        float speed_attack_effect;
        float speed_attack_skill;
    };
#endif

struct CPlayerStats {

};

#endif