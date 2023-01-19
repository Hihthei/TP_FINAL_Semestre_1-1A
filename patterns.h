#pragma once

#include "Settings.h"

#include <stdbool.h>

struct Enemy_s;
struct Bullet_s;
struct Player_s;
struct Scene_s;

/// @brief The different types of pattern availables.
typedef enum Pattern_Type
{
	PATTERN_BULLET_MOVE,
	PATTERN_ENEMY_MOVE,
	PATTERN_ENEMY_THROW,
	PATTERN_FRIEND_MOVE,
	PATTERN_FRIEND_THROW,
} PatternType;

/// @brief Structure used as persistent storage for pattern functions.
typedef struct PatternData_t
{
	struct Scene_s *scene;
	void *data;
	bool destroy;
} PatternData;

typedef void (*func_ptr)();
typedef bool (*bool_enemy_func_ptr)(struct Enemy_s *s, PatternData *d);
typedef void (*void_enemy_func_ptr)(struct Enemy_s *s, PatternData *d);
typedef void (*void_player_func_ptr)(struct Player_s *s, PatternData *d);
typedef void (*void_bullet_func_ptr)(struct Bullet_s *s, PatternData *d);

static inline bool bool_enemy_func_none(struct Enemy_s *s, PatternData *d) { UNUSED(s); UNUSED(d); return false; }
static inline void void_enemy_func_none(struct Enemy_s *s, PatternData *d) { UNUSED(s); UNUSED(d); }
static inline void void_player_func_none(struct Player_s *s, PatternData *d) { UNUSED(s); UNUSED(d); }
static inline void void_bullet_func_none(struct Bullet_s *s, PatternData *d) { UNUSED(s); UNUSED(d); }

/// @brief Set to 'true' the field 'destroy' of all the given PatternData instances.
/// @param d, array of PatternData of given size.
/// @param size of the array.
void invalidate_patterns_data(PatternData *d, int size);

/// @brief Set a scene on multiple PatternData objects.
/// @param d, array of PatternData of given size.
/// @param size of the array.
/// @param scene, the scene that you want to set on all the PatternData instances.
void set_patterns_scene(PatternData *d, int size, struct Scene_s *scene);

/// @brief Get a pattern function from the pattern library.
/// @param pattern_type type of the pattern you want to get.
/// @param pattern_id UID of the pattern of the given type.
func_ptr get_pattern(PatternType pattern_type, unsigned int pattern_id);

//Additional functions to get more features ;)
void enemy_drops_life(struct Enemy_s *self, PatternData *d);
