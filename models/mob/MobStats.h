/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MODELS_MOB_STATS_H
#define TOS_MODELS_MOB_STATS_H

#include "../../stdlib/Types.h"

struct SMobStatsTotal {
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
    uint32 dmg;
    uint32 dmg_base;
    uint32 dmg_equip;
    uint32 dmg_item;
    uint32 dmg_effect;
    uint32 dmg_skill;

    uint32 dmg_pircing;
    uint32 dmg_pircing_base;
    uint32 dmg_pircing_equip;
    uint32 dmg_pircing_item;
    uint32 dmg_pircing_effect;
    uint32 dmg_pircing_skill;

    uint32 dmg_slashing;
    uint32 dmg_slashing_base;
    uint32 dmg_slashing_equip;
    uint32 dmg_slashing_item;
    uint32 dmg_slashing_effect;
    uint32 dmg_slashing_skill;

    uint32 dmg_bludgeoning;
    uint32 dmg_bludgeoning_base;
    uint32 dmg_bludgeoning_equip;
    uint32 dmg_bludgeoning_item;
    uint32 dmg_bludgeoning_effect;
    uint32 dmg_bludgeoning_skill;

    uint32 dmg_stabbing;
    uint32 dmg_stabbing_base;
    uint32 dmg_stabbing_equip;
    uint32 dmg_stabbing_item;
    uint32 dmg_stabbing_effect;
    uint32 dmg_stabbing_skill;

    uint32 dmg_fire;
    uint32 dmg_fire_base;
    uint32 dmg_fire_equip;
    uint32 dmg_fire_item;
    uint32 dmg_fire_effect;
    uint32 dmg_fire_skill;

    uint32 dmg_water;
    uint32 dmg_water_base;
    uint32 dmg_water_equip;
    uint32 dmg_water_item;
    uint32 dmg_water_effect;
    uint32 dmg_water_skill;

    uint32 dmg_ice;
    uint32 dmg_ice_base;
    uint32 dmg_ice_equip;
    uint32 dmg_ice_item;
    uint32 dmg_ice_effect;
    uint32 dmg_ice_skill;

    uint32 dmg_earth;
    uint32 dmg_earth_base;
    uint32 dmg_earth_equip;
    uint32 dmg_earth_item;
    uint32 dmg_earth_effect;
    uint32 dmg_earth_skill;

    uint32 dmg_wind;
    uint32 dmg_wind_base;
    uint32 dmg_wind_equip;
    uint32 dmg_wind_item;
    uint32 dmg_wind_effect;
    uint32 dmg_wind_skill;

    uint32 dmg_poison;
    uint32 dmg_poison_base;
    uint32 dmg_poison_equip;
    uint32 dmg_poison_item;
    uint32 dmg_poison_effect;
    uint32 dmg_poison_skill;

    uint32 dmg_lightning;
    uint32 dmg_lightning_base;
    uint32 dmg_lightning_equip;
    uint32 dmg_lightning_item;
    uint32 dmg_lightning_effect;
    uint32 dmg_lightning_skill;

    uint32 dmg_holy;
    uint32 dmg_holy_base;
    uint32 dmg_holy_equip;
    uint32 dmg_holy_item;
    uint32 dmg_holy_effect;
    uint32 dmg_holy_skill;

    uint32 dmg_arcane;
    uint32 dmg_arcane_base;
    uint32 dmg_arcane_equip;
    uint32 dmg_arcane_item;
    uint32 dmg_arcane_effect;
    uint32 dmg_arcane_skill;

    uint32 dmg_corrupted;
    uint32 dmg_corrupted_base;
    uint32 dmg_corrupted_equip;
    uint32 dmg_corrupted_item;
    uint32 dmg_corrupted_effect;
    uint32 dmg_corrupted_skill;

    uint32 dmg_crit;
    uint32 dmg_crit_base;
    uint32 dmg_crit_equip;
    uint32 dmg_crit_item;
    uint32 dmg_crit_effect;
    uint32 dmg_crit_skill;

    f32 dmg_crit_chance;
    f32 dmg_crit_chance_base;
    f32 dmg_crit_chance_equip;
    f32 dmg_crit_chance_item;
    f32 dmg_crit_chance_effect;
    f32 dmg_crit_chance_skill;

    uint32 dmg_reflection;
    uint32 dmg_reflection_base;
    uint32 dmg_reflection_equip;
    uint32 dmg_reflection_item;
    uint32 dmg_reflection_effect;
    uint32 dmg_reflection_skill;

    // Health & Resource
    uint32 health;
    uint32 health_base;
    uint32 health_equip;
    uint32 health_item;
    uint32 health_effect;
    uint32 health_skill;

    uint32 health_on_dmg_dealt;
    uint32 health_on_dmg_dealt_base;
    uint32 health_on_dmg_dealt_equip;
    uint32 health_on_dmg_dealt_item;
    uint32 health_on_dmg_dealt_effect;
    uint32 health_on_dmg_dealt_skill;

    uint32 health_on_dmg_taken;
    uint32 health_on_dmg_taken_base;
    uint32 health_on_dmg_taken_equip;
    uint32 health_on_dmg_taken_item;
    uint32 health_on_dmg_taken_effect;
    uint32 health_on_dmg_taken_skill;

    uint32 health_regen;
    uint32 health_regen_base;
    uint32 health_regen_equip;
    uint32 health_regen_item;
    uint32 health_regen_effect;
    uint32 health_regen_skill;

    uint32 health_regen_on_dmg_dealt;
    uint32 health_regen_on_dmg_dealt_base;
    uint32 health_regen_on_dmg_dealt_equip;
    uint32 health_regen_on_dmg_dealt_item;
    uint32 health_regen_on_dmg_dealt_effect;
    uint32 health_regen_on_dmg_dealt_skill;

    uint32 health_regen_on_dmg_taken;
    uint32 health_regen_on_dmg_taken_base;
    uint32 health_regen_on_dmg_taken_equip;
    uint32 health_regen_on_dmg_taken_item;
    uint32 health_regen_on_dmg_taken_effect;
    uint32 health_regen_on_dmg_taken_skill;

    uint32 resource;
    uint32 resource_base;
    uint32 resource_equip;
    uint32 resource_item;
    uint32 resource_effect;
    uint32 resource_skill;

    uint32 resource_on_dmg_dealt;
    uint32 resource_on_dmg_dealt_base;
    uint32 resource_on_dmg_dealt_equip;
    uint32 resource_on_dmg_dealt_item;
    uint32 resource_on_dmg_dealt_effect;
    uint32 resource_on_dmg_dealt_skill;

    uint32 resource_on_dmg_taken;
    uint32 resource_on_dmg_taken_base;
    uint32 resource_on_dmg_taken_equip;
    uint32 resource_on_dmg_taken_item;
    uint32 resource_on_dmg_taken_effect;
    uint32 resource_on_dmg_taken_skill;

    uint32 resource_regen;
    uint32 resource_regen_base;
    uint32 resource_regen_equip;
    uint32 resource_regen_item;
    uint32 resource_regen_effect;
    uint32 resource_regen_skill;

    uint32 resource_regen_on_dmg_dealt;
    uint32 resource_regen_on_dmg_dealt_base;
    uint32 resource_regen_on_dmg_dealt_equip;
    uint32 resource_regen_on_dmg_dealt_item;
    uint32 resource_regen_on_dmg_dealt_effect;
    uint32 resource_regen_on_dmg_dealt_skill;

    uint32 resource_regen_on_dmg_taken;
    uint32 resource_regen_on_dmg_taken_base;
    uint32 resource_regen_on_dmg_taken_equip;
    uint32 resource_regen_on_dmg_taken_item;
    uint32 resource_regen_on_dmg_taken_effect;
    uint32 resource_regen_on_dmg_taken_skill;

    uint32 resource_loss;
    uint32 resource_loss_base;
    uint32 resource_loss_equip;
    uint32 resource_loss_item;
    uint32 resource_loss_effect;
    uint32 resource_loss_skill;

    uint32 resource_loss_on_dmg_dealt;
    uint32 resource_loss_on_dmg_dealt_base;
    uint32 resource_loss_on_dmg_dealt_equip;
    uint32 resource_loss_on_dmg_dealt_item;
    uint32 resource_loss_on_dmg_dealt_effect;
    uint32 resource_loss_on_dmg_dealt_skill;

    uint32 resource_loss_on_dmg_taken;
    uint32 resource_loss_on_dmg_taken_base;
    uint32 resource_loss_on_dmg_taken_equip;
    uint32 resource_loss_on_dmg_taken_item;
    uint32 resource_loss_on_dmg_taken_effect;
    uint32 resource_loss_on_dmg_taken_skill;

    // Defense types
    //      think about it as armor and/or resistence if it helps
    uint32 defense;
    uint32 defense_base;
    uint32 defense_equip;
    uint32 defense_item;
    uint32 defense_effect;
    uint32 defense_skill;

    uint32 defense_pircing;
    uint32 defense_pircing_base;
    uint32 defense_pircing_equip;
    uint32 defense_pircing_item;
    uint32 defense_pircing_effect;
    uint32 defense_pircing_skill;

    uint32 defense_slashing;
    uint32 defense_slashing_base;
    uint32 defense_slashing_equip;
    uint32 defense_slashing_item;
    uint32 defense_slashing_effect;
    uint32 defense_slashing_skill;

    uint32 defense_bludgeoning;
    uint32 defense_bludgeoning_base;
    uint32 defense_bludgeoning_equip;
    uint32 defense_bludgeoning_item;
    uint32 defense_bludgeoning_effect;
    uint32 defense_bludgeoning_skill;

    uint32 defense_stabbing;
    uint32 defense_stabbing_base;
    uint32 defense_stabbing_equip;
    uint32 defense_stabbing_item;
    uint32 defense_stabbing_effect;
    uint32 defense_stabbing_skill;

    uint32 defense_fire;
    uint32 defense_fire_base;
    uint32 defense_fire_equip;
    uint32 defense_fire_item;
    uint32 defense_fire_effect;
    uint32 defense_fire_skill;

    uint32 defense_water;
    uint32 defense_water_base;
    uint32 defense_water_equip;
    uint32 defense_water_item;
    uint32 defense_water_effect;
    uint32 defense_water_skill;

    uint32 defense_ice;
    uint32 defense_ice_base;
    uint32 defense_ice_equip;
    uint32 defense_ice_item;
    uint32 defense_ice_effect;
    uint32 defense_ice_skill;

    uint32 defense_earth;
    uint32 defense_earth_base;
    uint32 defense_earth_equip;
    uint32 defense_earth_item;
    uint32 defense_earth_effect;
    uint32 defense_earth_skill;

    uint32 defense_wind;
    uint32 defense_wind_base;
    uint32 defense_wind_equip;
    uint32 defense_wind_item;
    uint32 defense_wind_effect;
    uint32 defense_wind_skill;

    uint32 defense_poison;
    uint32 defense_poison_base;
    uint32 defense_poison_equip;
    uint32 defense_poison_item;
    uint32 defense_poison_effect;
    uint32 defense_poison_skill;

    uint32 defense_lightning;
    uint32 defense_lightning_base;
    uint32 defense_lightning_equip;
    uint32 defense_lightning_item;
    uint32 defense_lightning_effect;
    uint32 defense_lightning_skill;

    uint32 defense_holy;
    uint32 defense_holy_base;
    uint32 defense_holy_equip;
    uint32 defense_holy_item;
    uint32 defense_holy_effect;
    uint32 defense_holy_skill;

    uint32 defense_arcane;
    uint32 defense_arcane_base;
    uint32 defense_arcane_equip;
    uint32 defense_arcane_item;
    uint32 defense_arcane_effect;
    uint32 defense_arcane_skill;

    uint32 defense_corrupted;
    uint32 defense_corrupted_base;
    uint32 defense_corrupted_equip;
    uint32 defense_corrupted_item;
    uint32 defense_corrupted_effect;
    uint32 defense_corrupted_skill;

    uint32 shield_type;
    bool shield_dispellable;

    uint32 shield;
    uint32 shield_base;
    uint32 shield_equip;
    uint32 shield_item;
    uint32 shield_effect;
    uint32 shield_skill;

    // Accuracy
    f32 dodge_chance;
    f32 dodge_chance_base;
    f32 dodge_chance_equip;
    f32 dodge_chance_item;
    f32 dodge_chance_effect;
    f32 dodge_chance_skill;

    f32 root_protection;
    f32 root_protection_base;
    f32 root_protection_equip;
    f32 root_protection_item;
    f32 root_protection_effect;
    f32 root_protection_skill;

    f32 miss_chance;
    f32 miss_chance_base;
    f32 miss_chance_equip;
    f32 miss_chance_item;
    f32 miss_chance_effect;
    f32 miss_chance_skill;

    // Movement
    // Additional speeds may be defined for Mobs
    f32 speed_walk1; // normal/fast
    f32 speed_walk1_base;
    f32 speed_walk1_equip;
    f32 speed_walk1_item;
    f32 speed_walk1_effect;
    f32 speed_walk1_skill;

    f32 speed_walk2; // casual/slow

    f32 speed_swim1; // normal/fast
    f32 speed_swim1_base;
    f32 speed_swim1_equip;
    f32 speed_swim1_item;
    f32 speed_swim1_effect;
    f32 speed_swim1_skill;

    f32 speed_swim2; // casual/slow

    f32 speed_fly1; // normal/fast
    f32 speed_fly1_base;
    f32 speed_fly1_equip;
    f32 speed_fly1_item;
    f32 speed_fly1_effect;
    f32 speed_fly1_skill;

    f32 speed_fly2; // casual/slow

    f32 speed_jump;
    f32 speed_dodge;
    f32 speed_turn;

    // Fighting speed
    f32 speed_cast;
    f32 speed_cast_base;
    f32 speed_cast_equip;
    f32 speed_cast_item;
    f32 speed_cast_effect;
    f32 speed_cast_skill;

    f32 speed_attack;
    f32 speed_attack_base;
    f32 speed_attack_equip;
    f32 speed_attack_item;
    f32 speed_attack_effect;
    f32 speed_attack_skill;
};

struct SMobStats {
    byte stat_str; // strength      : effects health + base damage
    byte stat_int; // inteligence   : effects resource + base demage
    byte stat_acc; // accuracy      : effects critical chance + base damage + miss chance
    byte stat_agi; // agility       : effects resource + base damage + dodge change
    byte stat_def; // defense       : effects resource + base defense + dodge change
    byte stat_sta; // stamina       : effects health regen + resource regen

    // Damage types
    uint32 dmg;
    uint32 dmg_pircing;
    uint32 dmg_slashing;
    uint32 dmg_bludgeoning;
    uint32 dmg_stabbing;
    uint32 dmg_fire;
    uint32 dmg_water;
    uint32 dmg_ice;
    uint32 dmg_arcane;
    uint32 dmg_corrupted;
    uint32 dmg_reflection;

    uint32 dmg_crit;
    f32 dmg_crit_chance;

    // Health & Resource
    uint32 health;
    uint32 health_on_dmg_dealt;
    uint32 health_on_dmg_taken;

    uint32 health_regen;
    uint32 health_regen_on_dmg_dealt;
    uint32 health_regen_on_dmg_taken;

    uint32 resource;
    uint32 resource_on_dmg_dealt;
    uint32 resource_on_dmg_taken;

    uint32 resource_regen;
    uint32 resource_regen_on_dmg_dealt;
    uint32 resource_regen_on_dmg_taken;

    uint32 resource_loss;
    uint32 resource_loss_on_dmg_dealt;
    uint32 resource_loss_on_dmg_taken;

    // Defense types
    //      think about it as armor and/or resistence if it helps
    uint32 defense;
    uint32 defense_pircing;
    uint32 defense_slashing;
    uint32 defense_bludgeoning;
    uint32 defense_stabbing;
    uint32 defense_fire;
    uint32 defense_water;
    uint32 defense_ice;
    uint32 defense_earth;
    uint32 defense_wind;
    uint32 defense_poison;
    uint32 defense_lightning;
    uint32 defense_holy;
    uint32 defense_arcane;
    uint32 defense_corrupted;

    uint32 shield_type;
    bool shield_dispellable;
    uint32 shield;

    // Accuracy
    f32 dodge_chance;
    f32 root_protection;
    f32 miss_chance;

    // Movement
    // Additional speeds may be defined for Mobs
    f32 speed_walk1; // normal/fast
    f32 speed_walk2; // casual/slow

    f32 speed_swim1; // normal/fast
    f32 speed_swim2; // casual/slow

    f32 speed_fly1; // normal/fast
    f32 speed_fly2; // casual/slow

    f32 speed_jump;
    f32 speed_dodge;
    f32 speed_turn;

    // Fighting speed
    f32 speed_cast;
    f32 speed_attack;
};

struct CMobStats {

};

#endif