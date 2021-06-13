#include "Enemy.h"
#include "PhysicsManager.h"
std::vector<std::vector<Vector2>> Enemy::sPaths;

Formation* Enemy::sFormation = NULL;

Player* Enemy::sPlayer = nullptr;

void Enemy::CreatePaths() {

	//Level half end point
	int screenMidPoint = Graphics::Instance()->SCREEN_WIDTH * 0.4f;

	int currentPath = 0;
	BezierPath* path = new BezierPath();
	path->AddCurve({ Vector2(screenMidPoint + 50.0f, -10.0f), Vector2(screenMidPoint + 50.0f, -20.0f), Vector2(screenMidPoint + 50.0f, 30.0f), Vector2(screenMidPoint + 50.0f, 20.0f) }, 1);

	path->AddCurve({ Vector2(screenMidPoint + 50.0f, 20.0f), Vector2(screenMidPoint + 50.0f, 100.0f), Vector2(75.0f, 325.0f), Vector2(75.0f, 425.0f) }, 25);

	path->AddCurve({ Vector2(75.0f, 425.0f), Vector2(75.0f, 650.0f), Vector2(350.0f, 650.0f), Vector2(350.0f, 425.0f) }, 25);

	sPaths.push_back(std::vector<Vector2>());
	path->Sample(&sPaths[currentPath]);
	delete path;

	currentPath = 1;
	path = new BezierPath();
	int fullScreen = screenMidPoint * 2;
	path->AddCurve({ Vector2(screenMidPoint - 50.0f, -10.0f), Vector2(screenMidPoint - 50.0f, -20.0f), Vector2(screenMidPoint - 50.0f, 30.0f), Vector2(screenMidPoint - 50.0f, 20.0f) }, 1);

	path->AddCurve({ Vector2(screenMidPoint - 50.0f, 20.0f), Vector2(screenMidPoint - 50.0f, 100.0f), Vector2(fullScreen - 75.0f, 325.0f), Vector2(fullScreen - 75.0f, 425.0f) }, 25);

	path->AddCurve({ Vector2(fullScreen - 75.0f, 425.0f), Vector2(fullScreen - 75.0f, 650.0f), Vector2(fullScreen - 350.0f, 650.0f), Vector2(fullScreen - 350.0f, 425.0f) }, 25);

	sPaths.push_back(std::vector<Vector2>());
	path->Sample(&sPaths[currentPath]);
	delete path;

	currentPath = 2;
	path = new BezierPath();
	int temp = screenMidPoint + 100.0f;
	
	path->AddCurve({ Vector2(-40.0f, 720.0f), Vector2(-50.0f, 720.0f), Vector2(10.0f, 720.0f), Vector2(0.0f, 720.0f) }, 1);
	path->AddCurve({ Vector2(0.0f, 720.0f), Vector2(200.0f, 720.0f), Vector2(temp, 500.0f), Vector2(temp, 400.0f) }, 15);
	path->AddCurve({ Vector2(temp, 400.0f), Vector2(temp, 200.0f), Vector2(40.0f, 200.0f), Vector2(40.0f, 400.0f) }, 15);
	path->AddCurve({ Vector2(40.0f, 400.0f), Vector2(40.0f, 500.0f), Vector2(temp -120.0f, 600.0f), Vector2(temp - 40.0f, 440.0f) }, 15);

	sPaths.push_back(std::vector<Vector2>());
	path->Sample(&sPaths[currentPath]);
	delete path;

	currentPath = 3;
	path = new BezierPath();
	temp = screenMidPoint + 60.0f;
	int temp2 = fullScreen - 40.0f;

	path->AddCurve({ Vector2(temp2 + 40.0f, 720.0f), Vector2(temp2 + 50.0f, 720.0f), Vector2(temp2 + 10.0f, 720.0f), Vector2(temp2, 720.0f) }, 1);
	path->AddCurve({ Vector2(temp2, 720.0f), Vector2(temp2 - 200.0f, 720.0f), Vector2(temp, 500.0f), Vector2(temp, 400.0f) }, 15);
	path->AddCurve({ Vector2(temp, 400.0f), Vector2(temp, 200.0f), Vector2(temp2 - 40.0f, 200.0f), Vector2(temp2 - 40.0f, 400.0f) }, 15);
	path->AddCurve({ Vector2(temp2 - 40.0f, 400.0f), Vector2(temp2 - 40.0f, 500.0f), Vector2(temp + 120.0f, 600.0f), Vector2(temp + 40.0f, 440.0f) }, 15);

	sPaths.push_back(std::vector<Vector2>());
	path->Sample(&sPaths[currentPath]);
	delete path;
}

void Enemy::SetFormation(Formation* f) {

	sFormation = f;
}

void Enemy::CurrentPlayer(Player* player) {

	sPlayer = player;
}

Enemy::Enemy(int index, int path, bool challengeStage) {

	mTimer = Timer::Instance();

	mCurrentPath = path;

	mCurrentState = flyIn;

	mCurrentWaypoint = 1;
	Pos(sPaths[mCurrentPath][0]);

	mEnemies[0] = NULL;
	mEnemies[1] = NULL;

	mSpeed = 400.0f;

	mIndex = index;

	mChallengeStage = challengeStage;

	mId = PhysicsManager::Instance()->RegisterEntity(this, PhysicsManager::CollisionLayers::Hostile);

	mDeathAnimation = new AnimatedTexture("enemydead.png", 0, 0, 128, 128, 5, 1.0f, AnimatedTexture::horizontal);
	mDeathAnimation->Parent(this);
	mDeathAnimation->Pos(VEC2_ZERO);
	mDeathAnimation->WrapMode(AnimatedTexture::once);
}

Enemy::~Enemy() {

	mTimer = NULL;

	for (int i = 0; i < 2; i++) {

		delete mEnemies[i];
		mEnemies[i] = NULL;
	}

	delete mDeathAnimation;
	mDeathAnimation = nullptr;
}

void Enemy::Hit(PhysEntity* other) {

	if (mCurrentState == formation) {

		Parent(nullptr);
	}

	mCurrentState = dead;
}

void Enemy::PathComplete() {

	if (mChallengeStage) {

		mCurrentState = dead;
	}
}

void Enemy::JoinFormation() {

	Pos(WorldFormationPos());
	Parent(sFormation);
	mCurrentState = formation;
}

Vector2 Enemy::WorldFormationPos() {

	return sFormation->Pos() + LocalFormationPos();
}

void Enemy::FlyInComplete() {

	if (mChallengeStage)
		mCurrentState = dead;
	else
		JoinFormation();
}

void Enemy::FlyInStatehandler() {

	// If it is still in range of the path keep interpolating to the next waypoint
	if (mCurrentWaypoint < sPaths[mCurrentPath].size()) {

		Vector2 dist = sPaths[mCurrentPath][mCurrentWaypoint] - Pos();
		Translate(dist.Normalized() * mTimer->DeltaTime() * mSpeed, world);

		// For the enemies to be facing the direction they are moving
		Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

		if ((sPaths[mCurrentPath][mCurrentWaypoint] - Pos()).MagnitudeSqr() < EPSILON)
			mCurrentWaypoint++;

		if (mCurrentWaypoint >= sPaths[mCurrentPath].size())
			PathComplete();
	}
	// Path finished
	else {

		Vector2 dist = WorldFormationPos() - Pos();
		Translate(dist.Normalized() * mTimer->DeltaTime() * mSpeed, world);

		Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

		//If true, position in the formation is reached
		if (dist.MagnitudeSqr() < EPSILON) {

			FlyInComplete();
		}
	}
}

void Enemy::FormationStateHandler() {

	Pos(LocalFormationPos());

	float rotation = Rotation();

	if (rotation != 0) {

		if (rotation > 2.0f) {

			float rotationSpeed = 200.0f;
			float rotationDir = (rotation == 180.0f) ? 1.0f : rotation - 180.0f;
			Rotate((rotationDir / abs(rotationDir)) * mTimer->DeltaTime() * rotationSpeed);
		}
		else {

			Rotation(0);
		}
	}
}

void Enemy::DeadStateHandler() {

	if (mDeathAnimation->isAnimating()) {

		mDeathAnimation->Update();
	}
}

void Enemy::StatesHandler() {

	switch (mCurrentState) {

	case flyIn:

		FlyInStatehandler();
		break;

	case formation:

		FormationStateHandler();
		break;

	case dive:

		DiveStateHandler();
		break;

	case dead:

		DeadStateHandler();
		break;
	}

}

void Enemy::RenderFlyInState() {

	mEnemies[0]->Render();
}

void Enemy::RenderFormationState() {

	mEnemies[sFormation->GetTick() % 2]->Render();
}

void Enemy::RenderDeadState() {

	if (mDeathAnimation->isAnimating())
		mDeathAnimation->Render();
}

void Enemy::RenderStates() {

	switch (mCurrentState)
	{
	case flyIn:

		RenderFlyInState();
		break;

	case formation:

		RenderFormationState();
		break;

	case dive:

		RenderDiveState();
		break;

	case dead:

		RenderDeadState();
		break;
	}

	PhysEntity::Render();
}

bool Enemy::IgnoreCollisions() {

	return mCurrentState == dead;
}

Enemy::STATES Enemy::GetCurrentState() {

	return mCurrentState;
}

Enemy::TYPES Enemy::GetType() {

	return mType;
}

int Enemy::GetIndex() {

	return mIndex;
}

void Enemy::Dive(int type) {

	// Break from formation
	Parent(NULL);
	mCurrentState = dive;
	mDiveStartPos = Pos();
	mCurrentWaypoint = 1;
}

bool Enemy::InDeathAnimation() {

	return mDeathAnimation->isAnimating();
}

void Enemy::Update() {

	if (Active()) {

		StatesHandler();
	}
}


void Enemy::Render() {

	if (Active())
		RenderStates();
}