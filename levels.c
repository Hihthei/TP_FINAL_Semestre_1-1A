#include "levels.h"

void craft_level_1(Scene *self)
{
	Enemy *enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.0f, 4.5f));

	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 0);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 0);

	Scene_AppendEnemy(self, enemy);
}
