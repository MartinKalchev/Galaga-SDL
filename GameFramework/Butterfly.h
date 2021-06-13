#ifndef _BUTTERFLY_H
#define _BUTTERFLY_H
#include "Enemy.h"

class Butterfly : public Enemy {

private:

	static std::vector<std::vector<Vector2>> sDivePaths;

	//Dive with a boss or not
	bool mEscort;

private:

	Vector2 LocalFormationPos();

	void DiveStateHandler();
	

	void RenderDiveState();
	

public:

	static void CreateDivePaths();

	Butterfly(int index, int path, bool challengeStage);
	~Butterfly();

	void Dive(int type = 0);

	void Hit(PhysEntity* other) override;

};


#endif // !_BUTTERFLY_H
