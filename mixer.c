#include "mixer.h"

Mixer *mixer_new()
{
	Mixer *mx = (Mixer *)malloc(sizeof(Mixer));
	memset(mx, 0, sizeof(Mixer));
	mixer_load(mx);

	return mx;
}

void mixer_delete(Mixer *self)
{
	if (!self) {
		return;
	}

	for (int i = 0; i < MAX_SOUNDS; i++) {
		if (self->sounds[i]) {
			Mix_FreeChunk(self->sounds[i]);
		}
	}

	free(self);
}

struct SoundSet_t
{
	enum SoundType type;
	const char *path;
};

void mixer_load(Mixer *self)
{
	struct SoundSet_t sources[] = {
	{BackgroundSound, "../Assets/Music/background.wav"},
	{DestructionSound, "../Assets/Music/explosion.wav"},
	{ApparitionSound, "../Assets/Music/apparition.wav"},
	{CollisionSound, "../Assets/Music/explosion.wav"},
	{PlayerShotSound, "../Assets/Music/base_piou.wav"},
	{PlayerSpecialShotSound, "../Assets/Music/special_attack.wav"},
	{FighterShotSound, "../Assets/Music/base_piou.wav"},
	};

	int length = sizeof(sources)/sizeof(struct SoundSet_t);
	for (int i = 0; i < length && i < MAX_SOUNDS; i++) {
		Mix_Chunk *chk = Mix_LoadWAV(sources[i].path);
		if (chk) {
			self->sounds[sources[i].type] = chk;
		}
	}
}

void mixer_set_volume(Mixer *self, int type, int val)
{
	if (!self || type < 0 || type > MAX_SOUNDS) {
		return;
	}

	Mix_Volume(type, val);
}

void mixer_play_music(Mixer *self, int type, int duration)
{
	if (!self || type < 0 || type > MAX_SOUNDS || !self->sounds[type]) {
		return;
	}

	int channel = type;
	if (type < 4) { //Because menu and background sound have to be on the same channel.
		channel = 0;
	} else {
		channel -= 3;
		if (type > 10) {
			channel -= 1;
		}
	}

	Mix_PlayChannel(channel, self->sounds[type], duration);
}
