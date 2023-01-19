#include "Enemy.h"
#include "Common.h"
#include "Scene.h"

void Enemy_Update_pos_impl(Enemy *, PatternData *);
void Enemy_Update_impl(Enemy *, PatternData *);
void Enemy_Throw_Attack_impl(Enemy *self, PatternData *);

Enemy *Enemy_New(Scene *scene, int type, Vec2 position)
{
    Enemy *self = (Enemy *)calloc(1, sizeof(Enemy));
    AssertNew(self);

    self->scene = scene;
    self->position = position;
    self->type = type;
    self->state = ENEMY_FIRING;
	self->lifePoints = 20;
	self->lastAttack = -1;
	self->collisionDamages = 10;

	self->update = &Enemy_Update_impl;
	self->updatePos = &Enemy_Update_pos_impl;
	self->throwAttack = &Enemy_Throw_Attack_impl;
	self->enemyRaisedOrDead = &void_enemy_func_none;

	memset(&self->_data[0], 0, sizeof(PatternData)*4);

    Assets *assets = Scene_GetAssets(self->scene);
    switch (type)
    {
    case ENEMY_FIGHTER:
        self->worldW = 64 * PIX_TO_WORLD;
        self->worldH = 64 * PIX_TO_WORLD;
        self->radius = 0.4f;
        self->texture = assets->fighter;
        break;

    default:
        assert(false);
        break;
    }

    return self;
}

void Enemy_Delete(Enemy *self)
{
    if (!self) return;

	invalidate_patterns_data(&self->_data[0], 4);
	self->enemyRaisedOrDead(self, &self->_data[3]);
	self->update(NULL, &self->_data[0]);
	self->updatePos(NULL, &self->_data[1]);
	self->throwAttack(NULL, &self->_data[2]);

	mixer_play_music(self->scene->mixer, DestructionSound, 1);

    free(self);
}

void Enemy_Update_pos_impl(Enemy *self, PatternData *d)
{
	UNUSED(self);
	UNUSED(d);
	// [TODO] Add default func here.
}

void Enemy_Update_impl(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	self->updatePos(self, &self->_data[1]);
	self->throwAttack(self, &self->_data[2]);
}

void Enemy_Throw_Attack_impl(Enemy *self, PatternData *d)
{
	UNUSED(self);
	UNUSED(d);
}

bool Enemy_Should_Throw_Attack_impl(Enemy *self, PatternData *d)
{
	if (d->destroy) {
		return false;
	}
	return (self->lastAttack == -1 || self->lastAttack);
}

void Enemy_Update(Enemy *self)
{
	if (self->firstUpdate) {
		mixer_play_music(self->scene->mixer, ApparitionSound, 1);
		self->firstUpdate = false;
		self->enemyRaisedOrDead(self, &self->_data[3]);
	}
	self->update(self, &self->_data[0]);
}

void Enemy_Render(Enemy *self)
{
	// On récupère des infos essentielles (communes à tout objet)
	SDL_Renderer *renderer = Scene_GetRenderer(self->scene);
	Camera *camera = Scene_GetCamera(self->scene);
	// On calcule la position en pixels en fonction de la position
	// en tuiles, la taille de la fenêtre et la taille des textures.
	float scale = Camera_GetWorldToViewScale(camera);
	SDL_FRect dst = { 0 };
	// Changez 48 par une autre valeur pour grossir ou réduire l'objet
	dst.h = dst.w = scale;
	Camera_WorldToView(camera, self->position, &dst.x, &dst.y);
	// Le point de référence est le centre de l'objet
	dst.x -= 0.50f * dst.w;
	dst.y -= 0.50f * dst.h;
	// On affiche en position dst (unités en pixels)
	SDL_RenderCopyExF(renderer, self->texture, NULL, &dst, 90.0f, NULL, 0);
}

void Enemy_Damage(Enemy *self, int damage)
{
	self->lifePoints -= damage;
	if (self->lifePoints <= 0) {
		self->state = ENEMY_DEAD;
		mixer_play_music(self->scene->mixer, DestructionSound, 1);
	}
}
