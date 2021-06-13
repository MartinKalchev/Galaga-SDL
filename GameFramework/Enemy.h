#ifndef _ENEMY_H
#define _ENEMY_H
#include "AnimatedTexture.h"
#include "BezierPath.h"
#include "Formation.h"
#include "PhysEntity.h"
#include "Player.h"
class Enemy : public PhysEntity {


public:

	enum STATES { flyIn, formation, dive, dead };
	enum TYPES {butterfly, wasp, boss};

protected:
	
	static std::vector<std::vector<Vector2>> sPaths;
	static Formation* sFormation;

	static Player* sPlayer;

	Timer* mTimer;

	Texture* mEnemies[2];

	STATES mCurrentState;

	int mCurrentPath;

	int mCurrentWaypoint;
	const float EPSILON = 50.0f;

	float mSpeed;

	TYPES mType;
	int mIndex;

	bool mChallengeStage;

	Vector2 mDiveStartPos;

	AnimatedTexture* mDeathAnimation;

protected:

	virtual void PathComplete();

	void JoinFormation();
	virtual void FlyInComplete();

	virtual Vector2 WorldFormationPos();

	// Each enemy type has different formations and it will be decided in each specific enemy class
	virtual Vector2 LocalFormationPos() = 0;

	virtual void FlyInStatehandler();
	virtual void FormationStateHandler();
	virtual void DiveStateHandler() = 0;
	virtual void DeadStateHandler();

	void StatesHandler();

	virtual void RenderFlyInState();
	virtual void RenderFormationState();
	virtual void RenderDiveState() = 0;
	virtual void RenderDeadState();

	void RenderStates();

	bool IgnoreCollisions() override;
public:

	// Called only once
	static void CreatePaths();

	static void SetFormation(Formation* f);

	static void CurrentPlayer(Player* player);

	Enemy(int index,int path, bool challengeStage);
	virtual ~Enemy();

	virtual void Hit(PhysEntity* other) override;

	STATES GetCurrentState();

	TYPES GetType();

	int GetIndex();

	virtual void Dive(int type = 0);

	bool InDeathAnimation();

	void Update();
	void Render();

};

#endif 