#ifndef _STAR_H
#define _STAR_H
#include "Timer.h"
#include "Texture.h"

using namespace QuickSDL;

class Star : public Texture {

private:

	static bool sScroll;

	Timer* mTimer;
	bool mVisible; // used for the flickering of the stars

	float mFlickerTimer;
	float mFLickerSpeed;

	float mScrollSpeed;
public:

	static void Scroll(bool b);

	Star(int layer);
	~Star();

	void Update();
	void Render();

private:

	void ScrollStar();

};
#endif // !_STAR_H

