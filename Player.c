#include "Player.h"
#include "Scene.h"
#include "Common.h"


void Player_Update_impl(Player *, PatternData *);
void Player_Update_pos_impl(struct Player_s *, PatternData *);

Player *Player_New(Scene *scene)
{
    Player *self = (Player *)calloc(1, sizeof(Player));
    AssertNew(self);

    Assets *assets = Scene_GetAssets(scene);

    self->scene = scene;
    self->position = Vec2_Set(1.0f, 4.5f);
    self->radius = 0.25f;
	self->texture = assets->player;
	self->lastAttack = -1;
	self->lifePoints = 100;

	self->update = &Player_Update_impl;
	self->updatePos = &Player_Update_pos_impl;
	self->playerDead = &nofunc;
	self->playerCollision = &nofunc;

	memset(&self->_data[0], 0, sizeof(PatternData)*2);
	set_patterns_scene(&self->_data[0], 2, scene);

    return self;
}

void Player_Delete(Player *self)
{
    if (!self) return;

	invalidate_patterns_data(self->_data, 2);
	self->update(NULL, &self->_data[0]);
	self->updatePos(NULL, &self->_data[1]);

    free(self);
}

void Player_Update_pos_impl(Player *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	// On récupère des infos essentielles (communes à tout objet)
	Scene *scene = self->scene;
	Input *input = Scene_GetInput(scene);
	// Mise à jour de la vitesse en fonction de l'état des touches
	Vec2 velocity = Vec2_Scale(Vec2_Set(input->hAxis, input->vAxis), 2);
	// Mise à jour de la position
	self->position = Vec2_Add(self->position, Vec2_Scale(velocity, Timer_GetDelta(g_time)));

	if (self->position.x < self->radius) {
		self->position.x = self->radius;
	}
	else if (self->position.x > 16 - self->radius) {
		self->position.x = 16 - self->radius;
	}
	if (self->position.y < self->radius) {
		self->position.y = self->radius;
	}
	else if (self->position.y > 9 - self->radius) {
		self->position.y = 9 - self->radius;
	}
}

void Player_Update_impl(Player *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	// On récupère des infos essentielles (communes à tout objet)
	Scene *scene = self->scene;
	Input *input = Scene_GetInput(scene);

	self->updatePos(self, &self->_data[1]);

	if (input->shootPressed) {
		//Don't let 'em spam it all!
		if (self->lastAttack == -1 || self->lastAttack > 50) {
			Vec2 velocity = Vec2_Set(4.0f, 0.0f);
			Bullet *bullet = Bullet_New(self->scene, self->position, velocity, BULLET_PLAYER, 90.0f);
			Scene_AppendBullet(self->scene, bullet);
			self->lastAttack = 0;
		} else {
			self->lastAttack += g_time->elapsed;
		}
	}
}

void Player_Update(Player *self)
{
	self->update(self, &self->_data[0]);
}

void Player_Render(Player *self)
{
	// On récupère des infos essentielles (communes à tout objet)
	Scene *scene = self->scene;
	SDL_Renderer *renderer = Scene_GetRenderer(scene);
	//Assets *assets = Scene_GetAssets(scene);
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

//Bullet might be null if we collided with another ship.
void Player_Damage(Player *self, int damage, void *bullet)
{
	UNUSED(bullet);

	self->lifePoints -= damage;
	if (self->lifePoints <= 20) {
		self->state = PLAYER_DYING;
	}
	if (self->lifePoints <= 0) {
		self->state = PLAYER_DEAD;
		self->playerDead();
	}
}

void Player_Ship_Collision(Player *self, Enemy *e)
{
	printf("Collision detected!\n");
	//Generates damages to both the ship and the player's one.
	Enemy_Damage(e, 30);
	Player_Damage(self, 30, NULL);
	self->playerCollision();
}
