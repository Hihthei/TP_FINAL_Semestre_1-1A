#pragma once

#include "Settings.h"

#define ANIM_TEXTURES_MAX 8
#define SCENE_ANIM_NUM_MAX 16


typedef struct TextureSpec_s
{
	SDL_Texture **ptr;
	const char *path;
} TextureSpec;


typedef struct Animation_t {
	SDL_Texture *textures[ANIM_TEXTURES_MAX];
	int current;
} Animation;


/// @brief Structure contenant l'ensemble des assets du jeu.
/// Cela correspond aux ressources utilisées (textures, musiques, sons...)
typedef struct Assets_s
{
    /// @brief Tableau des différents calques de fond.
    SDL_Texture *layers[2];

    /// @brief Texture du vaisseau du joueur.
    SDL_Texture *player;

    /// @brief Texture du tir du joueur.
    SDL_Texture *playerBullet;

    /// @brief Texture du vaisseau ennemi.
    SDL_Texture *fighter;

    /// @brief Texture du tir d'un ennemi.
    SDL_Texture *fighterBullet;

	/// @brief Background of the start menu.
	SDL_Texture *startScreen;

	/// @brief Blinking message on the start screen.
	SDL_Texture *startScreenMessage;

	/// @brief Background of the loser's menu.
	SDL_Texture *lostScreen;

	/// @brief Blinking message on the loser's screen.
	SDL_Texture *lostScreenMessage;

	///@brief Total anims available in the assets.
	Animation *animations[SCENE_ANIM_NUM_MAX];
	TextureSpec animationsToLoad[SCENE_ANIM_NUM_MAX][ANIM_TEXTURES_MAX];
} Assets;

/// @brief Crée la structure contenant les assets du jeu.
/// @param renderer le moteur de rendu.
/// @return La structure contenant les assets du jeu.
Assets *Assets_New();

/// @brief Loads required assets and anims into memory.
void Assets_Load(Assets *self, SDL_Renderer *renderer);

/// @brief Détruit la structure contenant les assets du jeu.
/// @param self les assets.
void Assets_Delete(Assets *self);

/// @brief Create a new animation instance.
Animation *animation_new();

/// @brief Delete an Animation instance.
void animation_delete(Animation *self);

/// @brief Get the next frame of an animation.
SDL_Texture *animation_get_next(Animation *self);
