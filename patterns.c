#include "patterns.h"

#include "Settings.h"
#include "Common.h"
#include "Scene.h"
#include "Enemy.h"
#include "Bullet.h"

#include <stdlib.h>

#include <Math.h>


#define PATTERN_BULLET_MOVE_MAX 4

#define PATTERN_ENEMY_MOVE_MAX 3
#define PATTERN_ENEMY_THROW_MAX 6

#define PATTERN_FRIEND_MOVE_MAX 0
#define PATTERN_FRIEND_THROW_MAX 0

#define NORMAL_DAMAGES 2
#define BOSS_DAMAGES 5


	/*
	 * [SECTION] Helper structures and functions for the pattern functions
	 */


float absf(float f)
{
	if (f < 0) {
		return -f;
	}
	return f;
}

struct SlowData_t
{
	float sourceTime;
};

struct SpiralData_t
{
	float sourceTime;
	Vec2 source;
	Vec2 source2;
};

struct TwoTimes_t
{
	float first;
	float second;
	bool state;
};

struct ThreeThrows_t
{
	float a;
	float first;
	float second;
	bool mid;
};


	/*
	 * [SECTION] Enemy position patterns
	 */


void enemy_update_pos_pattern(Enemy *self, PatternData *d)
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

	if (*as) {
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

void enemy_circle_pos_pattern(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		if (d->data) {
			free(d->data);
		}
		return;
	}

	struct SpiralData_t *as = (struct SpiralData_t *)d->data;

	if (!as) {
		d->data = malloc(sizeof(struct SpiralData_t));
		as = (struct SpiralData_t *)d->data;
		as->sourceTime = 0;
		as->source = Vec2_Add(self->position, Vec2_Set(-2, 0));
	}

	as->sourceTime += Timer_GetDelta(g_time);
	self->position = Vec2_Add(as->source, Vec2_Set(cos(as->sourceTime)*1.5, sin(as->sourceTime)*1.5));
}

void enemy_kamikaze_pos_pattern(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	Vec2 direction = Vec2_Normalize(Vec2_Sub(self->scene->player->position, self->position));
	self->position = Vec2_Add(self->position, Vec2_Scale(direction, 1 * Timer_GetDelta(g_time)));
}


	/*
	 * [SECTION] Bullet patterns
	 */


void bullet_spiral_pattern(struct Bullet_s *self, PatternData *d)
{
	if (d->destroy) {
		if (d->data) {
			free(d->data);
		}
		return;
	}

	struct SpiralData_t *as = (struct SpiralData_t *)d->data;

	if (!as) {
		d->data = malloc(sizeof(struct SpiralData_t));
		as = (struct SpiralData_t *)d->data;
		as->sourceTime = Timer_GetElapsed(g_time);
		as->source = self->position;
	}

	Vec2 direction = Vec2_Rotate(Vec2_Set(1, 0), 1*(Timer_GetElapsed(g_time)-as->sourceTime));
	self->position = Vec2_Add(as->source,
							  Vec2_Scale(direction, 0.1f*(Timer_GetElapsed(g_time)-as->sourceTime))
							  );
	self->position = Vec2_Add(Vec2_Set(-Vec2_Length(direction)*1.1*(Timer_GetElapsed(g_time)-as->sourceTime), 0), Vec2_Add(self->position, Vec2_Scale(self->velocity, Timer_GetDelta(g_time))));
}

void bullet_random_pattern(struct Bullet_s *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	Vec2 direction = Vec2_Normalize(Vec2_Set(rand(), rand()));
	self->position = Vec2_Add(Vec2_Scale(direction,
								Vec2_Length(Vec2_Add(self->position,
													 Vec2_Scale(self->velocity,
																Timer_GetDelta(g_time))
													 ))
								),
								self->position
							 );
}

void bullet_slow_pattern(struct Bullet_s *self, PatternData *d)
{
	if (d->destroy) {
		if (d->data) {
			free(d->data);
		}
		return;
	}

	struct SpiralData_t *as = (struct SpiralData_t *)d->data;

	if (!as) {
		d->data = malloc(sizeof(struct SpiralData_t));
		as = (struct SpiralData_t *)d->data;
		as->sourceTime = 0;
	}

	as->sourceTime += Timer_GetElapsed(g_time);
	self->position = Vec2_Add(self->position, Vec2_Set(absf(cos(as->sourceTime)), 0));
}

void bullet_player_auto_focus_pattern(struct Bullet_s *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	if (self->position.x < self->scene->player->position.x) {
		self->position = Vec2_Add(self->position, Vec2_Scale(self->velocity, Timer_GetDelta(g_time)));
	} else {
		self->velocity = Vec2_Scale(Vec2_Normalize(Vec2_Sub(self->scene->player->position, self->position)), Vec2_Length(self->velocity));
		self->position = Vec2_Add(self->position, Vec2_Scale(self->velocity, Timer_GetDelta(g_time)));
		self->angle = angle_radian_to_degrees(Vec2_AngleBetweenY(self->velocity));
	}
}


	/*
	 * [SECTION] Enemy's bullet sending pattern.
	 */


void enemy_throw_pattern(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	if (self->lastAttack == -1 || self->lastAttack > 5) {
		self->lastAttack = 0;
		Vec2 velocity = Vec2_Set(-2.0f, 0.0f);
		Bullet *bullet = Bullet_New(self->scene, self->position, velocity, BULLET_FIGHTER, 90.0f, NORMAL_DAMAGES);
		bullet->updatePos = &bullet_player_auto_focus_pattern;
		Scene_AppendBullet(self->scene, bullet);
		mixer_play_music(self->scene->mixer, FighterShotSound, 1);
	} else {
		self->lastAttack += Timer_GetDelta(g_time);
	}
}

void enemy_throw_two_pattern(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		if (d->data) {
			free(d->data);
		}
		return;
	}

	struct TwoTimes_t *as = (struct TwoTimes_t *)d->data;

	if (!as) {
		d->data = malloc(sizeof(struct TwoTimes_t));
		as = (struct TwoTimes_t *)d->data;
		as->first = as->second = 0;
		as->state = false;
	}

	if (as->state) {
		as->second += Timer_GetDelta(g_time);
		if (as->second > 0.5) {
			as->state = false;
			as->second = 0;
			Scene_AppendBullet(self->scene, Bullet_New(self->scene, self->position, Vec2_Set(-3.0, 0), BULLET_FIGHTER, 90.0f, NORMAL_DAMAGES));
			mixer_play_music(self->scene->mixer, FighterShotSound, 1);
			return;
		}
	} else {
		as->first += Timer_GetDelta(g_time);
		if (as->first > 2) {
			as->state = true;
			as->first = 0;
			Scene_AppendBullet(self->scene, Bullet_New(self->scene, self->position, Vec2_Set(-3.0, 0), BULLET_FIGHTER, 90.0f, NORMAL_DAMAGES));
			mixer_play_music(self->scene->mixer, FighterShotSound, 1);
			return;
		}
	}
}

void enemy_throw_three_pattern(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		if (d->data) {
			free(d->data);
		}
		return;
	}

	struct ThreeThrows_t *as = (struct ThreeThrows_t *)d->data;

	if (!as) {
		d->data = malloc(sizeof(struct ThreeThrows_t));
		as = (struct ThreeThrows_t *)d->data;
		as->first = as->second = as->a = 0;
		as->mid = false;
	}

	as->first += Timer_GetDelta(g_time);
	as->second += Timer_GetDelta(g_time);

	if (as->second > 2) {
		as->mid = !as->mid;
		as->second = 0;
	}

	if (as->mid) {
		Bullet *bullet = Bullet_New(self->scene, self->position, Vec2_Set(-3, 0), BULLET_FIGHTER, 90.0f, BOSS_DAMAGES);
		bullet->texture = self->scene->assets->fighterAttack2;
		Scene_AppendBullet(self->scene, bullet);
	}
	if (as->first > 0.2) {
		as->first = 0;
		as->a += 0.1;
		if (as->a > 0.8) {
			as->a = 0;
		}

		Bullet *bullet = Bullet_New(self->scene, self->position, Vec2_Rotate(Vec2_Set(-2, 0), as->a), BULLET_FIGHTER, 90.0f, NORMAL_DAMAGES);
		Scene_AppendBullet(self->scene, bullet);
		bullet = Bullet_New(self->scene, self->position, Vec2_Rotate(Vec2_Set(-2, 0), -as->a), BULLET_FIGHTER, 90.0f, NORMAL_DAMAGES);
		Scene_AppendBullet(self->scene, bullet);
		mixer_play_music(self->scene->mixer, FighterShotSound, 1);
	}
}

void enemy_throw_mid_three_pattern(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		if (d->data) {
			free(d->data);
		}
		return;
	}

	struct ThreeThrows_t *as = (struct ThreeThrows_t *)d->data;

	if (!as) {
		d->data = malloc(sizeof(struct ThreeThrows_t));
		as = (struct ThreeThrows_t *)d->data;
		as->first = as->second = as->a = 0;
		as->mid = false;
	}

	as->first += Timer_GetDelta(g_time);
	as->second += Timer_GetDelta(g_time);

	if (as->second > 1) {
		as->mid = !as->mid;
		as->second = 0;
	}

	if (as->first > 0.2) {
		as->first = 0;

		if (as->mid) {
			if (as->a < -0.8) {
				as->mid = false;
			}
			as->a -= 0.1;
		} else {
			if (as->a > 0.8) {
				as->mid = true;
			}
			as->a += 0.1;
		}

		Bullet *bullet = Bullet_New(self->scene, self->position, Vec2_Set(-2, 0), BULLET_FIGHTER, 90.0f, 2);
		bullet->texture = self->scene->assets->fighterAttack2;
		Scene_AppendBullet(self->scene, bullet);
		Scene_AppendBullet(self->scene, Bullet_New(self->scene, self->position, Vec2_Rotate(Vec2_Set(-2, 0), as->a), BULLET_FIGHTER, 90.0f, NORMAL_DAMAGES));
		Scene_AppendBullet(self->scene, Bullet_New(self->scene, self->position, Vec2_Rotate(Vec2_Set(-2, 0), -as->a), BULLET_FIGHTER, 90.0f, NORMAL_DAMAGES));
		mixer_play_music(self->scene->mixer, FighterShotSound, 1);
	}
}

void enemy_throw_hard_pattern(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		if (d->data) {
			free(d->data);
		}
		return;
	}

	struct SpiralData_t *as = (struct SpiralData_t *)d->data;

	if (!as) {
		d->data = malloc(sizeof(struct SpiralData_t));
		as = (struct SpiralData_t *)d->data;
		as->source = Vec2_Set(2, 0);
		as->source2 = Vec2_Set(2, 0);
		as->sourceTime = 0;
	}


	//Scene_AppendBullet(d->scene, Bullet_New(d->scene, self->position, Vec2_Rotate(as->source, a*i), BULLET_FIGHTER, 90.0f));

	as->sourceTime += Timer_GetDelta(g_time);
	if (as->sourceTime > 0.08) {
		Bullet *bullet = Bullet_New(self->scene, self->position, as->source, BULLET_FIGHTER, 90.0f, BOSS_DAMAGES);
		bullet->texture = self->scene->assets->fighterSpecial;
		Scene_AppendBullet(self->scene, bullet);
		bullet = Bullet_New(self->scene, self->position, Vec2_Rotate(as->source, M_PI_2), BULLET_FIGHTER, 90.0f, BOSS_DAMAGES);
		bullet->texture = self->scene->assets->fighterSpecial;
		Scene_AppendBullet(self->scene, bullet);
		bullet = Bullet_New(self->scene, self->position, Vec2_Rotate(as->source, M_PI), BULLET_FIGHTER, 90.0f, BOSS_DAMAGES);
		bullet->texture = self->scene->assets->fighterSpecial;
		Scene_AppendBullet(self->scene, bullet);
		bullet = Bullet_New(self->scene, self->position, Vec2_Rotate(as->source, -M_PI_2), BULLET_FIGHTER, 90.0f, BOSS_DAMAGES);
		bullet->texture = self->scene->assets->fighterSpecial;
		Scene_AppendBullet(self->scene, bullet);

		bullet = Bullet_New(self->scene, self->position, as->source2, BULLET_FIGHTER, 90.0f, BOSS_DAMAGES);
		bullet->texture = self->scene->assets->fighterSpecial;
		Scene_AppendBullet(self->scene, bullet);
		bullet = Bullet_New(self->scene, self->position, Vec2_Rotate(as->source2, M_PI_2), BULLET_FIGHTER, 90.0f, BOSS_DAMAGES);
		bullet->texture = self->scene->assets->fighterSpecial;
		Scene_AppendBullet(self->scene, bullet);
		bullet = Bullet_New(self->scene, self->position, Vec2_Rotate(as->source2, M_PI), BULLET_FIGHTER, 90.0f, BOSS_DAMAGES);
		bullet->texture = self->scene->assets->fighterSpecial;
		Scene_AppendBullet(self->scene, bullet);
		bullet = Bullet_New(self->scene, self->position, Vec2_Rotate(as->source2, -M_PI_2), BULLET_FIGHTER, 90.0f, BOSS_DAMAGES);
		bullet->texture = self->scene->assets->fighterSpecial;
		Scene_AppendBullet(self->scene, bullet);
		as->source = Vec2_Rotate(as->source, 0.5);
		as->source2 = Vec2_Rotate(as->source2, -0.5);
		as->sourceTime = 0;

		mixer_play_music(self->scene->mixer, FighterShotSound, 1);
	}
}

void enemy_auto_aim_pattern(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		if (d->data) {
			free(d->data);
		}
		return;
	}

	float *as = (float *)d->data;

	if (!as) {
		d->data = malloc(sizeof(float));
		as = (float *)d->data;
		(*as) = 0;
	}

	(*as) += Timer_GetDelta(g_time);

	if ((*as) <= 1.0) {
		Vec2 direction = Vec2_Normalize(Vec2_Sub(self->scene->player->position, self->position));
		Bullet *bullet = Bullet_New(self->scene, self->position, Vec2_Scale(direction, 3), BULLET_FIGHTER, 90.0f, NORMAL_DAMAGES);
		Scene_AppendBullet(self->scene, bullet);
		mixer_play_music(self->scene->mixer, FighterShotSound, 1);
	} else if ((*as) >= 2) {
		(*as) = 0;
	}
}

void bullet_enemy_auto_focus_pattern(struct Bullet_s *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	if (self->scene->enemyCount == 0) {
		self->position = Vec2_Add(self->position, Vec2_Scale(self->velocity, Timer_GetDelta(g_time)));
	}

	int min = 0;
	float minDist = Vec2_Distance(self->position, self->scene->enemies[0]->position);
	for (int i = 1; i < self->scene->enemyCount; i++) {
		float dist = Vec2_Distance(self->position, self->scene->enemies[i]->position);
		if (dist < minDist) {
			minDist = dist;
			min = i;
		}
	}

	self->velocity = Vec2_Scale(Vec2_Normalize(Vec2_Sub(self->scene->enemies[min]->position, self->position)), Vec2_Length(self->velocity));
	self->position = Vec2_Add(self->position, Vec2_Scale(self->velocity, Timer_GetDelta(g_time)));
	self->angle = angle_radian_to_degrees(Vec2_AngleBetweenY(self->velocity));
}

void enemy_drops_life(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		//The enemy's dead, generate the power up.
		Bullet *bullet = Bullet_New(self->scene, self->position, Vec2_Set(0, 0), BULLET_FIGHTER, 90.0f, -15);
		bullet->texture = self->scene->assets->healDrop;
		Scene_AppendBullet(self->scene, bullet);
	}
}

void bullet_auto_depop_pattern(struct Bullet_s *self, PatternData *d)
{
	if (d->destroy) {
		if (d->data) {
			free(d->data);
		}
		return;
	}

	float *as = (float *)d->data;

	if (!as) {
		d->data = malloc(sizeof(float));
		as = (float *)d->data;
		(*as) = 0;
	}

	(*as) += Timer_GetDelta(g_time);

	if ((*as) >= 0.5) {
		//This way, the bullet will be out of bounds and will be automatically deleted.
		self->position = Vec2_Set(-1, -1);
	}
}

func_ptr pattern_library[5][6] = {
	/*PATTERN_BULLET_MOVE*/{
		(func_ptr)&bullet_random_pattern,
		(func_ptr)&bullet_player_auto_focus_pattern,
		(func_ptr)&bullet_slow_pattern,
		(func_ptr)&bullet_spiral_pattern
	},
	/*PATTERN_ENEMY_MOVE*/ {
		(func_ptr)&enemy_update_pos_pattern,
		(func_ptr)&enemy_circle_pos_pattern,
		(func_ptr)&enemy_kamikaze_pos_pattern
	},
	/*PATTERN_ENEMY_THROW*/{
		(func_ptr)&enemy_throw_pattern,
		(func_ptr)&enemy_throw_two_pattern,
		(func_ptr)&enemy_throw_mid_three_pattern,
		(func_ptr)&enemy_throw_three_pattern,
		(func_ptr)&enemy_throw_hard_pattern,
		(func_ptr)&enemy_auto_aim_pattern
	},
	{(func_ptr)NULL},
	{(func_ptr)NULL},
};

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
