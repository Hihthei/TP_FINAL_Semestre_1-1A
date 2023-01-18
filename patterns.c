#include "patterns.h"

#include "Settings.h"
#include "Common.h"
#include "Scene.h"
#include "Enemy.h"
#include "Bullet.h"

#include <stdlib.h>

#include <Math.h>


#define PATTERN_BULLET_MOVE_MAX 2

#define PATTERN_ENEMY_MOVE_MAX 1
#define PATTERN_ENEMY_THROW_MAX 1

#define PATTERN_FRIEND_MOVE_MAX 0
#define PATTERN_FRIEND_THROW_MAX 0


void basic_update_pos_pattern(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		if (d->data) {
			free(d->data);
		}
		return;
	}

	bool *as = (bool *)d->data;

	if (!as) {
		d->data = malloc(sizeof(bool));
		as = (bool *)d->data;
		(*as) = true;
	}

	if (!(*as)) {
		if (self->position.y > 8) {
			(*as) = false;
			return;
		}
		self->position.y += 0.4f * Timer_GetDelta(g_time);
	} else {
		if (self->position.y < 1) {
			(*as) = true;
			return;
		}
		self->position.y -= 0.4f * Timer_GetDelta(g_time);
	}
}

void basic_throw_pattern(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	if (self->lastAttack == -1 || self->lastAttack > 5) {
		self->lastAttack = 0;
		Vec2 velocity = Vec2_Set(-2.0f, 0.0f);
		Bullet *bullet = Bullet_New(self->scene, self->position, velocity, BULLET_FIGHTER, 90.0f);
		Scene_AppendBullet(self->scene, bullet);
	} else {
		self->lastAttack += Timer_GetDelta(g_time);
	}
}

void bullet_auto_focus_pattern(struct Bullet_s *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	if (self->position.x < d->scene->player->position.x) {
		self->position = Vec2_Add(self->position, Vec2_Scale(self->velocity, Timer_GetDelta(g_time)));
	} else {
		Vec2 direction = Vec2_Normalize(Vec2_Sub(self->position, d->scene->player->position));
		self->position = Vec2_Scale(direction,
									Vec2_Length(Vec2_Add(self->position,
														 Vec2_Scale(self->velocity,
																	Timer_GetDelta(g_time))
														 ))
									);
	}
}

void bullet_random_pattern(struct Bullet_s *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	Vec2 direction = Vec2_Normalize(Vec2_Set(rand(), rand()));
	self->position = Vec2_Scale(direction,
								Vec2_Length(Vec2_Add(self->position,
													 Vec2_Scale(self->velocity,
																Timer_GetDelta(g_time))
													 ))
								);
}

func_ptr pattern_library[5][2] = {
	{(func_ptr)&bullet_random_pattern, (func_ptr)&bullet_auto_focus_pattern},
	{(func_ptr)&basic_update_pos_pattern},
	{(func_ptr)&basic_throw_pattern},
	{(func_ptr)NULL},
	{(func_ptr)NULL},
};

void set_patterns_scene(PatternData *d, int size, struct Scene_s *scene)
{
	int i = 0;
	while (i < size) {
		d[i].scene = scene;
		i++;
	}
}

void invalidate_patterns_data(PatternData *d, int size)
{
	int i = 0;
	while (i < size) {
		d[i].destroy = true;
		i++;
	}
}

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
	case PATTERN_ENEMY_THROW: {
			if (pi < PATTERN_ENEMY_THROW_MAX) {
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
	case PATTERN_FRIEND_THROW: {
			if (pi < PATTERN_FRIEND_THROW_MAX) {
				return pattern_library[pt][pi];
			}
			break;
		}
	default: break;
	}
	abort();
}
