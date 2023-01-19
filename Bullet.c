#include "Bullet.h"
#include "Common.h"
#include "Scene.h"


void Bullet_Update_impl(Bullet *, PatternData *);
void Bullet_Update_pos_impl(Bullet *, PatternData *);

Bullet *Bullet_New(Scene *scene, Vec2 position, Vec2 velocity, int type, float angle, int damages)
{
    Bullet *self = (Bullet *)calloc(1, sizeof(Bullet));
    AssertNew(self);

    self->position = position;
    self->velocity = velocity;
    self->type = type;
    self->angle = angle;
    self->scene = scene;
	self->fromPlayer = false;
	self->damages = damages;

	self->update = &Bullet_Update_impl;
	self->updatePos = &Bullet_Update_pos_impl;

	memset(&self->_data[0], 0, sizeof(PatternData)*2);

	Assets *assets = Scene_GetAssets(scene);


	self->worldW = 8 * PIX_TO_WORLD;
	self->worldH = 16 * PIX_TO_WORLD;


	//fighter_bullet.png
    switch (type)
    {
    case BULLET_FIGHTER:
		self->texture = assets->fighterBullet;
        self->radius = 0.05f;
        self->fromPlayer = false;
        break;

	case BULLET_EXPLOSION:
		self->texture = scene->assets->explosion;
		self->radius = 0.02f;
		self->fromPlayer = false;
		break;

    default:
    case BULLET_PLAYER:
		self->texture = assets->playerBullet;
        self->radius = 0.05f;
        self->fromPlayer = true;
        break;
    }

    return self;
}

Bullet* Explosion_New(Scene* scene, Vec2 position)
{
	Bullet* exp = Bullet_New(scene, position, Vec2_Zero, BULLET_EXPLOSION, 0, 0);
	exp->updatePos = &bullet_auto_depop_pattern;
	return exp;
}

void Bullet_Delete(Bullet *self)
{
    if (!self) return;

	invalidate_patterns_data(&self->_data[0], 2);
	self->update(NULL, &self->_data[0]);
	self->update(NULL, &self->_data[1]);

    free(self);
}

void Bullet_Update_pos_impl(Bullet *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	// Mise à jour de la position
	self->position = Vec2_Add(self->position, Vec2_Scale(self->velocity, Timer_GetDelta(g_time)));
}

void Bullet_Update_impl(Bullet *self, PatternData *d)
{
	if (d->destroy) {
		return;
	}

	self->updatePos(self, &self->_data[1]);
}

void Bullet_Update(Bullet *self)
{
	// On récupère des infos essentielles (communes à tout objet)
	//Scene *scene = self->scene;
	self->update(self, &self->_data[0]);
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
	dst.h = 15 * PIX_TO_WORLD * scale;
	dst.w = 15 * PIX_TO_WORLD * scale;
	Camera_WorldToView(camera, self->position, &dst.x, &dst.y);
	// Le point de référence est le centre de l'objet
	dst.x -= 0.50f * dst.w;
	dst.y -= 0.50f * dst.h;
	// On affiche en position dst (unités en pixels)
	SDL_RenderCopyExF(renderer, self->texture, NULL, &dst, self->angle, NULL, 0);
}
