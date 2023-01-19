#include "uicomponents.h"

#include "Settings.h"


void ui_element_delete(UiElement *e)
{
	if (!e) {
		return;
	}

	e->destroy(e);
	free(e);
}

void ui_element_render(UiElement *e, Scene *s)
{
	if (!e || !s) {
		return;
	}

	int b;
	int *a = &b;
	*a = 0;

	e->render(e, s);
}

void ui_element_update(UiElement *e, Scene *s)
{
	if (!e || !s) {
		return;
	}
	e->update(e, s);
}

void ui_element_init(UiElement *e)
{
	if (!e) {
		return;
	}
	e->size = Vec2_Set(0, 0);
	e->position = Vec2_Set(0, 0);
}

void ui_LifeBar_update(LifeBar *b, Scene *scene)
{
	b->lifePoints = scene->player->lifePoints;
	b->base.position = Vec2_Add(Vec2_Set(-scene->player->radius-0.5, -scene->player->radius - 0.5), scene->player->position);
}

void ui_Overlay_update(Overlay *b, Scene *s)
{
	UNUSED(b);
	UNUSED(s);
}

void ui_Button_update(Button *self, Scene *scene)
{
	Input *input = Scene_GetInput(scene);
	if (input->shootPressed) {
		self->clicked = true;
	} else {
		self->clicked = false;
	}
}

void ui_BlinkingButton_update(BlinkingButton *self, Scene *scene)
{
	ui_Button_update((Button *)self, scene);
	self->time += Timer_GetDelta(g_time);
}

void ui_LifeBar_render(LifeBar *self, Scene *scene)
{
	SDL_Renderer *renderer = Scene_GetRenderer(scene);
	Camera *camera = Scene_GetCamera(scene);

	// On calcule la position en pixels en fonction de la position
	// en tuiles, la taille de la fenêtre et la taille des textures.
	float scale = Camera_GetWorldToViewScale(camera);
	SDL_FRect dst = { 0 };

	dst.h = self->base.size.y * scale;
	dst.w = self->base.size.x * scale;
	Camera_WorldToView(camera, self->base.position, &dst.x, &dst.y);

	SDL_Rect rect = {(int)dst.x, (int)dst.y, (int)dst.w, (int)dst.h};

	// Fill in the rectangle's BG
	SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
	SDL_RenderFillRect (renderer, &rect);

	//Fill with remaining life
	dst.h = self->base.size.y * scale;
	dst.w = self->base.size.x * scale * self->lifePoints / 100;

	Camera_WorldToView(camera, self->base.position, &dst.x, &dst.y);

	SDL_Rect rect2 = {(int)dst.x, (int)dst.y, (int)dst.w, (int)dst.h};

	if (scene->player->state == PLAYER_DYING) {
		SDL_SetRenderDrawColor (renderer, 255, 10, 10, 255);
	} else {
		SDL_SetRenderDrawColor (renderer, 10, 255, 10, 255);
	}
	SDL_RenderFillRect (renderer, &rect2);

	//Draw the edges of the rectangle
	SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect (renderer, &rect);
}

void ui_Overlay_render(Overlay *self, Scene *scene)
{
	SDL_Renderer *renderer = Scene_GetRenderer(scene);
	Camera *camera = Scene_GetCamera(scene);
	// On calcule la position en pixels en fonction de la position
	// en tuiles, la taille de la fenêtre et la taille des textures.
	float scale = Camera_GetWorldToViewScale(camera);
	SDL_FRect dst = { 0 };
	dst.h = self->base.size.x * scale;
	dst.w = self->base.size.y * scale;
	Camera_WorldToView(camera, self->base.position, &dst.x, &dst.y);
	dst.x = self->base.position.x * scale;
	dst.y = self->base.position.y * scale;

	SDL_Rect rect = {(int)dst.x, (int)dst.y, (int)dst.w, (int)dst.h};

	SDL_SetRenderDrawColor(renderer, self->color.r, self->color.g, self->color.b, self->color.a);
	SDL_RenderDrawRect(renderer, &rect);
}

void ui_Button_render(Button *self, Scene *scene)
{
	if (self->image) {
		SDL_Renderer *renderer = Scene_GetRenderer(scene);
		Camera *camera = Scene_GetCamera(scene);
		// On calcule la position en pixels en fonction de la position
		// en tuiles, la taille de la fenêtre et la taille des textures.
		float scale = Camera_GetWorldToViewScale(camera);
		SDL_FRect dst = { 0 };
		dst.h = self->base.size.x * scale;
		dst.w = self->base.size.y * scale;
		Camera_WorldToView(camera, self->base.position, &dst.x, &dst.y);
		dst.x = self->base.position.x * scale;
		dst.y = self->base.position.y * scale;

		SDL_RenderCopyExF(renderer, self->image, NULL, &dst, 0, NULL, 0);
	}
}

void ui_BlinkingButton_render(BlinkingButton *self, Scene *scene)
{
	if (self->base.image) {
		SDL_Renderer *renderer = Scene_GetRenderer(scene);
		Camera *camera = Scene_GetCamera(scene);
		// On calcule la position en pixels en fonction de la position
		// en tuiles, la taille de la fenêtre et la taille des textures.
		float scale = Camera_GetWorldToViewScale(camera);
		SDL_FRect dst = { 0 };
		dst.h = self->base.base.size.x * scale;
		dst.w = self->base.base.size.y * scale;
		Camera_WorldToView(camera, self->base.base.position, &dst.x, &dst.y);

		dst.x = self->base.base.position.x * scale;
		dst.y = self->base.base.position.y * scale;

		SDL_RenderCopyExF(renderer, self->base.image, NULL, &dst, 0, NULL, 0);

		if (self->blinking && self->time >= 0.5) {
			if (self->time >= 2) {
				self->time = 0;
				return;
			}
			SDL_RenderCopyExF(renderer, self->blinking, NULL, &dst, 0, NULL, 0);
		}
	}
}

void ui_LifeBar_destroy(LifeBar *b)
{
	UNUSED(b);
}

void ui_Overlay_destroy(Overlay *o)
{
	UNUSED(o);
}

void ui_Button_destroy(Button *btn)
{
	UNUSED(btn);
}

void ui_BlinkingButton_destroy(BlinkingButton *bb)
{
	UNUSED(bb);
}

UiElement *ui_element_Overlay_new()
{
	Overlay *o = (Overlay *)malloc(sizeof(Overlay));
	memset(o, 0, sizeof(Overlay));
	ui_element_init((UiElement *)o);

	SDL_Color c = {0, 0, 0, 200};
	o->color = c;

	o->base.destroy = (void (*)(UiElement *)) &ui_Overlay_destroy;
	o->base.render = (void (*)(UiElement *, Scene *)) &ui_Overlay_render;
	o->base.update = (void (*)(UiElement *, Scene *)) &ui_Overlay_update;

	return (UiElement *)o;
}

UiElement *ui_element_LifeBar_new()
{
	LifeBar *b = (LifeBar *)malloc(sizeof(LifeBar));
	memset(b, 0, sizeof(LifeBar));
	ui_element_init((UiElement *)b);

	b->lifePoints = 0;

	b->base.destroy = (void (*)(UiElement *)) &ui_LifeBar_destroy;
	b->base.render = (void (*)(UiElement *, Scene *)) &ui_LifeBar_render;
	b->base.update = (void (*)(UiElement *, Scene *)) &ui_LifeBar_update;

	return (UiElement *)b;
}

void ui_element_Button_init(Button *btn)
{
	ui_element_init((UiElement *)btn);

	btn->clicked = false;
	btn->image = NULL;

	btn->base.destroy = (void (*)(UiElement *)) &ui_Button_destroy;
	btn->base.render = (void (*)(UiElement *, Scene *)) &ui_Button_render;
	btn->base.update = (void (*)(UiElement *, Scene *)) &ui_Button_update;
}

UiElement *ui_element_Button_new()
{
	Button *btn = (Button *)malloc(sizeof(Button));
	memset(btn, 0, sizeof(Button));
	ui_element_Button_init(btn);

	return (UiElement *)btn;
}

UiElement *ui_element_BlinkingButton_new()
{
	BlinkingButton *bb = (BlinkingButton *)malloc(sizeof(BlinkingButton));
	memset(bb, 0, sizeof(BlinkingButton));
	ui_element_Button_init((Button *)bb);

	bb->blinking = NULL;
	bb->time = 0;

	((UiElement *)bb)->destroy = (void (*)(UiElement *)) &ui_BlinkingButton_destroy;
	((UiElement *)bb)->render = (void (*)(UiElement *, Scene *)) &ui_BlinkingButton_render;
	((UiElement *)bb)->update = (void (*)(UiElement *, Scene *)) &ui_BlinkingButton_update;

	return (UiElement *)bb;
}
