#include "Enemy.h"
#include "Common.h"
#include "Scene.h"

void Enemy_Update_pos_impl(Vec2 *, Enemy *, void **, bool destroy);
void Enemy_Update_impl(Enemy *, void **, bool destroy);
void Enemy_Throw_Attack_impl(Enemy *self, void **, bool destroy);
void Enemy_No_Throw_Attack_impl(Enemy *self, void **, bool destroy);
bool Enemy_Should_Throw_Attack_impl(Enemy *self, void **, bool destroy);

Enemy *Enemy_New(Scene *scene, int type, Vec2 position)
{
    Enemy *self = (Enemy *)calloc(1, sizeof(Enemy));
    AssertNew(self);

    self->scene = scene;
    self->position = position;
    self->type = type;
    self->state = ENEMY_FIRING;
	self->lifePoints = 100;
	self->lastAttack = -1;

	self->update = &Enemy_Update_impl;
	self->updatePos = &Enemy_Update_pos_impl;
	self->shouldThrowAttack = &Enemy_Should_Throw_Attack_impl;
	self->throwAttack = &Enemy_Throw_Attack_impl;
	self->noThrowAttack = &Enemy_No_Throw_Attack_impl;

	memset(&self->_data[0], 0, sizeof(void *)*5);

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

	//Delete the possible _data fields.
	self->update(NULL, &self->_data[0], true);
	self->updatePos(NULL, NULL, &self->_data[1], true);
	self->throwAttack(NULL, &self->_data[2], true);
	self->noThrowAttack(NULL, &self->_data[3], true);
	self->shouldThrowAttack(NULL, &self->_data[4], true);

    free(self);
}

void Enemy_Update_pos_impl(Vec2 *v, Enemy *self, void **d, bool destroy)
{
	UNUSED(v);
	UNUSED(self);
	UNUSED(d);
	UNUSED(destroy);
	// [TODO] Add default func here.
}

void Enemy_Update_impl(Enemy *self, void **d, bool destroy)
{
	UNUSED(d);
	if (destroy) {
		return;
	}

	self->updatePos(&self->position, self, &self->_data[1], false);
	if (self->shouldThrowAttack(self, &self->_data[4], false) == true) {
		self->throwAttack(self, &self->_data[2], false);
	} else {
		self->noThrowAttack(self, &self->_data[3], false);
	}
}

void Enemy_No_Throw_Attack_impl(Enemy *self, void **d, bool destroy)
{
	UNUSED(d);
	if (destroy) {
		return;
	}

	self->lastAttack += g_time->elapsed;
}

void Enemy_Throw_Attack_impl(Enemy *self, void **d, bool destroy)
{
	UNUSED(d);
	if (destroy) {
		return;
	}

	self->lastAttack = 0;
	Vec2 velocity = Vec2_Set(-2.0f, 0.0f);
	Bullet *bullet = Bullet_New(self->scene, self->position, velocity, BULLET_FIGHTER, 90.0f);
	bullet->fromPlayer = false;
	Scene_AppendBullet(self->scene, bullet);
}

bool Enemy_Should_Throw_Attack_impl(Enemy *self, void **d, bool destroy)
{
	UNUSED(d);
	if (destroy) {
		return false;
	}
	return (self->lastAttack == -1 || self->lastAttack);
}

void Enemy_Update(Enemy *self)
{
	self->update(self, self->_data[0], false);
}

void Enemy_Render(Enemy *self)
{
	// On récupère des infos essentielles (communes à tout objet)
	Scene *scene = self->scene;
	SDL_Renderer *renderer = Scene_GetRenderer(scene);
	Camera *camera = Scene_GetCamera(scene);
	// On calcule la position en pixels en fonction de la position
	// en tuiles, la taille de la fenêtre et la taille des textures.
	float scale = Camera_GetWorldToViewScale(camera);
	SDL_FRect dst = { 0 };
	// Changez 48 par une autre valeur pour grossir ou réduire l'objet
	dst.h = 48 * PIX_TO_WORLD * scale;
	dst.w = 48 * PIX_TO_WORLD * scale;
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
	if (damage <= 0) {
		self->state = ENEMY_DEAD;
		// [TODO] Change to the right sprite.
	}
}
