#ifndef _STARTSCREEN_H
#define _STARTSCREEN_H
#include "InputManager.h"
#include "AnimatedTexture.h"
#include "BackgroundStars.h"
#include "Scoreboard.h"

using namespace QuickSDL;


class StartScreen : public GameEntity {

private:

	Timer* mTimer;
	InputManager* mInput;

	// Create the top bar entities
	GameEntity* mTopBar;
	Texture* mPlayerOne;
	Texture* mHiScore;
	Texture* mPlayerTwo;

	//Scoreboard entities
	Scoreboard* mPlayerOneScore;
	Scoreboard* mTopScore;
	Scoreboard* mPlayerTwoScore;

	//Logo Entities
	Texture* mLogo;
	AnimatedTexture* mAnimatedLogo;


	// PlayMode Entities
	GameEntity* mPlayModes;
	Texture* mOnePlayerMode;
	Texture* mTwoPlayerMode;
	Texture* mCursor;
	Vector2 mCursorStartPos;
	Vector2 mCursorOffSet;
	int mSelectedMode;

	// Screen Animation Variables
	Vector2 mAnimationStartPos;
	Vector2 mAnimationEndPos;
	float mAnimationTotalTime; // How long the animation will play
	float mAnimationTimer; // How long will it take to go through the animation
	bool mAnimationDone;

	BackgroundStars* mStars;

public:

	StartScreen();
	~StartScreen();

	int SelectedMode();

	void ResetAnimation();

	void changeSelectedMode(int change);

	void Update();
	void Render();


};



#endif // !_STARTSCREEN_H

