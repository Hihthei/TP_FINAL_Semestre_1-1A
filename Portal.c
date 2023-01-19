#include "Portal.h"
#include "Enemy.h"
#include "Common.h"
#include "Scene.h"

Portal* Portal_New(Scene* scene, void* linked, Vec2 position)
{
    Portal* self = (Portal*)calloc(1, sizeof(Portal));
    AssertNew(self);

    self->scene = scene;
    self->position = position;
    self->linked_portal = linked;

    Assets* assets = Scene_GetAssets(self->scene);

    return self;
}
