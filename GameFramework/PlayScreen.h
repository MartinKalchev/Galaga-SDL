#ifndef _PLAYSCREEN_H
#define _PLAYSCREEN_H
#include "InputManager.h"
#include "BackgroundStars.h"
#include "PlaySideBar.h"
#include "Level.h"
#include "Player.h"
#include "Enemy.h"

using namespace QuickSDL;

class PlayScreen : public GameEntity{

private:

	Timer* mTimer;
	InputManager* mInput;
	AudioManager* mAudio;

	BackgroundStars* mStars;
	PlaySideBar* mSideBar;

	Texture* mStartLabel;
	float mLevelStartTimer;
	float mLevelStartDelay;

	bool mGameStarted;

	Level* mLevel;
	bool mLevelStarted;
	int mCurrentStage;

	Player* mPlayer;

private:

	void StartNextLevel();

public:

	PlayScreen();
	~PlayScreen();

	void StartNewGame();

	bool GameOver();

	void Update();
	void Render();

};
#endif // !_PLAYSCREEN_H

