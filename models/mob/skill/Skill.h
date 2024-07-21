#ifndef TOS_MODELS_SKILL_H
#define TOS_MODELS_SKILL_H

#include "../../../stdlib/Types.h"
#include "ProjectileDistribution.h"
#include "SkillLocation.h"
#include "AoeDistribution.h"
#include "StatsTarget.h"
#include "../MobStats.h"

#define MAX_SKILL_NAME 32
#define MAX_SKILL_DESCRIPTION 128

struct Skill
{
    // not required for server
    // const char name[MAX_SKILL_NAME];
    // const char description[MAX_SKILL_DESCRIPTION];

    int id;

    // animations
    void* animation_casting;
    void* animation_channeling;

    bool projectile_animation_uses_item; // Use the equiped item for the projectile

    byte target; // what is targatable (self, enemies, allies, ground, none?) Has to be bitset 2^n to define multiple
    byte default_target;

    f32 visual_scale; // make visual larger/smaller

    // @tood how to handle multiplicative stats?
    // you can have 2 stats for 2 target types (e.g. you could create a buff and debuff in one skill)
    SMobStats stats1;
    StatsTarget stats1_target;

    SMobStats stats2;
    StatsTarget stats2_target;

    byte skill_movement; // none, follows target, random moevement, random movement in aoe

    byte count; // how often is the skill executed @question can this also be used as tick count for heals? or is this in the MobStats?
    byte count_delay; // delay between executions
    byte count_manual; // you can manually perform the skill count * times
    // @todo how to implement random distribution e.g. place n totems in random distribution close to selected spot

    // General
    int resource_cost;
    int health_cost;

    bool is_range;
    f32 attack_range;
    void* attack_anim;

    bool is_melee;
    f32 melee_range;

    // Projectile data
    bool is_projectile;
    f32 projectile_speed;
    int projectile_count;
    int projectile_shape; // 3d Model

    int projectile_distribution;
    bool projectile_angle; // Projectiles fan out from start point

    // Either creates new sub-projectiles on shatter OR
    // shatter is basically a second hit
    bool shatter;
    int shatter_projectiles;
    int shatter_count; // How often can it shatter at most
    f32 shatter_probability;
    f32 shatter_range;
    f32 shatter_duration;
    uint32 shatter_damage;

    bool is_retargatable; // If multiple you may be able to switch targets in between
    bool is_boomerang;

    // Casting happens before the skill
    f32 cast_duration;
    bool is_cast_cancalable;
    bool is_cast_disruptable;
    bool is_cast_movable;

    // Channeling happens during the skill.
    // even a normal punch attack is a channelled skill
    f32 channeling_duration; // -1.0 = infinite (e.g. aura), 0 = no duration = instant
    bool is_channeling; // duration = skill_duration + it shows the channeling timeline
    bool is_channeling_cancalable;
    bool is_channeling_disruptable;
    bool is_channeling_movable;

    byte channeling_distribution; // beginning, on hit, end, intervals, smooth -> also effects dmg if channeling is cancled
    byte channeling_ticks; // how often is dmg dealt

    f32 passthrough_damage; // How much of your damage in % is passing through to the next mob.
    int passtrhough_count;

    // AOE
    bool is_aoe;
    f32 aoe_distance;
    int aoe_shape; // circle, square, donut
    AoeDistribution aoe_fill_pattern;
    f32 aoe_dim1;
    f32 aoe_dim2;
    f32 aoe_dim3;
    int aoe_shatter;
    SkillLocation aoe_location;
    byte aoe_apply; // Applies while in aoe, applies even after moving out of aoe

    // DOT
    f32 dot_duration;
    int dot_count;
    byte dot_state; // only when moving, standing still, always

    byte dot_effective; // always; on move; on stand still

    byte dot_buff_effective; // buff modifier (e.g. increase by/decrease by)
    float dot_buff; // buff dot when on move or on standstill

    int bleeding_dot;
    int poison_dot;
    int burn_dot;
    int ice_dot;
    int resource_drain;
    int dot_shatter;

    // Minion (and totems)
    int minion_type;
    int minon_count; // Max minion count
    int minion_duration;
    int minion_summon_count; // How many summons per summon cast @question still required with general skill count? I don't think so.

    // Effects
    bool is_effect_spreading;
    f32 effect_spreading_probability;
    f32 effect_spreading_radius; // What is the maximum distance for spreading
    f32 effect_spreading_max_count; // How many mobs can be effected at maximum
    f32 effect_duration;

    // Aura
    f32 aura_range;

    // Push/pull
    byte push_pull_distance;
    byte push_pull_direction; // circle, pov = fan like; > 0 = push, < 0 = pull

    byte player_visibility; // = e.g. sneak skill

    // @todo skill jumps over to close by enemies = chain lightning

    // @todo illusion skill that multiplies the targeted mob and also shuffels the position

    // move_distances (e.g. launch attack propells attacker forward x distance)
    byte movement_distances;
};

#endif