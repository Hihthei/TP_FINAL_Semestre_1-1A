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

void mixer_load(Mixer *self)
{
	const char *sources[] = {
	};

	int length = sizeof(sources)/sizeof(const char *);
	for (int i = 0; i < length && i < MAX_SOUNDS; i++) {
		self->sounds[i] = Mix_LoadWAV(sources[i]);
	}
}

void mixer_set_volume(Mixer *self, int type, int val)
{
	if (!self || type < 0 || type > MAX_SOUNDS) {
		return;
	}

	Mix_Volume(type, val);
}

void mixer_play_music(Mixer *self, int type, int duration, enum SoundSource src)
{
	if (!self || type < 0 || type > MAX_SOUNDS) {
		return;
	}

	switch (src) {
	case FighterSource:
		Mix_PlayChannel(type, self->sounds[type], duration);
		break;
	case PlayerSource:
		Mix_PlayChannel(type, self->sounds[type], duration);
		break;
	case OtherSource:
		Mix_PlayChannel(type, self->sounds[type], duration);
		break;
	default:
		break;
	}

}
