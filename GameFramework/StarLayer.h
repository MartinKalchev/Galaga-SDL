#ifndef _STARLAYER_H
#define _STARLAYER_H
#include "Star.h"

class StarLayer {

private:

	//Array properties for the stars
	static const int STAR_COUNT = 30;

	Star* mStars[STAR_COUNT];

public:

	StarLayer(int layer);
	~StarLayer();

	void Update();
	void Render();

};
#endif // !_STARLAYER_H
