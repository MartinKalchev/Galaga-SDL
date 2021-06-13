#include "PlayScreen.h"


PlayScreen::PlayScreen() {

	mTimer = Timer::Instance();
	mInput = InputManager::Instance();
	mAudio = AudioManager::Instance();

	mStars = BackgroundStars::Instance();

	mSideBar = new PlaySideBar();
	mSideBar->Parent(this);
	mSideBar->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH*0.85f, Graphics::Instance()->SCREEN_HEIGHT*0.05f));

	mStartLabel = new Texture("START", "emulogic.ttf", 32, { 150, 0, 0 });
	mStartLabel->Parent(this);
	mStartLabel->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.4f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));
	
	mLevel = NULL;
	mLevelStartDelay = 1.0f;
	mLevelStarted = false;

	mPlayer = NULL;

	Enemy::CreatePaths();
	Wasp::CreateDivePaths();
	Butterfly::CreateDivePaths();
	Boss::CreateDivePaths();
}

PlayScreen::~PlayScreen() {

	mTimer = NULL;
	mInput = NULL;
	mAudio = NULL;
	mStars = NULL;

	delete mSideBar;
	mSideBar = NULL;

	delete mStartLabel;
	mStartLabel = NULL;

	delete mLevel;
	mLevel = NULL;

	delete mPlayer;
	mPlayer = NULL;
}

void PlayScreen::StartNextLevel() {

	mCurrentStage++;
	mLevelStartTimer = 0.0f;
	mLevelStarted = true;
	
	delete mLevel;
	mLevel = new Level(mCurrentStage, mSideBar, mPlayer);
}

void PlayScreen::StartNewGame() {

	delete mPlayer;
	mPlayer = new Player();
	mPlayer->Parent(this);
	mPlayer->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.4f, Graphics::Instance()->SCREEN_HEIGHT * 0.9f));
	mPlayer->Active(false);

	//Deactivate the stars effect
	mStars->Scroll(false);

	mSideBar->SetHighScore(30000);
	mSideBar->SetLives(mPlayer->GetLives());
	mSideBar->SetPlayerScore(mPlayer->GetScore());
	mSideBar->SetLevel(0);

	mGameStarted = false;
	mLevelStarted = false;
	mLevelStartTimer = 0.0f;

	// Play theme song once
	mAudio->PlayMusic("gamestart.wav", 0);

	mCurrentStage = 0;
}

bool PlayScreen::GameOver() {

	if (!mLevelStarted)
		return false;

	return (mLevel->State() == Level::gameover);
}


void PlayScreen::Update() {


	if (mGameStarted) {

		if (!mLevelStarted) {

			mLevelStartTimer += mTimer->DeltaTime();
			if (mLevelStartTimer >= mLevelStartDelay)
				StartNextLevel();
		}
	}
	else {
		// When the start music stops start the level
		if (!Mix_PlayingMusic()) {

			mGameStarted = true;
		}
	}

	// Only update side bar if the game and level have started
	if (mGameStarted) {

		if(mCurrentStage > 0)
			mSideBar->Update();

		if (mLevelStarted) {

			mLevel->Update();

			if (mLevel->State() == Level::finished) {

				mLevelStarted = false;
			}
		}

		mPlayer->Update();
		mSideBar->SetPlayerScore(mPlayer->GetScore());
	}
}

void PlayScreen::Render() {

	if (!mGameStarted) 
		mStartLabel->Render();
	
	if (mGameStarted) {

		if(mLevelStarted)
			mLevel->Render();

		mPlayer->Render();
	}

	mSideBar->Render();
}