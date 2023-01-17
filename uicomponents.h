#pragma once

#include "Scene.h"
#include "Math.h"

typedef struct UiElement_t
{
	Vec2 position;
	Vec2 size;

	void (*render)(struct UiElement_t *self, Scene *s);
	void (*destroy)(struct UiElement_t *self);
	void (*update)(struct UiElement_t *self, Scene *s);
} UiElement;

typedef struct LifeBar_t
{
	UiElement base;
	int lifePoints;
} LifeBar;

typedef struct Overlay_t
{
	UiElement base;
	SDL_Color color;
} Overlay;

typedef struct Label_t
{
	UiElement base;
	const char *text;
	SDL_Texture *message;
	SDL_Surface *surfaceMessage;
} Label;

UiElement *ui_element_LifeBar_new();
UiElement *ui_element_Overlay_new();
UiElement *ui_element_Label_new(const char *text, Scene *scene);

void ui_element_delete(UiElement *self);
void ui_element_render(UiElement *self, Scene *s);
void ui_element_update(UiElement *self, Scene *s);
void ui_element_init(UiElement *self);