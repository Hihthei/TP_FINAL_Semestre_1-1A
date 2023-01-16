#pragma once

typedef void (*func_ptr)();

typedef enum Pattern_Type
{
	PATTERN_BULLET_MOVE,
	PATTERN_ENEMY_MOVE,
	PATTERN_ENEMY_ATTACK,
	PATTERN_ENEMY_THROW_TIME,
	PATTERN_ENEMY_THROW,
	PATTERN_FRIEND_MOVE,
	PATTERN_FRIEND_ATTACK,
	PATTERN_FRIEND_THROW_TIME,
	PATTERN_FRIEND_THROW,
} PatternType;

func_ptr get_pattern(PatternType pattern_type, unsigned int pattern_id);
