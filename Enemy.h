#pragma once

#include "Settings.h"
#include "Math.h"
#include "patterns.h"

typedef struct Scene_s Scene;

/// @brief Enumération représentant les états possibles d'un ennemi.
typedef enum EnemyState_e
{
    /// @brief Etat par défaut d'un ennemi.
    ENEMY_FIRING,

    /// @brief Etat d'un ennemi quand il joue l'animation de destruction.
    ENEMY_DYING,

    /// @brief Etat d'un ennemi quand il est mort et que son animation de
    /// destruction est terminée.
    ENEMY_DEAD,
} EnemyState;

/// @brief Enumération des types possibles pour un ennemi.
typedef enum EnemyType_e
{
    /// @brief Ennemi simple.
    ENEMY_FIGHTER,
} EnemyType;

/// @brief Structure représentant un ennemi.
typedef struct Enemy_s
{
    /// @brief Scène associée.
    Scene *scene;

    /// @brief Texture utilisée pour le rendu.
    SDL_Texture *texture;

    /// @brief Position de l'ennemi exprimée dans le référentiel monde.
    Vec2 position;

    /// \Initial position
    Vec2 position_initial;

    /// @brief Type de l'ennemi.
    /// Les valeurs possibles sont définies dans EnemyType.
    int type;

    /// @brief Etat de l'ennemi.
    /// Les valeurs possibles sont définies dans EnemyState.
    int state;

    /// @brief Largeur de la texture de l'ennemi
    /// exprimée dans le référentiel monde.
    float worldW;

    /// @brief Hauteur de la texture de l'ennemi
    /// exprimée dans le référentiel monde.
    float worldH;

    /// @brief Rayon de l'ennemi exprimé dans le référentiel monde.
    /// Il est utilisé dans le moteur physique pour tester les collisions.
    float radius;

	/// \brief Remaining life points of the enemy.
	int lifePoints;

	/// \brief Last time this entity thrown an attack.
	float lastAttack;

	/// \brief Used to know if we have to call enemyRaisedOrDead.
	bool firstUpdate;

	/// @brief Damages caused by the ship when colliding with the player's one.
	int collisionDamages;

	//For these funcs, when implemented, only the last argument is guranteed
	//to be non-null when PatternData::destroy == true.
	void_enemy_func_ptr update;
	void_enemy_func_ptr updatePos;
	void_enemy_func_ptr throwAttack;
	void_enemy_func_ptr enemyRaisedOrDead; //This function guarantees that first argument is non-null.
	PatternData _data[4];
} Enemy;

/// @brief Crée un nouvel ennemi.
/// @param scene la scène.
/// @param type le type de l'ennemi.
/// @param position la position de départ de l'ennemi exprimée dans le référentiel monde.
/// @param read function to call on instanciation and deletion.
/// @return L'ennemi créé.
Enemy *Enemy_New(Scene *scene, int type, Vec2 position);

/// @brief Détruit un ennemi.
/// Cette méthode est appelée par la scène.
/// @param self l'ennemi.
void Enemy_Delete(Enemy *self);

/// @brief Met à jour un ennemi.
/// @param self l'ennemi.
void Enemy_Update(Enemy *self);

/// @brief Dessine un ennemi dans le moteur de rendu.
/// @param self l'ennemi.
void Enemy_Render(Enemy *self);

/// @brief Inflige des dommages à un ennemi.
/// @param self l'ennemi.
/// @param damage la quantité de dommages (nombre de points de vie à perdre).
void Enemy_Damage(Enemy *self, int damage);
