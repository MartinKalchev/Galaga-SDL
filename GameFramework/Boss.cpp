#include "Boss.h"
#include "BoxCollider.h"

std::vector<std::vector<Vector2>> Boss::sDivePaths;

void Boss::CreateDivePaths() {

	int currentPath = 0;
	BezierPath* path = new BezierPath();

	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(-90.0f, -60.0f), Vector2(-90.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(-90.0f, 0.0f), Vector2(-90.0f, 60.0f), Vector2(-100.0f, 275.0f), Vector2(-15.0f, 275.0f) }, 15);
	path->AddCurve({ Vector2(-15.0f, 275.0f), Vector2(85.0f, 275.0f), Vector2(85.0f, 125.0f), Vector2(-15.0f, 125.0f) }, 15);
	path->AddCurve({ Vector2(-15.0f, 125.0f), Vector2(-175.0f, 125.0f), Vector2(0.0f, 450.0f), Vector2(120.0f, 450.0f) }, 25);
	path->AddCurve({ Vector2(120.0f, 450.0f), Vector2(160.0f, 450.0f), Vector2(200.0f, 500.0f), Vector2(200.0f, 550.0f) }, 15);
	path->AddCurve({ Vector2(200.0f, 550.0f), Vector2(200.0f, 540.0f), Vector2(200.0f, 810.0f), Vector2(200.0f, 800.0f) }, 1);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currentPath]);
	delete path;

	currentPath = 1;
	path = new BezierPath();

	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(90.0f, -60.0f), Vector2(90.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(90.0f, 0.0f), Vector2(90.0f, 60.0f), Vector2(100.0f, 275.0f), Vector2(15.0f, 275.0f) }, 15);
	path->AddCurve({ Vector2(15.0f, 275.0f), Vector2(-85.0f, 275.0f), Vector2(-85.0f, 125.0f), Vector2(15.0f, 125.0f) }, 15);
	path->AddCurve({ Vector2(15.0f, 125.0f), Vector2(175.0f, 125.0f), Vector2(0.0f, 450.0f), Vector2(-120.0f, 450.0f) }, 25);
	path->AddCurve({ Vector2(-120.0f, 450.0f), Vector2(-160.0f, 450.0f), Vector2(-200.0f, 500.0f), Vector2(-200.0f, 550.0f) }, 15);
	path->AddCurve({ Vector2(-200.0f, 550.0f), Vector2(-200.0f, 540.0f), Vector2(-200.0f, 810.0f), Vector2(-200.0f, 800.0f) }, 1);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currentPath]);
	delete path;

	currentPath = 2;
	path = new BezierPath();

	// Capture dive path
	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(-90.0f, -60.0f), Vector2(-90.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(-90.0f, 0.0f), Vector2(-90.0f, 60.0f), Vector2(100.0f, 340.0f), Vector2(100.0f, 400.0f) }, 15);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currentPath]);
	delete path;

	currentPath = 3;
	path = new BezierPath();

	// Capture dive path
	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(90.0f, -60.0f), Vector2(90.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(90.0f, 0.0f), Vector2(90.0f, 60.0f), Vector2(-100.0f, 340.0f), Vector2(-100.0f, 400.0f) }, 15);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currentPath]);
	delete path;
}

Boss::Boss(int index, int path, bool challengeStage)
	: Enemy(index, path, challengeStage)
{
	
	mEnemies[0] = new Texture("boss.png");
	mEnemies[0]->Parent(this);
	mEnemies[0]->Pos(VEC2_ZERO);

	mEnemies[1] = new Texture("boss2.png");
	mEnemies[1]->Parent(this);
	mEnemies[1]->Pos(VEC2_ZERO);

	mType = boss;

	mCaptureDive = false;
	mCurrentPath = 1;
	mCapturing = false;

	mCaptureBeam = new CaptureBeam();
	mCaptureBeam->Parent(this);
	mCaptureBeam->Pos(Vector2(0.0f, -190.0f));
	mCaptureBeam->Rotation(180.0f);

	AddCollider(new BoxCollider(mEnemies[1]->ScaledDimensions()));

	mWasHit = false;
}


Boss::~Boss() {

	delete mCaptureBeam;
	mCaptureBeam = NULL;
}


Vector2 Boss::LocalFormationPos() {

	Vector2 retVal;

	retVal.x = (sFormation->GridSize().x + sFormation->GridSize().x * 2 * (mIndex / 2)) * pow(-1, mIndex % 2 + 1);

	retVal.y = -sFormation->GridSize().y;

	return retVal;
}

void Boss::CaptureBeamHandler() {

	mCaptureBeam->Update();

	if (!mCaptureBeam->isAnimating()) {

		Translate(VEC2_UP * mSpeed * mTimer->DeltaTime(), world);

		if (Pos().y >= 910.0f) {

			Pos(Vector2(WorldFormationPos().x, -20.0f));
			mCapturing = false;
		}
	}
}

void Boss::DiveStateHandler() {

	
	if (mCurrentWaypoint < sDivePaths[mCurrentPath].size()) {

		Vector2 waypointPos = mDiveStartPos + sDivePaths[mCurrentPath][mCurrentWaypoint];

		Vector2 dist = waypointPos - Pos();
		Translate(dist.Normalized() * mTimer->DeltaTime() * mSpeed, world);

		Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

		if ((waypointPos - Pos()).MagnitudeSqr() < EPSILON)
			mCurrentWaypoint++;

		// If butterfly reaches end of screen teleport to top of screen and in to formation
		if (mCurrentWaypoint == sDivePaths[mCurrentPath].size()) {

			if (mCaptureDive) {

				mCapturing = true;
				Rotation(180.0f);
			}
			else {

				Pos(Vector2(WorldFormationPos().x, -20.0f));
			}
			
		}
			
	}
	else {

		if (!mCaptureDive || !mCapturing) {

			Vector2 dist = WorldFormationPos() - Pos();
			Translate(dist.Normalized() * mTimer->DeltaTime() * mSpeed, world);
			Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

			if (dist.MagnitudeSqr() < EPSILON)
				JoinFormation();
		}
		else {

			CaptureBeamHandler();
		}
		
	}
}


void Boss::RenderDiveState() {

	mEnemies[0]->Render();


	if (mCapturing && mCaptureBeam->isAnimating())
		mCaptureBeam->Render();
}


void Boss::Dive(int type) {

	mCaptureDive = type != 0;

	Enemy::Dive();

	if (mCaptureDive) {

		mCapturing = false;
		mCurrentPath = 2 + rand() % 2;
		mCaptureBeam->ResetAnimation();
	}
	else {

		mCurrentPath = mIndex % 2;
	}
}

void Boss::Hit(PhysEntity* other) {

	if (mWasHit) {

		Enemy::Hit(other);
	}
	else {

		mWasHit = true;
		delete mEnemies[0];
		mEnemies[0] = new Texture("bosshit.png");
		mEnemies[0]->Parent(this);
		mEnemies[0]->Pos(VEC2_ZERO);
		mEnemies[0]->Rotation(0.0f);

		delete mEnemies[1];
		mEnemies[1] = new Texture("bosshit2.png");
		mEnemies[1]->Parent(this);
		mEnemies[1]->Pos(VEC2_ZERO);
		mEnemies[1]->Rotation(0.0f);

		sPlayer->AddScore(mCurrentPath == Enemy::formation ? 150 : mCaptureDive ? 400 : 800);


	}
}