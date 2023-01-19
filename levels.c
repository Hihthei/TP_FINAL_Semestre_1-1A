#include "levels.h"
#include "Assets.h"

void craft_level_1(Scene *self)
{
	Enemy *enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.0f, 4.5f));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 2);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 0);

	Scene_AppendEnemy(self, enemy);
}

void craft_level_1_boss(Scene *self)
{
	//création des portails
	Portal* portal_1_1 = Portal_New(self, BLUE_PORTAL, NULL, Vec2_Set(6.0f, 6.0f));
	Portal* portal_1_2 = Portal_New(self, BLUE_PORTAL, portal_1_1, Vec2_Set(15.0f, 2.0f));
	portal_1_1->linked_portal = portal_1_2;
	Scene_AppendPortal(self, portal_1_1);
	Scene_AppendPortal(self, portal_1_2);

	Portal* portal_2_1 = Portal_New(self, RED_PORTAL, NULL, Vec2_Set(5.0f, 1.0f));
	Portal* portal_2_2 = Portal_New(self, RED_PORTAL, portal_2_1, Vec2_Set(14.5f, 8.0f));
	portal_1_1->linked_portal = portal_2_2;
	Scene_AppendPortal(self, portal_2_1);
	Scene_AppendPortal(self, portal_2_2);

	//--------------------------------------------------------------------------
	//création d'enemies
	Enemy *enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.0f, 0.5f));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 2);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 1);
	Scene_AppendEnemy(self, enemy);

	enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.0f, 3.2f));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 2);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 1);
	Scene_AppendEnemy(self, enemy);

	enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.0f, 4.8f));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 2);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 1);
	Scene_AppendEnemy(self, enemy);

	enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.0f, 8.5f));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 2);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 1);
	Scene_AppendEnemy(self, enemy);
}

void craft_level_2(Scene *self)
{
	Enemy *enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.5, 2));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 1);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 1);
	Scene_AppendEnemy(self, enemy);

	enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.5, 7));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 1);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 1);
	Scene_AppendEnemy(self, enemy);
}

void craft_level_2_boss(Scene *self)
{
	Enemy *enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.5f, 4.5f));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 2);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 5);
	enemy->enemyRaisedOrDead = &enemy_drops_life;
	Scene_AppendEnemy(self, enemy);

	enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.5, 0.5f));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 2);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 2);
	Scene_AppendEnemy(self, enemy);

	enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.5, 8.5f));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 2);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 2);
	Scene_AppendEnemy(self, enemy);
}

void craft_level_3(Scene *self)
{
	Enemy *enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.5, 2));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 1);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 0);
	Scene_AppendEnemy(self, enemy);

	enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.5, 7));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 1);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 0);
	Scene_AppendEnemy(self, enemy);
}

void craft_level_4(Scene *self)
{
	Enemy *enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.5, 2));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 2);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 5);
	Scene_AppendEnemy(self, enemy);

	enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.5, 7));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 0);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 2);
	Scene_AppendEnemy(self, enemy);
}

void craft_level_4_5(Scene *self)
{
	Enemy *enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.5, 0.5));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 2);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 3);
	enemy->enemyRaisedOrDead = &enemy_drops_life;
	Scene_AppendEnemy(self, enemy);

	enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.5, 8.5));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 1);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 3);
	enemy->enemyRaisedOrDead = &enemy_drops_life;
	Scene_AppendEnemy(self, enemy);

	enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(15.5, 4.5));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 2);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 5);
	Scene_AppendEnemy(self, enemy);
}

void craft_level_4_boss(Scene *self)
{
	Enemy *enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(10, 0.5));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 2);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 4);
	Scene_AppendEnemy(self, enemy);

	enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(10, 4));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 1);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 4);
	Scene_AppendEnemy(self, enemy);

	enemy = Enemy_New(self, ENEMY_FIGHTER, Vec2_Set(10, 5));
	enemy->updatePos = get_pattern(PATTERN_ENEMY_MOVE, 1);
	enemy->throwAttack = get_pattern(PATTERN_ENEMY_THROW, 4);
	Scene_AppendEnemy(self, enemy);

	Scene_AppendEnemy(self, enemy);
}
