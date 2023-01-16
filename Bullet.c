#include "Bullet.h"
#include "Common.h"
#include "Scene.h"


void Bullet_Update_impl(Bullet *, void **, bool);
void Bullet_Update_pos_impl(Vec2 *v, const Bullet *, void **, bool);

Bullet *Bullet_New(Scene *scene, Vec2 position, Vec2 velocity, int type, float angle)
{
    Bullet *self = (Bullet *)calloc(1, sizeof(Bullet));
    AssertNew(self);

    self->position = position;
    self->velocity = velocity;
    self->type = type;
    self->angle = angle;
    self->scene = scene;
	self->fromPlayer = false;

	self->update = &Bullet_Update_impl;
	self->updatePos = &Bullet_Update_pos_impl;

	memset(&self->_data[0], 0, sizeof(void *)*2);

	Assets *assets = Scene_GetAssets(scene);

	//fighter_bullet.png
    switch (type)
    {
    case BULLET_FIGHTER:
        self->texture = assets->fighterBullet;
        self->worldW = 8 * PIX_TO_WORLD;
        self->worldH = 16 * PIX_TO_WORLD;
        self->radius = 0.05f;
        self->fromPlayer = false;
        break;

    default:
    case BULLET_PLAYER:
        self->texture = assets->playerBullet;
        self->worldW = 8 * PIX_TO_WORLD;
        self->worldH = 16 * PIX_TO_WORLD;
        self->radius = 0.05f;
        self->fromPlayer = true;
        break;
    }

    return self;
}

void Bullet_Delete(Bullet *self)
{
    if (!self) return;
	self->update(NULL, &self->_data[0], true);
	self->update(NULL, &self->_data[1], true);
    free(self);
}

void Bullet_Update_pos_impl(Vec2 *v, const Bullet *self, void **d, bool destroy)
{
	UNUSED(d);
	if (destroy) {
		return;
	}

	// Mise à jour de la position
	(*v) = Vec2_Add(self->position, Vec2_Scale(self->velocity, Timer_GetDelta(g_time)));
}

void Bullet_Update_impl(Bullet *self, void **d, bool destroy)
{
	UNUSED(d);
	if (destroy) {
		return;
	}

	self->updatePos(&self->position, self, &self->_data[1], false);
}

void Bullet_Update(Bullet *self)
{
	// On récupère des infos essentielles (communes à tout objet)
	//Scene *scene = self->scene;
	self->update(self, &self->_data[0], false);
}

void Bullet_Render(Bullet *self)
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
