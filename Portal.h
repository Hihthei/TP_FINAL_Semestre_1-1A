#pragma once

#include "Scene.h"
#include "patterns.h"

typedef struct Scene_s Scene;

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
    void* linked_portal;

}Portal;