#include "Portal.h"
#include "Enemy.h"
#include "Common.h"
#include "Scene.h"

Portal* Portal_New(Scene* scene, int type, void* linked, Vec2 position)
{
    Portal* self = (Portal*)calloc(1, sizeof(Portal));
    AssertNew(self);

    self->scene = scene;
    self->position = position;
    self->linked_portal = linked;

    Assets* assets = Scene_GetAssets(self->scene);

    switch (type)
    {
    case RED_PORTAL:
        self->texture = assets->portal[1];
        self->radius = 0.4f;
        break;

    default :
    BLUE_PORTAL:
        self->texture = assets->portal[0];
        self->radius = 0.4f;

    return self;
}
