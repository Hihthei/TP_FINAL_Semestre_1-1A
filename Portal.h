#pragma once

#include "Scene.h"
#include "patterns.h"

typedef struct Scene_s Scene;

//cr�ation d'un objet protail

typedef struct Portal_s
{
    /// @brief Sc�ne associ�e.
    Scene* scene;

    /// @brief Texture utilis�e pour le rendu.
    SDL_Texture* texture;

    /// @brief Position de l'ennemi exprim�e dans le r�f�rentiel monde.
    Vec2 position;

    /// \Initial position
    Vec2 position_initial;

    /// @brief Largeur de la texture du portail
    /// exprim�e dans le r�f�rentiel monde.
    float worldW;

    /// @brief Hauteur de la texture du portail
    /// exprim�e dans le r�f�rentiel monde.
    float worldH;

    /// @brief Rayon du portal exprim� dans le r�f�rentiel monde.
    /// Il est utilis� dans le moteur physique pour tester les collisions.
    float radius;

    /// @brief Pointeur vers le portail associ�
    void* linked_portal;

}Portal;