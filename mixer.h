#pragma once

#include "Settings.h"

#define MAX_SOUNDS 6

enum SoundType
{
	Background,
	Destruction,
	Apparition,
	FighterShoot,
	PlayerShoot,
	Crisis,
	Lost
};

enum SoundSource
{
	FighterSource,
	PlayerSource,
	OtherSource
};

typedef struct Mixer_t
{
	Mix_Chunk *sounds[MAX_SOUNDS];
} Mixer;

/// @brief Create a new sound mixer instance.
Mixer *mixer_new();

/// @brief Delete an existing sound mixer instance.
/// @param self the sound mixer.
void mixer_delete(Mixer *self);

/// @brief Load sound mixer's data.
/// @param self the mixer.
void mixer_load(Mixer *self);

/// @brief Change the volume for a type of sound (channel).
void mixer_set_volume(Mixer *self, int soundType, int value);

/// @brief Plays the music you want to!
void mixer_play_music(Mixer *self, int soundType, int duration, enum SoundSource source);
