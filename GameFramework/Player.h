#ifndef _PLAYER_H
#define _PLAYER_H
#include "AnimatedTexture.h"
#include "InputManager.h"
#include "AudioManager.h"
#include "Bullet.h"

using namespace QuickSDL;

class Player : public PhysEntity {

private:

	Timer* mTimer;
	InputManager* mInput;
	AudioManager* mAudio;

	// Used for switching the player on and off
	bool mVisible;
	// Stops the player from moving
	bool mAnimating;

	bool mWasHit;

	int mScore;
	int mLives;

	Texture* mShip;
	AnimatedTexture* mDeathAnimation;

	float mMoveSpeed;
	Vector2 mMoveBounds;

	static const int MAX_BULLETS = 2;
	Bullet* mBullets[MAX_BULLETS];

private:

	bool IgnoreCollisions() override;

	void MovementHandler();
	void FiringHandler();

public:

	Player();
	~Player();

	void Visible(bool visible);

	void Hit(PhysEntity* other) override;
	// Called whenever there is a collision between the player and an object in the scene
	bool WasHit();

	bool isAnimating();

	int GetScore();
	int GetLives();

	void AddScore(int change);

	void Update();
	void Render();

};
#endif // !_PLAYER_H