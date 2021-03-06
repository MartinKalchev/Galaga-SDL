#include "PlaySideBar.h"

PlaySideBar::PlaySideBar() {

	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	// Defining the side bar
	mBackground = new Texture("black.png");
	mBackground->Parent(this);
	mBackground->Pos(Vector2(45.0f, 380.0f));
	mBackground->Scale(Vector2(3.0f, 10.0f));

	// Player Score
	mHighLabel = new Texture("HIGH", "emulogic.ttf", 32, { 150, 0, 0 });
	mHighLabel->Parent(this);
	mHighLabel->Pos(Vector2(-25.0f, 0.0f));

	mScoreLabel = new Texture("SCORE", "emulogic.ttf", 32, { 150, 0, 0 });
	mScoreLabel->Parent(this);
	mScoreLabel->Pos(Vector2(25.0f, 32.0f));

	mHighScoreboard = new Scoreboard();
	mHighScoreboard->Parent(this);
	mHighScoreboard->Pos(Vector2(90.0f, 64.0f));

	mPlayerOneLabel = new Texture("1UP", "emulogic.ttf", 32, { 150, 0, 0 });
	mPlayerOneLabel->Parent(this);
	mPlayerOneLabel->Pos(Vector2(-45.0f, 160.0f));

	mBlinkInterval = 0.5f;
	mBlinkTimer = 0.0f;
	mPlayerOneLabelVisible = true;

	mPlayerOneScore = new Scoreboard();
	mPlayerOneScore->Parent(this);
	mPlayerOneScore->Pos(Vector2(90.0f, 192.0f));

	// Player Lives
	mShips = new GameEntity();
	mShips->Parent(this);
	mShips->Pos(Vector2(-40.0f, 450.0f));

	// Player lives textures in the side bar
	for (int i = 0; i < MAX_SHIP_TEXTURES; i++) {

		mShipTextures[i] = new Texture("playership.png");
		mShipTextures[i]->Parent(mShips);
		mShipTextures[i]->Pos(Vector2(62.0f * (i % 3), 70.0f * (i / 3)));
	}

	mTotalShipsLabel = new Scoreboard();
	mTotalShipsLabel->Parent(mShips);
	mTotalShipsLabel->Pos(Vector2(140.0f, 80.0f));

	//Player Flags
	mFlags = new GameEntity();
	mFlags->Parent(this);
	mFlags->Pos(Vector2(-50.0f, 650.0f));

	mFlagTimer = 0.0f;
	mFlagInterval = 0.2f;

}

PlaySideBar::~PlaySideBar() {

	mTimer = NULL;
	mAudio = NULL;

	delete mBackground;
	mBackground = NULL;

	delete mHighLabel;
	mHighLabel = NULL;

	delete mScoreLabel;
	mScoreLabel = NULL;

	delete mHighScoreboard;
	mHighScoreboard = NULL;

	delete mPlayerOneLabel;
	mPlayerOneLabel = NULL;

	delete mPlayerOneScore;
	mPlayerOneScore = NULL;

	delete mShips;
	mShips = NULL;

	for (int i = 0; i < MAX_SHIP_TEXTURES; i++) {

		delete mShipTextures[i];
		mShipTextures[i] = NULL;
	}

	delete mTotalShipsLabel;
	mTotalShipsLabel = NULL;

	delete mFlags;
	mFlags = NULL;

	ClearFlags();
}

void PlaySideBar::ClearFlags() {

	for (int i = 0; i < mFlagTextures.size(); i++) {
		delete mFlagTextures[i];
		mFlagTextures[i] = NULL;
	}

	mFlagTextures.clear();
}

void PlaySideBar::AddNextFlag() {

	if (mRemainingLevels >= 50)
		AddFlag("fiftylevelflag.png", 62.0f, 50);
	else if (mRemainingLevels >= 30)
		AddFlag("thirtylevelflag.png", 62.0f, 30);
	else if (mRemainingLevels >= 20)
		AddFlag("twentylevelflag.png", 62.0f, 20);
	else if (mRemainingLevels >= 10)
		AddFlag("tenlevelflag.png", 54.0f, 10);
	else if (mRemainingLevels >= 5)
		AddFlag("fivelevelflag.png", 30.0f, 5);
	else
		AddFlag("onelevelflag.png", 30.0f, 1);
}

void PlaySideBar::AddFlag(std::string filename, float width, int value) {

	// The index will be always the last index
	int index = mFlagTextures.size();

	if(index > 0)
		mFlagXOffset += width * 0.5f;

	mRemainingLevels -= value;
	mFlagTextures.push_back(new Texture(filename));
	mFlagTextures[index]->Parent(mFlags);
	mFlagTextures[index]->Pos(VEC2_RIGHT * mFlagXOffset);
	mFlagXOffset += width * 0.5f;

	mAudio->PlaySFX("flagsound.wav");
}

void PlaySideBar::SetHighScore(int score) {

	mHighScoreboard->SetScore(score);
}

void PlaySideBar::SetPlayerScore(int score) {

	mPlayerOneScore->SetScore(score);
}

void PlaySideBar::SetLives(int lives) {

	mTotalShips = lives;

	if (lives > MAX_SHIP_TEXTURES)
		mTotalShipsLabel->SetScore(lives);
}

void PlaySideBar::SetLevel(int level) {

	// Cler the past level flags to set the new level ones
	ClearFlags();

	mRemainingLevels = level;
	mFlagXOffset = 0.0f;
}

void PlaySideBar::Update() {

	mBlinkTimer += mTimer->DeltaTime();
	if (mBlinkTimer >= mBlinkInterval) {
		mPlayerOneLabelVisible = !mPlayerOneLabelVisible;
		mBlinkTimer = 0.0f;
	}

	if (mRemainingLevels > 0) {

		mFlagTimer += mTimer->DeltaTime();
		if (mFlagTimer >= mFlagInterval) {

			AddNextFlag();
			mFlagTimer = 0.0f;
		}
	}
}

void PlaySideBar::Render() {

	mBackground->Render();

	mHighLabel->Render();
	mScoreLabel->Render();
	mHighScoreboard->Render();

	if(mPlayerOneLabelVisible)
		mPlayerOneLabel->Render();

	mPlayerOneScore->Render();

	for (int i = 0; i < MAX_SHIP_TEXTURES && i < mTotalShips; i++) {

		mShipTextures[i]->Render();
	}

	// If the lives are more the the total ship tetures which is 5
	if(mTotalShips > MAX_SHIP_TEXTURES)
		mTotalShipsLabel->Render();

	for (int i = 0; i < mFlagTextures.size(); i++) {

		mFlagTextures[i]->Render();
	}
}