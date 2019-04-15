#pragma once

namespace SCLConstants
{
	// TODO: Some better way to do this? >_<

	const std::string NAME = "Secret Cow Level - Lori";

	// Screen / world
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const int SCREEN_COUNT = 3;						// 3 x 3 world
	const float SCREEN_WIDTH_WORLD_SIZE = 50.0f;	// Screen width in world units

	// Timing
	const float FPS_FIXED = 60.0f;
	const float FPS_MAX = 120.0f;
	const int FPS_SAMPLES_COUNT = 30;
	const bool IS_FPS_CAPPED = false;

	// UI
	const float TEXT_SIZE = 2.0f;
	const float TEXT_ALPHA = 0.9f;
	const float FPS_DISPLAY_UPDATE_TIME = 0.5f;

	// Player
	const unsigned int PLAYER_HP_MAX = 10;
	const float PLAYER_SIZE = 2.0f;
	const float PLAYER_MOVE_SPEED = 20.0f;
	const float PLAYER_INVINCIBLE_DUR = 2.0f;
	const float PLAYER_INVINCIBLE_ALPHA = 0.5f;
	const float PLAYER_FIRING_DISTANCE = 2.0f;		// Distance from center or player
	const float PLAYER_FIRING_SPEED_MIN = 7.5f;		// Bullet/sec
	const float PLAYER_FIRING_SPEED_MAX = 200.0f;	// Bullet/sec	
	const float PLAYER_FIRING_ANGLE_MIN = 20.0f;	// Degrees
	const float PLAYER_FIRING_ANGLE_MAX = 360.0f;	// Degrees
	const float PLAYER_SUPERMODE_DUR = 1.0f;
	const float PLAYER_SUPERMODE_SPEED = 200.0f;	// Bullets/sec
	const float PLAYER_SUPERMODE_ANGLE = 360.0f;	// Degrees

	// Enemies
	const unsigned int ENEMY_COUNT_MAX = 2000;		// wow... >__<
	const int ENEMY_GRID_SCREEN_DIV = 10;
	const int ENEMY_GRID_CELLS_FROM_PLAYER = 3;		// Cannot spawn enemies within this no. of cells from the player
	// TODO: How about checking the distance instead?
	const float ENEMY_SIZE = 2.0f;
	const float ENEMY_COLLIDER_SIZE = ENEMY_SIZE * 0.5f;
	const unsigned int ENEMY_TYPE_MAX = 3;

	// Bullets
	const unsigned int BULLET_COUNT_MAX = 500;
	const float BULLET_SIZE = 1.0f;
	const float BULLET_SPEED = 40.0f;
	const float BULLET_DIST_MAX = 25.0f;			// Max distance reached before getting destroyed

	// Pickups
	const float PICKUP_SIZE = 2.0f;
	const unsigned int PICKUP_COUNT_MAX = 50;
	const unsigned int PICKUP_HP_PLUS = 1;			// Additional HP per pickup
	const float PICKUP_ANGLE_PLUS = 2.5f;			// Additional firing angle per pickup
	const float PICKUP_SPEED_PLUS = 2.5f;			// Additional firing speed per pickup

	// Input
	const bool INPUT_IS_AUTOFIRE = true;
	const bool INPUT_IS_ROTATE_ON = true;			// Reset to false to have the player just shoot at mouse position
	const float INPUT_ROTATE_SPEED = 1080.0f;
	const float EDGE_SCROLLING_MIN = 0.3f;			// Scroll at 30% of the screen
	const float EDGE_SCROLLING_MAX = 0.7f;			// Scroll at 70% of the screen

	// Map
	const int MAP_GRID_SCREEN_DIV = 5;
	const int MAP_DECOR_PER_CELL = 2;
	const int MAP_STATIC_ITEM_PER_CELL = 1;			// Collidable items
	const float MAP_BG_SIZE = 200.0f;
	const float MAP_DECOR_SIZE = 15.0f;
	const float MAP_STATIC_ITEM_SIZE = 8.0f;
}