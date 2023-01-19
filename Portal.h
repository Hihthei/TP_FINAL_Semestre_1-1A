#pragma once

#include "Scene.h"
#include "patterns.h"

typedef struct Scene_s Scene;

typedef enum PortalType_e
{
    /// @brief Portail rouge
    RED_PORTAL,

    /// @brief Portail bleu
    BLUE_PORTAL,

} PortalType;

//création d'un objet protail

typedef struct Portal_s
{
    /// @brief Scène associée.
    Scene* scene;

    /// @brief Texture utilisée pour le rendu.
    SDL_Texture* texture;

    /// @brief Position de l'ennemi exprimée dans le référentiel monde.
    Vec2 position;

    /// \Initial position
    Vec2 position_initial;

    /// @brief Largeur de la texture du portail
    /// exprimée dans le référentiel monde.
    float worldW;

    /// @brief Hauteur de la texture du portail
    /// exprimée dans le référentiel monde.
    float worldH;

    /// @brief Rayon du portal exprimé dans le référentiel monde.
    /// Il est utilisé dans le moteur physique pour tester les collisions.
    float radius;

    /// @brief Pointeur vers le portail associé
    struct Portal_s* linked_portal;

}Portal;

/// @brief Créer un portail et l'ajoute à la scène via la méthode Scene_AppendBullet();
/// @param scene la scène.
/// @param type le type du projectile.
/// @param lindek le portail auquel il est lié
/// @param position la position de départ du portail exprimée dans le référentiel monde.
/// @return Le projectile créé.
Portal* Portal_New(Scene* scene, int type, void* linked, Vec2 position);