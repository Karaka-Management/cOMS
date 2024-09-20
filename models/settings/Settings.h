/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MODELS_SETTINGS_H
#define TOS_MODELS_SETTINGS_H

#include "../../stdlib/Types.h"
#include "../chat/ChatStatus.h"
#include "setting_types.h"

#if _WIN32
    #include <windows.h>
#else __linux__
    #include <linux/limits.h>
    #define MAX_PATH PATH_MAX
#endif

#ifndef RENDER_CHUNK_RADIUS
    #define RENDER_CHUNK_RADIUS 10
#endif

#ifndef RENDER_BLOCK_OBJECT_CHUNK_RADIUS
    #define RENDER_BLOCK_OBJECT_CHUNK_RADIUS 10
#endif

#ifndef RENDER_INTERACTIVE_CHUNK_RADIUS
    #define RENDER_INTERACTIVE_CHUNK_RADIUS 1
#endif

#ifndef RENDER_OBJECT_CHUNK_RADIUS
    #define RENDER_OBJECT_CHUNK_RADIUS 1
#endif

#ifndef RENDER_MONSTER_CHUNK_RADIUS
    #define RENDER_MONSTER_CHUNK_RADIUS 3
#endif

#ifndef RENDER_NPC_CHUNK_RADIUS
    #define RENDER_NPC_CHUNK_RADIUS 3
#endif

#ifndef RENDER_PAYER_CHUNK_RADIUS
    #define RENDER_PAYER_CHUNK_RADIUS 3
#endif

// @todo remove default values because we will load them during startup
struct SSettings {
    char path[MAX_PATH];
    bool is_changed = false;
    byte simd_version;

    char network_hostname[64];
    uint16 network_port;

    byte distance_terrain = RENDER_CHUNK_RADIUS;
    byte distance_terrain_secondary = RENDER_BLOCK_OBJECT_CHUNK_RADIUS;
    byte distance_terrain_tertiary = RENDER_INTERACTIVE_CHUNK_RADIUS;
    byte distance_models = RENDER_OBJECT_CHUNK_RADIUS;
    byte distance_monster = RENDER_MONSTER_CHUNK_RADIUS;
    byte distance_npc = RENDER_NPC_CHUNK_RADIUS;
    byte distance_player = RENDER_PAYER_CHUNK_RADIUS;

    uint32 player_cache = 8192; // = max active players on a server
    uint32 monster_cache = 8192;
    uint32 npc_cache = 8192;
    uint32 guild_cache = 128;
    uint32 message_cache = 1024;

    uint32 interpolation_buffer;

    bool is_auction_house_enabled = true;
    bool is_direct_trading_enabled = true;

    // @todo add more server settings for tournaments, tournament modes
    // @todo add more server settings for raids and dungeons
    // @todo add more settings for pvp
};

// Player settings that the server needs to know about
struct PSettings {
    byte render_distance_models = RENDER_OBJECT_CHUNK_RADIUS;
    byte render_distance_monster = RENDER_MONSTER_CHUNK_RADIUS;
    byte render_distance_npc = RENDER_NPC_CHUNK_RADIUS;
    byte render_distance_player = RENDER_PAYER_CHUNK_RADIUS;

    byte chat_status = CHAT_STATUS_OFFLINE;
    bool allow_invites = true;
};

struct CSettings {
    char path[MAX_PATH];
    bool is_changed = false;
    byte simd_version;

    char network_hostname[64];
    uint16 network_port;

    byte gpu_api = SETTING_TYPE_GPU_API_NONE;
    byte gpu_type = SETTING_TYPE_GPU_MEDIUM;
    byte gpu_fps = SETTING_TYPE_UNLIMITED;
    byte gpu_memory = 4; // how much vram are we using on the gpu

    byte gpu_aspect_ratio;
    byte gpu_resolution;
    byte gpu_brightness;
    byte gpu_contrast;
    byte gpu_gamma;
    byte gpu_fov;
    byte gpu_sync = SETTING_TYPE_DISABLED;

    uint32 gpu_number_of_npc_characters = 128;
    uint32 gpu_number_of_player_characters = 512;
    uint32 gpu_number_of_monster_characters = 128;

    byte gpu_render_distance_terrain = 10;
    byte gpu_render_distance_terrain_secondary = 10;
    byte gpu_render_distance_terrain_tertiary = 1;
    byte gpu_render_distance_models = 1;
    byte gpu_render_distance_monster = 3;
    byte gpu_render_distance_npc = 3;
    byte gpu_render_distance_player = 3;

    uint32 player_cache = 512;
    uint32 monster_cache = 128;
    uint32 npc_cache = 128;
    uint32 guild_cache = 128;
    uint32 message_cache = 64;

    byte gpu_animation_quality;
    byte gpu_attack_effect_quality;
    byte gpu_shadow_quality;
    byte gpu_terrain_quality;
    byte gpu_water_quality;
    byte gpu_grass_density;
    byte gpu_model_quality;
    byte gpu_texture_quality;
    byte gpu_foliage_distance;
    byte gpu_detail_level;
    byte gpu_reflection_quality;
    byte gpu_refraction_quality;
    byte gpu_caustics_quality;
    byte gpu_footprint_quality; // mostly used for snow, sand mud
    bool gpu_screen_effects; // e.g. water droplets/dust/freezing on screen

    bool gpu_raytracing = false;
    bool gpu_lense_effect = true;
    bool gpu_fog_effect = true;
    bool gpu_particles_environment = true;
    bool gpu_particles_players = true;
    bool gpu_particles_monster = true;
    bool gpu_particles_ui = true;
    bool gpu_particles_skills = true;
    bool gpu_particles_weapons = true;

    byte gpu_shadow_type = SETTING_TYPE_DISABLED; // none, baked, shadow mapping, point shadow, ray tracing
    byte gpu_light_ssao = SETTING_TYPE_DISABLED;
    byte gpu_light_bloom = SETTING_TYPE_DISABLED;

    byte gpu_reflection_blur = SETTING_TYPE_DISABLED;
    byte gpu_motion_blur = SETTING_TYPE_DISABLED;
    byte gpu_blur = SETTING_TYPE_DISABLED;
    byte gpu_anti_aliasing = SETTING_TYPE_DISABLED;
    byte gpu_sharpening = SETTING_TYPE_DISABLED;
    byte gpu_ambient_occlusion = SETTING_TYPE_DISABLED;

    bool gpu_gamma_correction = true;
    bool gpu_normal_mapping = true;
    bool gpu_parallax_mapping = true;

    bool gpu_depth_of_field = true;
    bool gpu_chromatic_aberration = true;
    bool gpu_vignetting = true;
    bool gpu_light_shafts = true;
    bool gpu_camera_shake = false;

    byte audio_volume_master = 128;
    byte audio_volume_game = 128;
    byte audio_volume_environment = 128;
    byte audio_volume_music = 128;
    byte audio_volume_speech = 128;

    uint32 game_window1_dim[2] = {1024, 768};
    uint32 game_window1_pos[2];
    byte game_window1_mode = SETTING_TYPE_WINDOW_MODE_FULLSCREEN;

    byte game_zoom;
    byte game_view = SETTING_TYPE_PERSPECTIVE_FIRST;

    // General game UI
    bool game_crosshair = false;

    bool game_healthbar_self = false;
    bool game_healthbar_group = false;
    bool game_healthbar_players = false;
    bool game_healthbar_npc = false;
    bool game_healthbar_monsters = false;

    bool game_name_monsters = false;
    bool game_name_npc = false;
    bool game_name_mobs = false;

    bool game_show_auras = true;
    bool game_show_helmets = true;

    uint32 game_interpolation_buffer;

    bool game_player_chat = false; // bubble above player
    byte game_chat_status = CHAT_STATUS_OFFLINE;

    // Extra windows
    byte game_window2_type = SETTING_TYPE_DISABLED;
    bool game_window2_visible = false;
    uint32 game_window2_dim[2] = {1024, 768};
    uint32 game_window2_pos[2];

    byte game_window3_type = SETTING_TYPE_DISABLED;
    bool game_window3_visible = false;
    uint32 game_window3_dim[2] = {1024, 768};
    uint32 game_window3_pos[2];

    byte game_window4_type = SETTING_TYPE_DISABLED;
    bool game_window4_visible = false;
    uint32 game_window4_dim[2] = {1024, 768};
    uint32 game_window4_pos[2];

    byte game_window5_type = SETTING_TYPE_DISABLED;
    bool game_window5_visible = false;
    uint32 game_window5_dim[2] = {1024, 768};
    uint32 game_window5_pos[2];

    // @todo Consider to allow settings for chat tabs
    //  define which messags go to which tab
    //  define custom chat tabs

    // UI settings
    // Themes
    uint32 game_ui_theme = 1;
    byte game_ui_size = 128;

    uint32 game_item_icon_theme = 1;

    uint32 game_menu_theme = 1;
    byte game_menu_size = 128;

    uint32 game_map_mini_theme = 1;
    byte game_map_mini_size = 128;
    int32 game_map_mini_pos[2] = { -1, -1 };

    uint32 game_quest_theme = 1;
    byte game_quest_size = 128;
    int32 game_quest_pos[2] = { -1, -1 };

    uint32 game_skill_bar_theme = 1;
    bool game_skill_bar_animated = false;
    byte game_skill_bar_size = 128;
    int32 game_skill_bar_pos[2] = { -1, -1 };

    uint32 game_health_theme = 1;
    bool game_health_animated = false;
    byte game_health_size = 128;
    int32 game_health_pos[2] = { -1, -1 };

    uint32 game_resource_theme = 1;
    bool game_resource_animated = false;
    byte game_resource_size = 128;
    int32 game_resource_pos[2] = { -1, -1 };

    uint32 game_party_theme = 1;
    bool game_party_animated = false;
    byte game_party_size = 128;
    int32 game_party_pos[2] = { -1, -1 };

    uint32 game_enemy_theme = 1;
    bool game_enemy_animated = false;
    byte game_enemy_size = 128;
    int32 game_enemy_pos[2] = { -1, -1 };

    uint32 game_select_info_theme = 1;
    byte game_select_info_size = 128;
    int32 game_select_info_pos[2] = { -1, -1 };

    uint32 game_chat_theme = 1;
    byte game_chat_size = 128;
    int32 game_chat_pos[2] = { -1, -1 };

    // HUD
    bool game_show_health_bar_self = false;
    bool game_show_health_bar_player = false;
    bool game_show_health_bar_monster = false;
    bool game_show_health_numbers = false;
    bool game_show_resource_numbers = false;
    bool game_show_buffs = false;
    bool game_show_xp_bar_numbers = true;

    bool game_show_name_self = false;
    bool game_show_name_player = false;
    bool game_show_name_monster = false;
    bool game_show_name_npc = false;
    bool game_show_title_self = false;
    bool game_show_title_other = false;
    byte game_minion_visibility_self = 128;
    byte game_minion_visibility_player = 128;

    bool game_show_dmg_numbers = false;
    bool game_show_cooldown_times = false;
    bool game_show_dodge = true;
    bool game_show_effect_gains = true; // e.g. XP

    bool game_minimap_show_merchants = false;
    bool game_minimap_show_quest = false;
    bool game_minimap_show_dungeons = false;
    bool game_minimap_show_names = false;
    bool game_show_clock = false;

    bool game_map_show_merchants = false;
    bool game_map_show_quest = false;
    bool game_map_show_dungeons = false;
    bool game_map_show_names = false;

    bool game_show_subtitles = true;

    // Mounts
    uint32 game_default_mount = 0;

    // Game behavior
    bool game_error_audio = true;
    bool game_error_text = true;
    byte game_default_zoom = 128;
    bool game_block_trade = false;
    bool game_block_group_invite = false;
    bool game_block_guild_invite = false;
    bool game_block_chat_invite = false;
    bool game_block_friend_invite = false;
    bool game_automatically_track_newest_quest = false;

    byte game_interact_direction = 0;
    byte game_interact_radius = 1;

    // Game pad settings
    byte stick_left_deadzone = 0;
    byte stick_right_deadzone = 0;

    byte input_look_speed = 0;
    bool input_invert_mouse = false;
    bool input_lock_cursor_to_window = true;
    bool input_click_to_move = true;

    // Hotkey settings
    // @todo hotkey combination e.g. alt+1
    byte hotkeys_movement_up = 0x57; // W
    byte hotkeys_movement_down = 0x53; // S
    byte hotkeys_movement_left = 0x41; // A
    byte hotkeys_movement_right = 0x44; // D

    byte hotkeys_cancel_action = 0x44; // X

    byte hotkeys_skill_tab_1 = 0x70; // F1
    byte hotkeys_skill_tab_2 = 0x71; // F2
    byte hotkeys_skill_tab_3 = 0x72; // F3

    byte hotkeys_skill_1 = 0x31; // 1
    byte hotkeys_skill_2 = 0x32; // 2
    byte hotkeys_skill_3 = 0x33; // 3
    byte hotkeys_skill_4 = 0x34; // 4
    byte hotkeys_skill_5 = 0x35; // 5
    byte hotkeys_skill_6 = 0x36; // 6
    byte hotkeys_skill_7 = 0x37; // 7
    byte hotkeys_skill_8 = 0x38; // 8
    byte hotkeys_skill_9 = 0x39; // 9
    byte hotkeys_skill_10 = 0x30; // 0

    byte hotkeys_interact = 45; // E
    byte hotkeys_jump = 0x20; // SPACE

    byte hotkeys_dodge = 0x20; // SPACE
    byte hotkeys_crouch = 0x14; // CAP
    byte hotkeys_walk = 0x11; // CTRL (switches to walking speed, one click only)
    byte hotkeys_emote = 0x12; // LEFT_ALT
    byte hotkeys_vertical_up = 0x14; // CAPS LOCK
    byte hotkeys_vertical_down = 0x10; // SHIFT

    byte hotkeys_view_next = 0x09; // TAB
    byte hotkeys_view_prv = 0x14; // CAPS LOCK
    byte hotkeys_compare_item = 0x11; // CTRL

    byte hotkeys_inventory = 0x49; // I
    byte hotkeys_character = 0x43; // C
    byte hotkeys_skills = 0x53; // S
    byte hotkeys_map = 0x4D; // M
    byte hotkeys_quest = 0x51; // Q
    byte hotkeys_teleport = 0x54; // T
    byte hotkeys_attack_move = 0x52; // R
    byte hotkeys_force_move = 0x46; // F
    byte hotkeys_courser_move = 0x58; // X (move to where courser is)

    byte hotkeys_chat = 0x0D; // ENTER
    byte hotkeys_hide_ui = 0x48; // H

    byte hotkey_zoom_in = 0x21; // Page up (@todo make mouse scroll up)
    byte hotkey_zoom_out = 0x22; // page down (@todo make mouse scroll down)
    byte hotkey_camera_look = 0x00; // @todo make right mouse hold

    byte hotkeys_menu = 0x1B; // ESC
    byte hotkeys_window_close = 0x1B; // ESC

    byte hotkeys_marker_1 = 0x31;
    byte hotkeys_marker_2 = 0x32;
    byte hotkeys_marker_3 = 0x33;
    byte hotkeys_marker_4 = 0x34;
    byte hotkeys_marker_5 = 0x35;
    byte hotkeys_marker_6 = 0x36;
    byte hotkeys_ping = 0x37;

    // Camera settings/positions
    // Makes it easy to switch to customizable camera positions
    byte hotkeys_camera_1 = 0x0;
    byte hotkeys_camera_2 = 0x0;
    byte hotkeys_camera_3 = 0x0;

    // @todo implement the actual camera settings
};

#endif