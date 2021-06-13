#include "Butterfly.h"
#include "BoxCollider.h"
#include "CircleCollider.h"

std::vector<std::vector<Vector2>> Butterfly::sDivePaths;

void Butterfly::CreateDivePaths() {

	int currenPath = 0;
	BezierPath* path = new BezierPath();


	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -45.0f), Vector2(-60.0f, -45.0f), Vector2(-60.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(-60.0f, 0.0f), Vector2(-60.0f, 80.0f), Vector2(200.0f, 125.0f), Vector2(200.0f, 200.0f) }, 15);
	path->AddCurve({ Vector2(200.0f, 200.0f), Vector2(200.0f, 275.0f), Vector2(175.0f, 250.0f), Vector2(175.0f, 325.0f) }, 15);
	path->AddCurve({ Vector2(175.0f, 325.0f), Vector2(175.0f, 425.0f), Vector2(375.0f, 425.0f), Vector2(375.0f, 525.0f) }, 15);
	path->AddCurve({ Vector2(375.0f, 525.0f), Vector2(375.0f, 575.0f), Vector2(300.0f, 625.0f), Vector2(300.0f, 775.0f) }, 15);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currenPath]);

	delete path;

	currenPath = 1;
	path = new BezierPath();

	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -45.0f), Vector2(60.0f, -45.0f), Vector2(60.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(60.0f, 0.0f), Vector2(60.0f, 80.0f), Vector2(-200.0f, 125.0f), Vector2(-200.0f, 200.0f) }, 15);
	path->AddCurve({ Vector2(-200.0f, 200.0f), Vector2(-200.0f, 275.0f), Vector2(-175.0f, 250.0f), Vector2(-175.0f, 325.0f) }, 15);
	path->AddCurve({ Vector2(-175.0f, 325.0f), Vector2(-175.0f, 425.0f), Vector2(-375.0f, 425.0f), Vector2(-375.0f, 525.0f) }, 15);
	path->AddCurve({ Vector2(-375.0f, 525.0f), Vector2(-375.0f, 575.0f), Vector2(-300.0f, 625.0f), Vector2(-300.0f, 775.0f) }, 15);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currenPath]);
	delete path;

	// Escort path for the boss dive
	currenPath = 2;
	path = new BezierPath();

	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(-90.0f, -60.0f), Vector2(-90.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(-90.0f, 0.0f), Vector2(-90.0f, 60.0f), Vector2(-100.0f, 275.0f), Vector2(-15.0f, 275.0f) }, 15);
	path->AddCurve({ Vector2(-15.0f, 275.0f), Vector2(85.0f, 275.0f), Vector2(85.0f, 125.0f), Vector2(-15.0f, 125.0f) }, 15);
	path->AddCurve({ Vector2(-15.0f, 125.0f), Vector2(-175.0f, 125.0f), Vector2(0.0f, 450.0f), Vector2(120.0f, 450.0f) }, 25);
	path->AddCurve({ Vector2(120.0f, 450.0f), Vector2(160.0f, 450.0f), Vector2(200.0f, 500.0f), Vector2(200.0f, 550.0f) }, 15);
	path->AddCurve({ Vector2(200.0f, 550.0f), Vector2(200.0f, 540.0f), Vector2(200.0f, 810.0f), Vector2(200.0f, 800.0f) }, 1);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currenPath]);
	delete path;

	//Escort path
	currenPath = 3;
	path = new BezierPath();

	path->AddCurve({ Vector2(0.0f, 0.0f), Vector2(0.0f, -60.0f), Vector2(90.0f, -60.0f), Vector2(90.0f, 0.0f) }, 15);
	path->AddCurve({ Vector2(90.0f, 0.0f), Vector2(90.0f, 60.0f), Vector2(100.0f, 275.0f), Vector2(15.0f, 275.0f) }, 15);
	path->AddCurve({ Vector2(15.0f, 275.0f), Vector2(-85.0f, 275.0f), Vector2(-85.0f, 125.0f), Vector2(15.0f, 125.0f) }, 15);
	path->AddCurve({ Vector2(15.0f, 125.0f), Vector2(175.0f, 125.0f), Vector2(0.0f, 450.0f), Vector2(-120.0f, 450.0f) }, 25);
	path->AddCurve({ Vector2(-120.0f, 450.0f), Vector2(-160.0f, 450.0f), Vector2(-200.0f, 500.0f), Vector2(-200.0f, 550.0f) }, 15);
	path->AddCurve({ Vector2(-200.0f, 550.0f), Vector2(-200.0f, 540.0f), Vector2(-200.0f, 810.0f), Vector2(-200.0f, 800.0f) }, 1);

	sDivePaths.push_back(std::vector<Vector2>());
	path->Sample(&sDivePaths[currenPath]);
	delete path;
}

Butterfly::Butterfly(int index, int path, bool challengeStage)
	: Enemy(index, path, challengeStage){

	mEnemies[0] = new Texture("butterfly.png");
	mEnemies[0]->Parent(this);
	mEnemies[0]->Pos(VEC2_ZERO);

	mEnemies[1] = new Texture("butterfly2.png");
	mEnemies[1]->Parent(this);
	mEnemies[1]->Pos(VEC2_ZERO);

	mType = butterfly;

	AddCollider(new BoxCollider(mEnemies[1]->ScaledDimensions()));
}

Butterfly::~Butterfly() {


}

Vector2 Butterfly::LocalFormationPos() {

	Vector2 retVal;


	retVal.x = (sFormation->GridSize().x + sFormation->GridSize().x * 2 * (mIndex / 4)) * pow(-1, mIndex % 2 + 1);

	retVal.y = sFormation->GridSize().y * ((mIndex % 4) / 2);

	return retVal;
}

void Butterfly::DiveStateHandler() {

	int currentPath = mIndex % 2;

	if (mEscort)
		currentPath += 2;

	if (mCurrentWaypoint < sDivePaths[currentPath].size()) {

		Vector2 waypointPos = mDiveStartPos + sDivePaths[currentPath][mCurrentWaypoint];

		Vector2 dist = waypointPos - Pos();
		Translate(dist.Normalized() * mTimer->DeltaTime() * mSpeed, world);

		Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

		if ((waypointPos - Pos()).MagnitudeSqr() < EPSILON)
			mCurrentWaypoint++;

		// If butterfly reaches end of screen teleport to top of screen and in to formation
		if (mCurrentWaypoint == sDivePaths[currentPath].size())
			Pos(Vector2(WorldFormationPos().x, 20.0f));
	}
	else {

		Vector2 dist = WorldFormationPos() - Pos();
		Translate(dist.Normalized() * mTimer->DeltaTime() * mSpeed, world);
		Rotation(atan2(dist.y, dist.x) * RAD_TO_DEG + 90.0f);

		if (dist.MagnitudeSqr() < EPSILON)
			JoinFormation();
	}
}

void Butterfly::RenderDiveState() {

	mEnemies[0]->Render();

}

void Butterfly::Dive(int type) {

	mEscort = type != 0;

	Enemy::Dive();
}

void Butterfly::Hit(PhysEntity* other) {

	sPlayer->AddScore(mCurrentState == Enemy::formation ? 80 : 160);
	Enemy::Hit(other);
}