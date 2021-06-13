#ifndef  _FORMATION_H
#define _FORMATION_H
#include "GameEntity.h"
#include "Timer.h"

using namespace QuickSDL;

class Formation : public GameEntity {

private:

	Timer* mTimer;

	// How much it transforms evey thick
	float mOffsetAmount;
	float mOffsetTimer;
	float mOffsetDelay;
	int mOffsetCounter;
	int mOffsetDirection;

	float mSpreadTimer;
	float mSpreadDelay;
	int mSpreadCounter;
	int mSpreadDirection;

	// How far apart every enemy is in the formation
	Vector2 mGridSize;

	bool mLocked;
public:

	Formation();
	~Formation();


	Vector2 GridSize();

	int GetTick();

	//Lock the formation
	void Lock();

	bool Locked();

	void Update();

};
#endif