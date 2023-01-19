#include "Assets.h"

Assets *Assets_New()
{
    Assets *self = (Assets *)calloc(1, sizeof(Assets));
    AssertNew(self);

	self->layers[0] = NULL;
	self->layers[1] = NULL;
	self->player = NULL;
	self->playerBullet = NULL;
	self->fighter = NULL;
	self->fighterBullet = NULL;

	memset(&self->animations[0], 0, sizeof(Animation *)*SCENE_ANIM_NUM_MAX);
	memset(&self->animationsToLoad[0], 0, sizeof(TextureSpec)*SCENE_ANIM_NUM_MAX*8);

    return self;
}

void Assets_Load(Assets *self, SDL_Renderer *renderer)
{
	// -------------------------------------------------------------------------
	// Chargement des textures

	TextureSpec texSpecs[] = {
		{ &self->layers[0],     "../Assets/Background/layer_01.png"   },
		{ &self->layers[1],     "../Assets/Background/layer_02.png"   },
		{ &self->player,		"../Assets/Player/player.png"		  },
		{ &self->playerBullet,  "../Assets/Player/bullet_default.png" },
		{ &self->fighter,       "../Assets/Enemy/fighter.png"         },
		{ &self->fighterBullet, "../Assets/Enemy/fighter_bullet.png"  },
		{ &self->portal[0],		"../Assets/Portal/blue_portal.png"	  },
		{ & self->portal[1],		"../Assets/Portal/red_portal.png" },
		{ &self->startScreen,   "../Assets/Background/open_screen.png"},
		{ &self->startScreenMessage, "../Assets/Background/texte_open_screen.png"},
		{ &self->lostScreen,    "../Assets/Background/dead_screen.png"},
		{ &self->lostScreenMessage, "../Assets/Background/texte_dead_screen.png"},
		{ &self->healDrop, "../Assets/Enemy/fighter_bullet_health.png"},
		{ &self->playerSpecialBullet, "../Assets/Player/bullet_special.png"},
		
	};
	int texSpecCount = sizeof(texSpecs) / sizeof(TextureSpec);

	for (int i = 0; i < texSpecCount; i++)
	{
		SDL_Texture **texPtr = texSpecs[i].ptr;
		const char *path = texSpecs[i].path;

		*texPtr = IMG_LoadTexture(renderer, path);
		if (*texPtr == NULL)
		{
			printf("ERROR - Loading texture %s\n", path);
			printf("      - %s\n", SDL_GetError());
			assert(false);
			abort();
		}
	}

	int i = 0;
	while (self->animationsToLoad[i][0].path) {
		int j = 0;
		self->animations[i] = animation_new();
		while (self->animationsToLoad[i][j].path && j < ANIM_TEXTURES_MAX) {

			SDL_Texture **texPtr = self->animationsToLoad[i][j].ptr;
			const char *path = self->animationsToLoad[i][j].path;

			(*texPtr) = IMG_LoadTexture(renderer, path);
			if (*texPtr == NULL)
			{
				printf("ERROR - Loading texture %s\n", path);
				printf("      - %s\n", SDL_GetError());
				assert(false);
				abort();
			}

			self->animations[i]->textures[j] = (*texPtr);
			j++;
		}
		i++;
	}
}

void Assets_Delete(Assets *self)
{
    if (!self) return;

    // -------------------------------------------------------------------------
    // Libère les textures

    SDL_Texture **texPointers[] = {
        &self->layers[0],
        &self->layers[1],
        &self->playerBullet,
        &self->fighter,
        &self->fighterBullet,
		& self->portal[0],
		& self->portal[1],
		&self->player,
		&self->startScreen,
		&self->startScreenMessage,
		&self->lostScreen,
		&self->lostScreenMessage,
    };
    int count = sizeof(texPointers) / sizeof(SDL_Texture **);

    for (int i = 0; i < count; i++)
    {
        if (*texPointers[i])
            SDL_DestroyTexture(*(texPointers[i]));
    }

	int i = 0;
	while (self->animations[i]) {
		animation_delete(self->animations[i]);
		i++;
	}

    free(self);
}

Animation *animation_new()
{
	Animation *self = (Animation *)malloc(sizeof(Animation));
	memset(&self->textures[0], 0, sizeof(Animation));
	return self;
}

void animation_delete(Animation *self)
{
	if (!self) {
		return;
	}

	int i = 0;
	while (self->textures[i] && i < ANIM_TEXTURES_MAX) {
		SDL_DestroyTexture(self->textures[i]);
		i++;
	}

	free(self);
}

SDL_Texture *animation_get_next(Animation *self)
{
	if (!self) {
		return NULL;
	}

	if (!self->textures[self->current +1]) {
		self->current = 0;
	} else {
		self->current++;
	}
	return self->textures[self->current];
}

