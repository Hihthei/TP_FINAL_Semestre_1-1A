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
    struct Portal_s* linked_portal;

}Portal;

/// @brief Cr�er un portail et l'ajoute � la sc�ne via la m�thode Scene_AppendBullet();
/// @param scene la sc�ne.
/// @param type le type du projectile.
/// @param lindek le portail auquel il est li�
/// @param position la position de d�part du portail exprim�e dans le r�f�rentiel monde.
/// @return Le projectile cr��.
Portal* Portal_New(Scene* scene, int type, void* linked, Vec2 position);