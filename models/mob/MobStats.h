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

/**
 * @todo optimize order of struct members to ensure optimal struct size
 */

// Character stats modifiable through leveling (simple +/- buttons)
struct PrimaryStatsPoints {
    byte stat_str; // strength      : effects health + base damage
    byte stat_int; // inteligence   : effects resource + base demage
    byte stat_acc; // accuracy      : effects critical chance + base damage + miss chance
    byte stat_agi; // agility       : effects resource + base damage + dodge chance
    byte stat_def; // defense       : effects resource + base defense + dodge chance
    byte stat_sta; // stamina       : effects health regen + resource regen
};

struct PrimaryStatsRel {
    f32 stat_str;
    f32 stat_int;
    f32 stat_acc;
    f32 stat_agi;
    f32 stat_def;
    f32 stat_sta;
};

struct PrimaryStatsRelPoints {
    byte stat_str;
    byte stat_int;
    byte stat_acc;
    byte stat_agi;
    byte stat_def;
    byte stat_sta;
};

// Character stats modifiable thorugh skill tree?
struct SecondaryStatsPoints {
    // Damage types
    byte dmg_pircing;
    byte dmg_slashing;
    byte dmg_bludgeoning;
    byte dmg_stabbing;
    byte dmg_fire;
    byte dmg_water;
    byte dmg_wind;
    byte dmg_earth;
    byte dmg_poison;
    byte dmg_lightning;
    byte dmg_ice;
    byte dmg_arcane;
    byte dmg_corrupted;
    byte dmg_holy;
    byte dmg_reflection;
    byte dmg_reflection_chance;

    byte dmg_crit;
    byte dmg_crit_chance;

    // Health & Resource
    byte health;
    byte health_on_dmg_dealt;
    byte health_on_dmg_taken;

    byte health_regen;
    byte health_regen_rel;
    byte health_regen_on_dmg_dealt;
    byte health_regen_on_dmg_taken;

    byte resource;
    byte resource_on_dmg_dealt;
    byte resource_on_dmg_taken;

    byte resource_regen;
    byte resource_regen_rel;
    byte resource_regen_on_dmg_dealt;
    byte resource_regen_on_dmg_taken;

    byte resource_loss;
    byte resource_loss_on_dmg_dealt;
    byte resource_loss_on_dmg_taken;

    // Defense types
    //      think about it as armor and/or resistence if it helps
    byte defense_pircing;
    byte defense_slashing;
    byte defense_bludgeoning;
    byte defense_stabbing;
    byte defense_fire;
    byte defense_water;
    byte defense_ice;
    byte defense_earth;
    byte defense_wind;
    byte defense_poison;
    byte defense_lightning;
    byte defense_holy;
    byte defense_arcane;
    byte defense_corrupted;

    // Accuracy
    byte dodge_chance;
    byte cc_protection;
    byte miss_chance;

    // Movement
    // Additional speeds may be defined for Mobs
    byte speed_walk1;
    byte speed_swim1;
    byte speed_fly1;

    // Fighting speed
    byte speed_cast;
    byte speed_attack;

    byte pickup_range;

    byte shield;

    byte aoe_scale;
    byte resource_cost;
    byte health_cost;
    byte attack_range;
    byte melee_range;
    byte projectile_speed;
    byte projectile_count;
    byte shatter_probability;
    byte shatter_range;
    byte shatter_dmg;
    byte shatter_count;
    byte passthrough_damage;
    byte passthrough_count;
    byte dot_duration;
    byte dot_count;
    byte bleeding_dot;
    byte poison_dot;
    byte burn_dot;
    byte ice_dot;
    byte resource_drain;
    byte shatter_dot;
    byte minon_duration;
    byte minion_count;
    byte effect_spreading_probability;
    byte effect_spreading_radius;
    byte effect_spreading_max_count;
    byte effect_duration;
    byte aura_range;
    byte cast_duration;

    byte agro_range;
};


// @todo change order for simd calculations so that all valus match up
struct SecondaryStatsValues {
    // Damage types
    int32 dmg_pircing;
    int32 dmg_slashing;
    int32 dmg_bludgeoning;
    int32 dmg_stabbing;
    int32 dmg_fire;
    int32 dmg_water;
    int32 dmg_wind;
    int32 dmg_earth;
    int32 dmg_poison;
    int32 dmg_lightning;
    int32 dmg_ice;
    int32 dmg_arcane;
    int32 dmg_corrupted;
    int32 dmg_holy;
    int32 dmg_reflection;
    int32 dmg_reflection_chance;

    int32 dmg_crit;
    f32 dmg_crit_chance;

    // Health & Resource
    int32 health;
    f32 health_on_dmg_dealt;
    f32 health_on_dmg_taken;

    int32 health_regen;
    f32 health_regen_rel;
    f32 health_regen_on_dmg_dealt;
    f32 health_regen_on_dmg_taken;

    int32 resource;
    f32 resource_on_dmg_dealt;
    f32 resource_on_dmg_taken;

    int32 resource_regen;
    f32 resource_regen_rel;
    f32 resource_regen_on_dmg_dealt;
    f32 resource_regen_on_dmg_taken;

    int32 resource_loss;
    f32 resource_loss_on_dmg_dealt;
    f32 resource_loss_on_dmg_taken;

    // Defense types
    //      think about it as armor and/or resistence if it helps
    int32 defense_pircing;
    int32 defense_slashing;
    int32 defense_bludgeoning;
    int32 defense_stabbing;
    int32 defense_fire;
    int32 defense_water;
    int32 defense_ice;
    int32 defense_earth;
    int32 defense_wind;
    int32 defense_poison;
    int32 defense_lightning;
    int32 defense_holy;
    int32 defense_arcane;
    int32 defense_corrupted;

    // Accuracy
    f32 dodge_chance;
    f32 cc_protection;
    f32 miss_chance;

    // Movement
    // Additional speeds may be defined for Mobs
    f32 speed_walk1;
    f32 speed_swim1;
    f32 speed_fly1;

    // Fighting speed
    f32 speed_cast;
    f32 speed_attack;

    f32 pickup_range;

    int32 shield;

    f32 aoe_scale;
    f32 resource_cost;
    f32 health_cost;
    f32 attack_range;
    f32 melee_range;
    f32 projectile_speed;
    int32 projectile_count;
    f32 shatter_probability;
    f32 shatter_range;
    int32 shatter_dmg;
    int32 shatter_count;
    f32 passthrough_damage;
    int32 passthrough_count;
    f32 dot_duration;
    int32 dot_count;
    int32 bleeding_dot;
    int32 poison_dot;
    int32 burn_dot;
    int32 ice_dot;
    int32 resource_drain;
    int32 shatter_dot;
    f32 minon_duration;
    int32 minion_count;
    f32 effect_spreading_probability;
    f32 effect_spreading_radius;
    int32 effect_spreading_max_count;
    f32 effect_duration;
    f32 aura_range;
    f32 cast_duration;

    f32 agro_range;
};

struct SecondaryStatsRel {
    // Damage types
    f32 dmg_pircing;
    f32 dmg_slashing;
    f32 dmg_bludgeoning;
    f32 dmg_stabbing;
    f32 dmg_fire;
    f32 dmg_water;
    f32 dmg_wind;
    f32 dmg_earth;
    f32 dmg_poison;
    f32 dmg_lightning;
    f32 dmg_ice;
    f32 dmg_arcane;
    f32 dmg_corrupted;
    f32 dmg_holy;
    f32 dmg_reflection;
    f32 dmg_reflection_chance;

    f32 dmg_crit;
    f32 dmg_crit_chance;

    // Health & Resource
    f32 health;
    f32 health_on_dmg_dealt;
    f32 health_on_dmg_taken;

    f32 health_regen;
    f32 health_regen_on_dmg_dealt;
    f32 health_regen_on_dmg_taken;

    f32 resource;
    f32 resource_on_dmg_dealt;
    f32 resource_on_dmg_taken;

    f32 resource_regen;
    f32 resource_regen_on_dmg_dealt;
    f32 resource_regen_on_dmg_taken;

    f32 resource_loss;
    f32 resource_loss_on_dmg_dealt;
    f32 resource_loss_on_dmg_taken;

    // Defense types
    //      think about it as armor and/or resistence if it helps
    f32 defense_pircing;
    f32 defense_slashing;
    f32 defense_bludgeoning;
    f32 defense_stabbing;
    f32 defense_fire;
    f32 defense_water;
    f32 defense_ice;
    f32 defense_earth;
    f32 defense_wind;
    f32 defense_poison;
    f32 defense_lightning;
    f32 defense_holy;
    f32 defense_arcane;
    f32 defense_corrupted;

    // Accuracy
    f32 dodge_chance;
    f32 cc_protection;
    f32 miss_chance;

    // Movement
    // Additional speeds may be defined for Mobs
    f32 speed_walk1;
    f32 speed_swim1;
    f32 speed_fly1;

    // Fighting speed
    f32 speed_cast;
    f32 speed_attack;

    f32 pickup_range;

    f32 shield;

    f32 aoe_scale;
    f32 resource_cost;
    f32 health_cost;
    f32 attack_range;
    f32 melee_range;
    f32 projectile_speed;
    f32 projectile_count;
    f32 shatter_probability;
    f32 shatter_range;
    f32 shatter_dmg;
    f32 shatter_count;
    f32 passthrough_damage;
    f32 passthrough_count;
    f32 dot_duration;
    f32 dot_count;
    f32 bleeding_dot;
    f32 poison_dot;
    f32 burn_dot;
    f32 ice_dot;
    f32 resource_drain;
    f32 shatter_dot;
    f32 minon_duration;
    f32 minion_count;
    f32 effect_spreading_probability;
    f32 effect_spreading_radius;
    f32 effect_spreading_max_count;
    f32 effect_duration;
    f32 aura_range;
    f32 cast_duration;

    f32 agro_range;
};

struct SecondaryStatsRelPoints {
    // Damage types
    byte dmg_pircing;
    byte dmg_slashing;
    byte dmg_bludgeoning;
    byte dmg_stabbing;
    byte dmg_fire;
    byte dmg_water;
    byte dmg_wind;
    byte dmg_earth;
    byte dmg_poison;
    byte dmg_lightning;
    byte dmg_ice;
    byte dmg_arcane;
    byte dmg_corrupted;
    byte dmg_holy;
    byte dmg_reflection;
    byte dmg_reflection_chance;

    byte dmg_crit;
    byte dmg_crit_chance;

    // Health & Resource
    byte health;
    byte health_on_dmg_dealt;
    byte health_on_dmg_taken;

    byte health_regen;
    byte health_regen_on_dmg_dealt;
    byte health_regen_on_dmg_taken;

    byte resource;
    byte resource_on_dmg_dealt;
    byte resource_on_dmg_taken;

    byte resource_regen;
    byte resource_regen_on_dmg_dealt;
    byte resource_regen_on_dmg_taken;

    byte resource_loss;
    byte resource_loss_on_dmg_dealt;
    byte resource_loss_on_dmg_taken;

    // Defense types
    //      think about it as armor and/or resistence if it helps
    byte defense_pircing;
    byte defense_slashing;
    byte defense_bludgeoning;
    byte defense_stabbing;
    byte defense_fire;
    byte defense_water;
    byte defense_ice;
    byte defense_earth;
    byte defense_wind;
    byte defense_poison;
    byte defense_lightning;
    byte defense_holy;
    byte defense_arcane;
    byte defense_corrupted;

    // Accuracy
    byte dodge_chance;
    byte cc_protection;
    byte miss_chance;

    // Movement
    // Additional speeds may be defined for Mobs
    byte speed_walk1;
    byte speed_swim1;
    byte speed_fly1;

    // Fighting speed
    byte speed_cast;
    byte speed_attack;

    byte pickup_range;

    byte shield;

    byte aoe_scale;
    byte resource_cost;
    byte health_cost;
    byte attack_range;
    byte melee_range;
    byte projectile_speed;
    byte projectile_count;
    byte shatter_probability;
    byte shatter_range;
    byte shatter_dmg;
    byte shatter_count;
    byte passthrough_damage;
    byte passthrough_count;
    byte dot_duration;
    byte dot_count;
    byte bleeding_dot;
    byte poison_dot;
    byte burn_dot;
    byte ice_dot;
    byte resource_drain;
    byte shatter_dot;
    byte minon_duration;
    byte minion_count;
    byte effect_spreading_probability;
    byte effect_spreading_radius;
    byte effect_spreading_max_count;
    byte effect_duration;
    byte aura_range;
    byte cast_duration;

    byte agro_range;
};

struct FixedStats {
    // Movement
    // Additional speeds may be defined for Mobs
    float speed_walk1;
    float speed_swim1;
    float speed_fly1;

    f32 speed_jump;
    f32 speed_dodge;
    f32 speed_turn;
};

// @question Do we even want this?
struct PlayerStats {
    f32 pickup_range;
};

struct SMobStatsTotal {
    PrimaryStatsPoints primary_total;
    SecondaryStatsValues secondary_total;

    FixedStats fixed_total;

    uint32 shield_type;
    uint32 shield;
    bool shield_dispellable;
};

struct SMobStatsTotalCached {
    PrimaryStatsPoints primary_total;
    PrimaryStatsPoints primary_char; // Only recalculated when char stats change
    PrimaryStatsPoints primary_skill; // Only recalculated when skill effect runs out
    PrimaryStatsPoints primary_item; // Only recalculated when item changes
    PrimaryStatsPoints primary_effect; // External e.g. from mob or ally

    SecondaryStatsValues secondary_total;
    SecondaryStatsValues secondary_char; // Only recalculated when char stats change
    SecondaryStatsValues secondary_skill; // Only recalculated when skill effect runs out
    SecondaryStatsValues secondary_item; // Only recalculated when item changes
    SecondaryStatsValues secondary_effect; // External e.g. from mob or ally
};

struct SMobStatsPoints {
    // Self stats
    PrimaryStatsPoints primary_stats;
    SecondaryStatsPoints secondary_stats; // @todo this is bad, a char doesn't have fire dmg but might have crit chance ... needs to split?

    // Item modifiers
    PrimaryStatsPoints item_primary_add;
    PrimaryStatsRelPoints item_primary_mul;

    SecondaryStatsPoints item_secondary_add;
    SecondaryStatsRelPoints item_secondary_mul;

    // Skill modifiers
    PrimaryStatsPoints skill_primary_add;
    PrimaryStatsRelPoints skill_primary_mul;

    SecondaryStatsPoints skill_secondary_add;
    SecondaryStatsRelPoints skill_secondary_mul;
};

#endif