#include "patterns.h"

#include "Math.h"
#include "Enemy.h"
#include "Bullet.h"

#include <stdlib.h>

#include <Math.h>


#define PATTERN_BULLET_MOVE_MAX 0

#define PATTERN_ENEMY_MOVE_MAX 1
#define PATTERN_ENEMY_ATTACK_MAX 0
#define PATTERN_ENEMY_THROW_TIME_MAX 0
#define PATTERN_ENEMY_THROW_MAX 0

#define PATTERN_FRIEND_MOVE_MAX 0
#define PATTERN_FRIEND_ATTACK_MAX 0
#define PATTERN_FRIEND_THROW_TIME_MAX 0
#define PATTERN_FRIEND_THROW_MAX 0


void basic_update_pos_pattern(Vec2 *v, Enemy *self, void **d, bool destroy)
{
	if (destroy) {
		if ((*d)) {
			free(*d);
		}
		return;
	}
	if (!(*d)) {
		(*d) = malloc(sizeof(bool));
		(*(bool *)(*d)) = true;
	}

	if ((*(bool *)(*d))) {
		if (v->y < 15) {
			(*(bool *)(*d)) = false;
		}
		v->y -= 0.2;
	} else {
		if (v->y > 1) {
			(*(bool *)(*d)) = true;
		}
		v->y += 0.2;
	}
}

func_ptr pattern_library[9][1] = {
	{},
	{(func_ptr)&basic_update_pos_pattern},
	{},
	{},
	{},
	{},
	{},
	{},
	{}
};

func_ptr get_pattern(PatternType pt, unsigned int pi)
{
	switch (pt) {
	case PATTERN_BULLET_MOVE: {
			if (pi < PATTERN_BULLET_MOVE_MAX) {
				return pattern_library[pt][pi];
			}
			break;
		}
	case PATTERN_ENEMY_MOVE: {
			if (pi < PATTERN_ENEMY_MOVE_MAX) {
				return pattern_library[pt][pi];
			}
			break;
		}
	case PATTERN_ENEMY_ATTACK: {
			if (pi < PATTERN_ENEMY_ATTACK_MAX) {
				return pattern_library[pt][pi];
			}
			break;
		}
	case PATTERN_ENEMY_THROW_TIME: {
			if (pi < PATTERN_ENEMY_THROW_MAX) {
				return pattern_library[pt][pi];
			}
			break;
		}
	case PATTERN_ENEMY_THROW: {
			if (pi < PATTERN_ENEMY_THROW_TIME_MAX) {
				return pattern_library[pt][pi];
			}
			break;
		}
	case PATTERN_FRIEND_MOVE: {
			if (pi < PATTERN_FRIEND_MOVE_MAX) {
				return pattern_library[pt][pi];
			}
			break;
		}
	case PATTERN_FRIEND_ATTACK: {
			if (pi < PATTERN_FRIEND_ATTACK_MAX) {
				return pattern_library[pt][pi];
			}
			break;
		}
	case PATTERN_FRIEND_THROW_TIME: {
			if (pi < PATTERN_FRIEND_THROW_MAX) {
				return pattern_library[pt][pi];
			}
			break;
		}
	case PATTERN_FRIEND_THROW: {
			if (pi < PATTERN_FRIEND_THROW_TIME_MAX) {
				return pattern_library[pt][pi];
			}
			break;
		}
	default: break;
	}
	abort();
}
