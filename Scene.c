#include "Scene.h"

#include "patterns.h"


Scene *Scene_New(SDL_Renderer *renderer)
{
    Scene *self = (Scene *)calloc(1, sizeof(Scene));
    AssertNew(self);

	self->renderer = renderer;

	self->assets = Assets_New();
    self->camera = Camera_New(LOGICAL_WIDTH, LOGICAL_HEIGHT);
	self->input = Input_New();
	self->mixer = mixer_new();
    self->waveIdx = 0;

	memset(self->waves, 0, sizeof(void_scene_level_func_ptr)*WAVES_CAPACITY);

    return self;
}

void Scene_Load(Scene *self)
{
	self->player = Player_New(self);
}

void Scene_Unload(Scene *self)
{
	for (int i = 0; i < self->enemyCount; i++)
	{
		Enemy_Delete(self->enemies[i]);
		self->enemies[i] = NULL;
	}
	self->enemyCount = 0;
	for (int i = 0; i < self->bulletCount; i++)
	{
		Scene_RemoveBullet(self, 0);
	}
	self->bulletCount = 0;
	if (self->player) {
		Player_Delete(self->player);
	}
	self->player = NULL;
}

void Scene_Delete(Scene *self)
{
    if (!self) return;

	for (int i = 0; i < self->enemyCount; i++)
	{
		Enemy_Delete(self->enemies[i]);
		self->enemies[i] = NULL;
	}
	for (int i = 0; i < self->bulletCount; i++)
	{
		Bullet_Delete(self->bullets[i]);
		self->bullets[i] = NULL;
	}
	for (int i = 0; i < self->uicCount; i++) {
		ui_element_delete(self->elements[i]);
		self->elements[i] = NULL;
	}

	if (self->player) {
		Player_Delete(self->player);
	}

	mixer_delete(self->mixer);
	Input_Delete(self->input);
	Camera_Delete(self->camera);
	Assets_Delete(self->assets);

    free(self);
}

void Scene_UpdateLevel(Scene *self)
{
	if (self->enemyCount > 0) {
        return;
	}

	//Loads the new level.
	if (self->waves[self->waveIdx]) {
		self->waves[self->waveIdx](self);
		self->waveIdx++;
	}
}

bool Scene_Update(Scene *self)
{
    Player *player = self->player;

    // Met � jour les entr�es utilisateur
    Input_Update(self->input);

    // -------------------------------------------------------------------------
    // Met � jour les tirs

	if (!self->ui_mode) {
		int i = 0;
		while (i < self->bulletCount)
		{
			Bullet *bullet = self->bullets[i];
			bool removed = false;

			Bullet_Update(bullet);

			bool outOfBounds =
				(bullet->position.x < -1.0f) ||
				(bullet->position.x > 17.0f) ||
				(bullet->position.y < -1.0f) ||
				(bullet->position.y > 10.0f);

			if (outOfBounds)
			{
				// Supprime le tir
				Scene_RemoveBullet(self, i);
				//removed = true;
				continue;
			}

			if (bullet->fromPlayer)
			{
				// Teste les collisions avec les ennemis
				for (int j = 0; j < self->enemyCount; j++)
				{
					Enemy *enemy = self->enemies[j];
					float dist = Vec2_Distance(bullet->position, enemy->position);
					if (dist < bullet->radius + enemy->radius)
					{
						// Inflige des dommages � l'ennemi
						Enemy_Damage(enemy, bullet->damages);

						// Supprime le tir
						Scene_RemoveBullet(self, i);
						removed = true;
						break;
					}
				}
			}
			else
			{
				// Teste la collision avec le joueur
				float dist = Vec2_Distance(bullet->position, self->player->position);
				if (dist < bullet->radius + player->radius)
				{
					// Inflige des dommages au joueur
					Player_Damage(player, bullet->damages, bullet);

					// Supprime le tir
					Scene_RemoveBullet(self, i);
					removed = true;
				}
			}

			// Passe au prochain tir
			if (removed == false)
			{
				i++;
			}
		}

		// -------------------------------------------------------------------------
		// Met � jour les ennemis

		i = 0;
		while (i < self->enemyCount)
		{
			Enemy *enemy = self->enemies[i];
			bool removed = false;

			Enemy_Update(enemy);

			//Look for a colision.
			if (Vec2_Distance(player->position, enemy->position) <= (enemy->radius + player->radius)) {
				//Trigger the collision.
				Player_Ship_Collision(player, enemy);
			}

			if (enemy->state == ENEMY_DEAD)
			{
				// Supprime l'ennemi
				Scene_RemoveEnemy(self, i);
				removed = true;
			}

			// Passe au prochain ennemi
			if (removed == false)
			{
				i++;
			}
		}

		// -------------------------------------------------------------------------
		// Met � jour le joueur

		Player_Update(self->player);

		// -------------------------------------------------------------------------
		// Met � jour le niveau

		Scene_UpdateLevel(self);
	}

	//Met ? jour l'interface.
	for (int i = 0; i < self->uicCount; i++) {
		ui_element_update(self->elements[i], self);
	}

	return (self->ui_mode == true ? self->input->quitPressed
						  : (self->input->quitPressed || self->player->lifePoints <= 0));
}

void Scene_Render(Scene *self)
{
    // Affichage du fond
    SDL_Renderer *renderer = Scene_GetRenderer(self);
    Assets *assets = Scene_GetAssets(self);
    SDL_RenderCopy(renderer, assets->layers[0], NULL, NULL);
    SDL_RenderCopy(renderer, assets->layers[1], NULL, NULL);

	if (!self->ui_mode) {
		// Affichage des bullets
		int bulletCount = self->bulletCount;
		for (int i = 0; i < bulletCount; i++)
		{
			Bullet_Render(self->bullets[i]);
		}

		// Affichage des ennemis
		int enemyCount = self->enemyCount;
		for (int i = 0; i < enemyCount; i++)
		{
			Enemy_Render(self->enemies[i]);
		}

		// Affichage du joueur
		Player_Render(self->player);
	}

	//Affichage des ?l?ments d'interface.
	for (int i = 0; i < self->uicCount; i++) {
		ui_element_render(self->elements[i], self);
	}
}

void Scene_AppendObject(void *object, void **objectArray, int *count, int capacity)
{
    int index = *count;
    if (index >= capacity)
        return;

    if (objectArray[index] != NULL)
    {
        assert(false);
        abort();
    }

    objectArray[index] = object;
    (*count)++;
}

void Scene_AppendEnemy(Scene *self, Enemy *enemy)
{
    Scene_AppendObject(
        enemy,
        (void **)(self->enemies),
        &(self->enemyCount),
        ENEMY_CAPACITY
    );
}

void Scene_AppendBullet(Scene *self, Bullet *bullet)
{
	if (!self) {
		return;
	}
    Scene_AppendObject(
        bullet,
        (void **)(self->bullets),
        &(self->bulletCount),
        BULLET_CAPACITY
    );
}

void Scene_AddUiElement(Scene *self, UiElement *e)
{
	if (!self) {
		return;
	}
	Scene_AppendObject(
		e,
		(void **)(self->elements),
		&(self->uicCount),
		UIC_CAPACITY
	);
}

void Scene_RemoveObject(int index, void **objectArray, int *count)
{
	int lastIndex = *count - 1;
	assert(0 <= index && index < *count);

	if (objectArray[index] == NULL)
	{
		assert(false);
		abort();
	}

	if (index == lastIndex)
	{
		// Supprime le pointeur
		objectArray[index] = NULL;
	}
	else
	{
		// Remplace le pointeur par le dernier du tableau
		objectArray[index] = objectArray[lastIndex];
		objectArray[lastIndex] = NULL;
	}
	(*count)--;
}

void Scene_RemoveUiElement(Scene *self, UiElement *e)
{
	if (!self || !e) {
		return;
	}
	int index = -1;
	for (int i = 0; i < self->uicCount; i++) {
		if (self->elements[i] == e) {
			index = i;
			break;
		}
	}
	if (index < 0) {
		return;
	}

	Scene_RemoveObject(index, (void **)(self->elements), &(self->uicCount));
}

void Scene_RemoveAllUiElements(Scene *self)
{
	if (!self) {
		return;
	}

	for (int i = 0; i < self->uicCount; i++) {
		ui_element_delete(self->elements[i]);
		self->elements[i] = NULL;
	}
	self->uicCount = 0;
}

void Scene_SetWaveIndex(Scene *self, int index)
{
	if (!self) {
		return;
	}

	self->waveIdx = index;
}

void Scene_RemoveEnemy(Scene *self, int index)
{
    Enemy_Delete(self->enemies[index]);
    Scene_RemoveObject(index, (void **)(self->enemies), &(self->enemyCount));
}

void Scene_RemoveBullet(Scene *self, int index)
{
    Bullet_Delete(self->bullets[index]);
    Scene_RemoveObject(index, (void **)(self->bullets), &(self->bulletCount));
}
