#ifndef _LEVEL_H
#define _LEVEL_H
#include "InputManager.h"
#include "PlaySideBar.h"
#include "BackgroundStars.h"
#include "Player.h"
#include "Butterfly.h"
#include "Wasp.h"
#include "Boss.h"
#include "tinyxml2.h"

using namespace tinyxml2;

class Level : public GameEntity {


public:
	//Keeps track of the state of the level
	enum LEVEL_STATES {running, finished, gameover};


private:

	Timer* mTimer;
	PlaySideBar* mSideBar;
	BackgroundStars* mStars;

	// Show the current stage
	int mStage;
	bool mStageStarted;

	// keeps track of all the labels shown
	float mLabelTimer;

	Texture* mStageLabel;
	Scoreboard* mStageNumber;
	// start and end point where the label appears
	float mStageLabelOnScreen;
	float mStageLabelOffScreen;

	Texture* mReadyLabel;
	float mReadyLabelOnScreen;
	float mReadyLabelOffScreen;

	Player* mPlayer;

	bool mPlayerHit;
	float mPlayerRespawnDelay;
	float mPlayerRespawnTimer;
	float mPlayerRespawnLabelOnScreen;

	Texture* mGameOverLabel;
	bool mGameOver;
	float mGameOverDelay;
	float mGameOverTimer;
	float mGameOverLabelOnScreen;

	LEVEL_STATES mCurrentState;

	bool mChallengeStage;

	Formation* mFormation;

	static const int MAX_BUTTERFLIES = 16;
	int mButterflyCount;

	static const int MAX_WASPS = 20;
	int mWaspCount;

	static const int MAX_BOSSES = 4;
	int mBossCount;


	Butterfly* mFormationButterflies[MAX_BUTTERFLIES];
	Wasp* mFormationWasps[MAX_WASPS];
	Boss* mFormationBosses[MAX_BOSSES];

	std::vector<Enemy*> mEnemies;

	XMLDocument mSpawningPatterns;
	int mCurrentFlyInPriority;
	int mCurrentFlyInIndex;

	bool mSpawningFinished;
	float mSpawnDelay;
	float mSpawnTimer;

	Butterfly* mDivingButterfly;
	bool mSkipFirstButterfly;
	float mButterflyDiveDelay;
	float mButterflyDiveTimer;

	Wasp* mDivingWasp;
	Wasp* mDivingWasp2;
	float mWaspDiveDelay;
	float mWaspDiveTimer;

	Boss* mDivingBoss;
	bool mCaptureDive;
	bool mSkipFirstBoss;
	float mBossDiveDelay;
	float mBossDiveTimer;

private:

	void StartStage();
	void StartLabelHandler();
	void CollisionHandler();
	void PlayerDeathHandler();

	bool EnemyFlyingIn();

	void EnemySpawningHandler();
	void EnemyFormationHandler();
	void EnemyDivingHandler();

public:

	Level(int stage, PlaySideBar* sideBar, Player* player);
	~Level();

	LEVEL_STATES State();

	void Update();
	void Render();

};
#endif // !_LEVEL_H