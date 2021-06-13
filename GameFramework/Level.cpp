#include "Level.h"

Level::Level(int stage, PlaySideBar* sideBar, Player* player) {

	mTimer = Timer::Instance();
	mSideBar = sideBar;
	mSideBar->SetLevel(stage);
	mStars = BackgroundStars::Instance();

	mStage = stage;
	mStageStarted = false;

	mLabelTimer = 0.0f;

	mStageLabel = new Texture("STAGE", "emulogic.ttf", 32, {75, 75, 200});
	mStageLabel->Parent(this);
	mStageLabel->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.35f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));

	mStageNumber = new Scoreboard({75, 75, 200});
	mStageNumber->SetScore(mStage);
	mStageNumber->Parent(this);
	mStageNumber->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));

	mStageLabelOnScreen = 0.0f;
	mStageLabelOffScreen = 1.5f;

	mReadyLabel = new Texture("READY", "emulogic.ttf", 32, { 150, 0, 0 });
	mReadyLabel->Parent(this);
	mReadyLabel->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.4f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));

	mReadyLabelOnScreen = mStageLabelOffScreen;
	mReadyLabelOffScreen = mReadyLabelOnScreen + 3.0f;

	mPlayer = player;
	mPlayerHit = false;
	mPlayerRespawnDelay = 2.5f;
	mPlayerRespawnTimer = 0.0f;
	mPlayerRespawnLabelOnScreen = 2.0f;

	mGameOverLabel = new Texture("GAME-OVER", "emulogic.ttf", 32, { 150, 0, 0 });
	mGameOverLabel->Parent(this);
	mGameOverLabel->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.4f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));

	mGameOver = false;
	mGameOverDelay = 5.0f;
	mGameOverTimer = 0.0f;
	mGameOverLabelOnScreen = 1.0f;

	mCurrentState = running;

	mButterflyCount = 0;
	mWaspCount = 0;
	mBossCount = 0;

	std::string fullPath = SDL_GetBasePath();
	fullPath.append("Data/Level1.xml");
	mSpawningPatterns.LoadFile(fullPath.c_str());

	mChallengeStage = mSpawningPatterns.FirstChildElement("Level")->FirstChildElement()->BoolAttribute("value");

	if (!mChallengeStage) {

		mFormation = new Formation();
		mFormation->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.39f, 150.0f));
		Enemy::SetFormation(mFormation);

		for (int i = 0; i < MAX_BUTTERFLIES; i++)
			mFormationButterflies[i] = NULL;

		for (int i = 0; i < MAX_WASPS; i++)
			mFormationWasps[i] = NULL;

		for (int i = 0; i < MAX_BOSSES; i++)
			mFormationBosses[i] = NULL;
	}

	mCurrentFlyInPriority = 0;
	mCurrentFlyInPriority = 0;
	mSpawningFinished = false;
	mSpawnDelay = 0.2f;
	mSpawnTimer = 0.0f;

	mDivingButterfly = NULL;
	mSkipFirstButterfly = false;
	mButterflyDiveDelay = 1.0f;
	mButterflyDiveTimer = 0.0f;

	mDivingWasp = NULL;
	mDivingWasp2 = NULL;
	mWaspDiveDelay = 3.0f;
	mWaspDiveTimer = 0.0f;

	mDivingBoss = NULL;
	mCaptureDive = true;
	mSkipFirstBoss = true;
	mBossDiveDelay = 5.0f;
	mBossDiveTimer = 0.0f;


	Enemy::CurrentPlayer(mPlayer);
}


Level::~Level() {

	mTimer = NULL;
	mSideBar = NULL;
	mStars = NULL;

	delete mStageLabel;
	mStageLabel = NULL;

	delete mStageNumber;
	mStageNumber = NULL;

	delete mReadyLabel;
	mReadyLabel = NULL;

	mPlayer = NULL;

	delete mGameOverLabel;
	mGameOverLabel = NULL;

	if (!mChallengeStage) {


		delete mFormation;
		mFormation = NULL;

		for (int i = 0; i < MAX_BUTTERFLIES; i++) {

			delete mFormationButterflies[i];
			mFormationButterflies[i] = NULL;
		}
			

		for (int i = 0; i < MAX_WASPS; i++) {

			delete mFormationWasps[i];
			mFormationWasps[i] = NULL;
		}

		for (int i = 0; i < MAX_BOSSES; i++) {

			delete mFormationBosses[i];
			mFormationBosses[i] = NULL;
		}
	}

	for (int i = 0; i < mEnemies.size(); i++) {

		delete mEnemies[i];
		mEnemies[i] = NULL;
	}

}

void Level::StartStage() {

	mStageStarted = true;

}

void Level::StartLabelHandler() {

	mLabelTimer += mTimer->DeltaTime();
	if (mLabelTimer >= mStageLabelOffScreen) {

		mStars->Scroll(true);
		if (mStage > 1) {

			StartStage();
		}
		else {

			if (mLabelTimer >= mReadyLabelOffScreen) {
				StartStage();
				mPlayer->Active(true);
				mPlayer->Visible(true);
			}
		}
	}
}

void Level::CollisionHandler() {

	if (!mPlayerHit) {

		if (mPlayer->WasHit()) {

			
			mSideBar->SetLives(mPlayer->GetLives());
			mPlayerHit = true;
			mPlayerRespawnTimer = 0.0f;
			mPlayer->Active(false);
			mStars->Scroll(false);
		}
	}
}

void Level::PlayerDeathHandler() {

	if (!mPlayer->isAnimating()) {

		if (mPlayer->GetLives() > 0) {

			if (mPlayerRespawnTimer == 0.0f)

				mPlayer->Visible(false);

			mPlayerRespawnTimer += mTimer->DeltaTime();
			if (mPlayerRespawnTimer >= mPlayerRespawnDelay) {

				mPlayer->Active(true);
				mPlayer->Visible(true);
				mPlayerHit = false;
				mStars->Scroll(true);
			}
		}
		// If player has no more lives
		else {

			if (mGameOverTimer == 0.0f)
				mPlayer->Visible(false);

			mGameOverTimer += mTimer->DeltaTime();
			if (mGameOverTimer >= mGameOverDelay) {

				mCurrentState = gameover;
			}
		}
	}
}

bool Level::EnemyFlyingIn() {

	for (int i = 0; i < MAX_BUTTERFLIES; i++)
		if (mFormationButterflies[i] != NULL && mFormationButterflies[i]->GetCurrentState() == Enemy::flyIn)
			return true;

	for (int i = 0; i < MAX_WASPS; i++)
		if (mFormationWasps[i] != NULL && mFormationWasps[i]->GetCurrentState() == Enemy::flyIn)
			return true;

	for (int i = 0; i < MAX_BOSSES; i++)
		if (mFormationBosses[i] != NULL && mFormationBosses[i]->GetCurrentState() == Enemy::flyIn)
			return true;

	return false;
}

void Level::EnemySpawningHandler() {

	mSpawnTimer += mTimer->DeltaTime();

	if (mSpawnTimer >= mSpawnDelay) {

		XMLElement* element = mSpawningPatterns.FirstChildElement("Level")->FirstChild()->NextSiblingElement();
		bool spawned = false;
		bool priorityFound = false;

		while (element != NULL) {

			int priority = element->IntAttribute("priority");
			int path = element->IntAttribute("path");
			XMLElement* child = element->FirstChildElement();

			if (mCurrentFlyInPriority == priority) {

				priorityFound = true;

				for (int i = 0; i < mCurrentFlyInIndex && child != NULL; i++) {

					child = child->NextSiblingElement();
				}

				if (child != NULL) {

					std::string type = child->Attribute("type");
					int index = child->IntAttribute("index");

					if (type.compare("Butterfly") == 0) {

						if (!mChallengeStage) {

							mFormationButterflies[index] = new Butterfly(index, path, false);
							mButterflyCount++;

						}else
							mEnemies.push_back(new Butterfly(index, path, false));

					}else if (type.compare("Wasp") == 0) {

						if (!mChallengeStage) {

							mFormationWasps[index] = new Wasp(index, path, false, false);
							mWaspCount++;
						}else
							mEnemies.push_back(new Wasp(index, path, false, false));
					
					}else if (type.compare("Boss") == 0) {

						if (!mChallengeStage) {

							mFormationBosses[index] = new Boss(index, path, false);
							mBossCount++;
						}else
							mEnemies.push_back(new Boss(index, path, false));
				
					}

					spawned = true;
				}
			}

			element = element->NextSiblingElement();
		}

		if (!priorityFound) {

			mSpawningFinished = true;
		} else {

			if (!spawned) {

				if (!EnemyFlyingIn()) {

					mCurrentFlyInPriority++;
					mCurrentFlyInIndex = 0;
				}
			} else {

				mCurrentFlyInIndex++;
			}
		}

		mSpawnTimer = 0.0f;
	}
}

void Level::EnemyFormationHandler() {

	mFormation->Update();

	bool levelCleared = mSpawningFinished;

	for (int i = 0; i < MAX_BUTTERFLIES; i++)
		if (mFormationButterflies[i] != NULL) {

			mFormationButterflies[i]->Update();
			if (mFormationButterflies[i]->GetCurrentState() != Enemy::dead || mFormationButterflies[i]->InDeathAnimation()) {

				levelCleared = false;
			}
		}
			

	for (int i = 0; i < MAX_WASPS; i++)
		if (mFormationWasps[i] != NULL) {

			mFormationWasps[i]->Update();
			if (mFormationWasps[i]->GetCurrentState() != Enemy::dead || mFormationWasps[i]->InDeathAnimation()) {

				levelCleared = false;
			}
		}
	

	for (int i = 0; i < MAX_BOSSES; i++)
		if (mFormationBosses[i] != NULL) {

			mFormationBosses[i]->Update();

			if (mFormationBosses[i]->GetCurrentState() != Enemy::dead || mFormationButterflies[i]->InDeathAnimation()) {

				levelCleared = false;
			}
		}

	if (levelCleared) {

		mCurrentState = finished;
	}
		

	if (!mFormation->Locked()) {

		if (mButterflyCount == MAX_BUTTERFLIES && mWaspCount == MAX_WASPS && mBossCount == MAX_BOSSES) {

			if (!EnemyFlyingIn()) {

				mFormation->Lock();
			}
		}
	}
	else {

		EnemyDivingHandler();
	}
}

void Level::EnemyDivingHandler() {

	if (mDivingButterfly == NULL) {

		mButterflyDiveTimer += mTimer->DeltaTime();

		if (mButterflyDiveTimer >= mButterflyDiveDelay) {

			bool skipped = false;

			for(int i = MAX_BUTTERFLIES - 1; i >= 0; i--) {

				if (mFormationButterflies[i]->GetCurrentState() == Enemy::formation) {

					if (!mSkipFirstButterfly || (mSkipFirstButterfly && skipped)) {

						mDivingButterfly = mFormationButterflies[i];
						mDivingButterfly->Dive();
						mSkipFirstButterfly = !mSkipFirstButterfly;
						break;
					}

					skipped = true;
				}
			}

			mButterflyDiveTimer = 0.0f;
		}
	} else {

		if (mDivingButterfly->GetCurrentState() != Enemy::dive) {

			mDivingButterfly = NULL;
		}
	}

	mWaspDiveTimer += mTimer->DeltaTime();

	if (mWaspDiveTimer >= mWaspDiveDelay) {

		for (int i = MAX_WASPS - 1; i >= 0; i--) {

			if (mFormationWasps[i]->GetCurrentState() == Enemy::formation) {

				if (mDivingWasp == NULL) {

					mDivingWasp = mFormationWasps[i];
					mDivingWasp->Dive();
				}
				else if (mDivingWasp2 == NULL) {

					mDivingWasp2 = mFormationWasps[i];
					mDivingWasp2->Dive();
				}

				break;
			}
		}

		mWaspDiveTimer = 0.0f;
	}

	if (mDivingWasp != NULL && mDivingWasp->GetCurrentState() != Enemy::dive)
		mDivingWasp = NULL;

	if (mDivingWasp2 != NULL && mDivingWasp2->GetCurrentState() != Enemy::dive)
		mDivingWasp2 = NULL;



	if (mDivingBoss == NULL) {

		mBossDiveTimer += mTimer->DeltaTime();

		if (mBossDiveTimer >= mBossDiveDelay) {

			bool skipped = false;

			for (int i = MAX_BOSSES - 1; i >= 0; i--) {

				if (mFormationBosses[i]->GetCurrentState() == Enemy::formation) {

					if (!mSkipFirstBoss || (mSkipFirstBoss && skipped)) {

						mDivingBoss = mFormationBosses[i];
						if (mCaptureDive) {

							mDivingBoss->Dive(1);
						}
						else {

							mDivingBoss->Dive();
							int index = mDivingBoss->GetIndex();
							int FirstEscortIndex = (index % 2 == 0) ? (index * 2) : (index * 2 - 1);
							int SecondEscortIndex = FirstEscortIndex + 4;

							if (mFormationButterflies[FirstEscortIndex]->GetCurrentState() == Enemy::formation)
								mFormationButterflies[FirstEscortIndex]->Dive(1);

							if (mFormationButterflies[SecondEscortIndex]->GetCurrentState() == Enemy::formation)
								mFormationButterflies[SecondEscortIndex]->Dive(1);
						}

						mSkipFirstBoss = !mSkipFirstBoss;
						mCaptureDive = !mCaptureDive;
						break;
					}

					skipped = true;
				}
			}

			mBossDiveTimer = 0.0f;
		}
	}
	else {

		if (mDivingBoss->GetCurrentState() != Enemy::dive) {

			mDivingBoss = NULL;
		}
	}
}

Level::LEVEL_STATES Level::State() {

	return mCurrentState;
}

void Level::Update() {

	//As long the stage hasnt started

	if (!mStageStarted) {
		
		StartLabelHandler();
	}
	else {

		if (!mSpawningFinished)
			EnemySpawningHandler();

		if(!mChallengeStage)
			EnemyFormationHandler();

		for (int i = 0; i < mEnemies.size(); i++) {

			mEnemies[i]->Update();
		}

		CollisionHandler();

		if (mPlayerHit) {

			PlayerDeathHandler();
		}	
	}
}

void Level::Render() {

	// Game start labels
	if (!mStageStarted) {

		if (mLabelTimer > mStageLabelOnScreen && mLabelTimer < mStageLabelOffScreen) {

			mStageLabel->Render();
			mStageNumber->Render();
		}
		else if (mLabelTimer > mReadyLabelOnScreen && mLabelTimer < mReadyLabelOffScreen) {

			mReadyLabel->Render();
		}
	}
	// Player respawn and game over labels
	else {


		if (!mChallengeStage) {

			for (int i = 0; i < MAX_BUTTERFLIES; i++)
				if (mFormationButterflies[i] != NULL)
					mFormationButterflies[i]->Render();

			for (int i = 0; i < MAX_WASPS; i++)
				if (mFormationWasps[i] != NULL)
					mFormationWasps[i]->Render();

			for (int i = 0; i < MAX_BOSSES; i++)
				if (mFormationBosses[i] != NULL)
					mFormationBosses[i]->Render();
		}

		for (int i = 0; i < mEnemies.size(); i++) 
			mEnemies[i]->Render();
		

		if (mPlayerHit) {

			if (mPlayerRespawnTimer >= mPlayerRespawnLabelOnScreen) {

				mReadyLabel->Render();
			}

			if (mGameOverTimer >= mGameOverLabelOnScreen) {

				mGameOverLabel->Render();
			}
		}
	}

	//Render player and enemies here
}