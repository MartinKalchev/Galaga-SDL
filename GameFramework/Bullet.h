#ifndef _BULLET_H
#define _BULLET_H
#include "Timer.h"
#include "PhysEntity.h"

using namespace QuickSDL;

class Bullet : public PhysEntity {

private:
	
	const int OFFSCREEN_BUFFER = 10;

	Timer* mTimer;

	float mSpeed;

	Texture* mBullet;
public:

	Bullet(bool friendly);
	~Bullet();

	void Fire(Vector2 pos);
	void Reload();

	void Hit(PhysEntity* other) override;

	void Update();
	void Render();

private:

	bool IgnoreCollisions() override;
};
#endif // !_BULLET_H
