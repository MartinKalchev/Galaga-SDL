#include "StartScreen.h"

StartScreen::StartScreen() {

	mTimer = Timer::Instance();

	mInput = InputManager::Instance();

	// Top Bar Entities
	mTopBar = new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH*0.5f, 80.0f));
	mPlayerOne = new Texture("1UP", "emulogic.ttf", 32, { 200, 0, 0 });
	mHiScore = new Texture("HI-SCORE", "emulogic.ttf", 32, { 200, 0, 0 });
	mPlayerTwo = new Texture("2UP", "emulogic.ttf", 32, { 200, 0, 0 });
	mPlayerOneScore = new Scoreboard();
	mTopScore = new Scoreboard();
	mPlayerTwoScore = new Scoreboard();


	mPlayerOne->Parent(mTopBar);
	mHiScore->Parent(mTopBar);
	mPlayerTwo->Parent(mTopBar);
	mPlayerOneScore->Parent(mTopBar);
	mTopScore->Parent(mTopBar);
	mPlayerTwoScore->Parent(mTopBar);

	mPlayerOne->Pos(Vector2(-Graphics::Instance()->SCREEN_WIDTH*0.35f, 0.0f));
	mHiScore->Pos(Vector2(-30.0f, 0.0f));
	mPlayerTwo->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.25f, 0.0f));

	mPlayerOneScore->Pos(Vector2(-Graphics::Instance()->SCREEN_WIDTH * 0.23f, 40.0f));
	mTopScore->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH*0.05f, 40.0f));
	mPlayerTwoScore->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.32f, 40.0f));

	mTopScore->SetScore(30000);


	mTopBar->Parent(this);

	//Logo Entities
	mLogo = new Texture("galagalogo.png", 0, 0, 360, 180);
	mAnimatedLogo = new AnimatedTexture("galagalogo.png", 0, 0, 360, 180, 3, 0.2f, AnimatedTexture::vertical);

	mLogo->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f, Graphics::Instance()->SCREEN_HEIGHT*0.32f));
	mAnimatedLogo->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f, Graphics::Instance()->SCREEN_HEIGHT * 0.32f));

	mLogo->Parent(this);
	mAnimatedLogo->Parent(this);

	//Play Mode Entities
	mPlayModes = new GameEntity(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f, Graphics::Instance()->SCREEN_HEIGHT * 0.55f));
	mOnePlayerMode = new Texture("1 Player", "emulogic.ttf", 32, { 230, 230, 230 });
	mTwoPlayerMode = new Texture("2 Players", "emulogic.ttf", 32, { 230, 230, 230 });
	mCursor = new Texture("Cursor.png");

	mOnePlayerMode->Parent(mPlayModes);
	mTwoPlayerMode->Parent(mPlayModes);
	mCursor->Parent(mPlayModes);

	mOnePlayerMode->Pos(Vector2(-18.0f, -35.0f));
	mTwoPlayerMode->Pos(Vector2(0.0f, 35.0f));
	mCursor->Pos(Vector2(-175.0f, -35.0f));

	mPlayModes->Parent(this);

	mCursorStartPos = mCursor->Pos(local);
	mCursorOffSet = Vector2(0.0f, 70.0f);
	mSelectedMode = 0;


	// Screen Animation variables
	ResetAnimation();

	mStars = BackgroundStars::Instance();
	mStars->Scroll(true);
}


StartScreen::~StartScreen() {

	// Freeing top bar entities
	delete mTopBar;
	mTopBar = NULL;

	delete mPlayerOne;
	mPlayerOne = NULL;

	delete mHiScore;
	mHiScore = NULL;

	delete mPlayerTwo;
	mPlayerTwo = NULL;

	delete mPlayerOneScore;
	mPlayerOneScore = NULL;

	delete mTopScore;
	mTopScore = NULL;

	delete mPlayerTwoScore;
	mPlayerTwoScore = NULL;

	//Freeing Logo Entities
	delete mLogo;
	mLogo = NULL;
	delete mAnimatedLogo;
	mAnimatedLogo = NULL;

	// Freeing Play Mode Entities
	delete mPlayModes;
	mPlayModes = NULL;

	delete mOnePlayerMode;
	mOnePlayerMode = NULL;

	delete mTwoPlayerMode;
	mTwoPlayerMode = NULL;

	delete mCursor;
	mCursor = NULL;
}

void StartScreen::ResetAnimation() {

	mAnimationStartPos = Vector2(0.0f, Graphics::Instance()->SCREEN_HEIGHT);
	mAnimationEndPos = VEC2_ZERO;
	mAnimationTotalTime = 5.0f;
	mAnimationTimer = 0.0f;
	mAnimationDone = false;

	Pos(mAnimationStartPos);
}

int StartScreen::SelectedMode() {

	return mSelectedMode;
}

void StartScreen::changeSelectedMode(int change) {

	mSelectedMode += change;

	if (mSelectedMode < 0)
		mSelectedMode = 1;
	else if (mSelectedMode > 1)
		mSelectedMode = 0;

	mCursor->Pos(mCursorStartPos + mCursorOffSet * mSelectedMode);
}


void StartScreen::Update() {

	if (!mAnimationDone) {

		mAnimationTimer += mTimer->DeltaTime();
		Pos(Lerp(mAnimationStartPos, mAnimationEndPos, mAnimationTimer / mAnimationTotalTime));

		if (mAnimationTimer >= mAnimationTotalTime) {
			mAnimationDone = true;
			mStars->Scroll(false);
		}
	}
	else {

		mAnimatedLogo->Update();

		if (mInput->KeyPressed(SDL_SCANCODE_DOWN))
			changeSelectedMode(1);
		else if (mInput->KeyPressed(SDL_SCANCODE_UP))
			changeSelectedMode(0);
	}
}

void StartScreen::Render() {

	// Rendering Top Bar Entities
	mPlayerOne->Render();
	mHiScore->Render();
	mPlayerTwo->Render();
	mPlayerOneScore->Render();
	mTopScore->Render();
	mPlayerTwoScore->Render();

	if (!mAnimationDone)
		mLogo->Render();
	else
		mAnimatedLogo->Render();

	// Rendering Play Mode Entities
	mOnePlayerMode->Render();
	mTwoPlayerMode->Render();
	mCursor->Render();
}