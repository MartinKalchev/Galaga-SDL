#ifndef _WASP_H
#define _WASP_H
#include "Enemy.h"

class Wasp : public Enemy {

private:

	static std::vector<std::vector<Vector2>> sDivePaths;

	bool mDiver;

private:

	void PathComplete();
	void FlyInComplete();

	Vector2 LocalFormationPos();

	void DiveStateHandler();
	

	void RenderDiveState();
	

public:

	static void CreateDivePaths();

	Wasp(int index, int path, bool challengeStage, bool diver);
	~Wasp();

	void Hit(PhysEntity* other) override;

};
#endif
