
#include "Settings.h"
#include "Common.h"
#include "Timer.h"
#include "Scene.h"
#include "uicomponents.h"
#include "Math.h"
#include "levels.h"

int main(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);

    //--------------------------------------------------------------------------
    // Initialisation

    // Initialise la SDL
    Game_Init(SDL_INIT_VIDEO, IMG_INIT_PNG);

    // Crée la fenêtre
    int sdlFlags = 0;
#ifdef FULLSCREEN
    sdlFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif
    SDL_Window *window = SDL_CreateWindow(
        u8"Shoot'Em Up", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH, WINDOW_HEIGHT, sdlFlags
    );

    if (!window)
    {
        printf("ERROR - Create window %s\n", SDL_GetError());
        assert(false); abort();
    }

    // Crée le moteur de rendu
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (!renderer)
    {
        printf("ERROR - Create renderer %s\n", SDL_GetError());
        assert(false); abort();
    }

    // Crée le temps global du jeu
    g_time = Timer_New();
    AssertNew(g_time);

    //--------------------------------------------------------------------------
    // Boucle de rendu

	Scene *scene = Scene_New(renderer);
	//Loads assets with the attached scene.
	Assets_Load(scene->assets, scene->renderer);
	//Add the levels to the scene.
	scene->waves[0] = &craft_level_1;
	scene->waves[1] = &craft_level_1_boss;
	scene->waves[2] = &craft_level_2;
	scene->waves[3] = &craft_level_2_boss;
	scene->waves[4] = &craft_level_3;
	scene->waves[5] = &craft_level_4;
	scene->waves[6] = &craft_level_4_5;
	scene->waves[7] = &craft_level_4_boss;

	//Play the background music.
	mixer_play_music(scene->mixer, MenuSound, -1);

	bool no_quit = true;
	//Show the menu.

	UiElement *button = ui_element_BlinkingButton_new();
	button->size = Vec2_Set(9, 16);
	button->position = Vec2_Set(0, 0);
	((Button *)button)->image = scene->assets->startScreen;
	((BlinkingButton *)button)->blinking = scene->assets->startScreenMessage;
	Scene_AddUiElement(scene, button);

	scene->ui_mode = true;
	while (true)
	{
		// Met à jour le temps
		Timer_Update(g_time);
		//printf("%f FPS\n", 60/Timer_GetElapsed(g_time));

		// Met à jour la scène

		bool quitLoop = Scene_Update(scene);
		if (quitLoop) {
			no_quit = false;
			break;
		}
		if (((Button *)button)->clicked) {
			break;
		}

		// Efface le rendu précédent
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		// Dessine la scène
		Scene_Render(scene);

		// Affiche le nouveau rendu
		SDL_RenderPresent(renderer);
	}

load_game:

	scene->ui_mode = false;
	Scene_RemoveUiElement(scene, button);
	Scene_Load(scene);
	Scene_SetWaveIndex(scene, 0);
	mixer_play_music(scene->mixer, BackgroundSound, -1);

	//Show the game if required.
	if (no_quit) {
		UiElement *lb = ui_element_LifeBar_new();
		((LifeBar *)lb)->base.size = Vec2_Set(1.4f, 0.1f);
		Scene_AddUiElement(scene, (UiElement *)lb);

		while (true)
		{
			// Met à jour le temps
			Timer_Update(g_time);
			//printf("%f FPS\n", 60/Timer_GetElapsed(g_time));

			// Met à jour la scène
			bool quitLoop = Scene_Update(scene);
			if (quitLoop)
				break;

			// Efface le rendu précédent
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			// Dessine la scène
			Scene_Render(scene);

			// Affiche le nouveau rendu
			SDL_RenderPresent(renderer);
		}

		//Clean the scene.
		int playerState = scene->player->state;
		Scene_Unload(scene);
		Scene_RemoveAllUiElements(scene);
		scene->ui_mode = true;

		//Display new data.
		button = ui_element_BlinkingButton_new();
		button->size = Vec2_Set(9, 16);
		button->position = Vec2_Set(0, 0);
		Scene_AddUiElement(scene, button);

		//Change the texture.
		no_quit = true;
		if (playerState == PLAYER_DEAD) {
			((Button *)button)->image = scene->assets->lostScreen;
			((BlinkingButton *)button)->blinking = scene->assets->lostScreenMessage;
			mixer_play_music(scene->mixer, LostSound, -1);
		} else {
			//((Button *)button)->image = scene.;
			mixer_play_music(scene->mixer, WonSound, -1);
		}

		while (true)
		{
			// Met à jour le temps
			Timer_Update(g_time);
			//printf("%f FPS\n", 60/Timer_GetElapsed(g_time));

			// Met à jour la scène
			bool quitLoop = Scene_Update(scene);
			if (quitLoop) {
				no_quit = false;
				break;
			}
			if (((Button *)button)->clicked) {
				break;
			}

			// Efface le rendu précédent
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			// Dessine la scène
			Scene_Render(scene);

			// Affiche le nouveau rendu
			SDL_RenderPresent(renderer);
		}
		if (no_quit) {
			goto load_game;
		}
	}

    //--------------------------------------------------------------------------
    // Libération de la mémoire

    Scene_Delete(scene);
    scene = NULL;

    Timer_Delete(g_time);
    g_time = NULL;

    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    Game_Quit();

    return EXIT_SUCCESS;
}
