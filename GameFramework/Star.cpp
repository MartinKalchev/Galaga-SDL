#include "Star.h"


bool Star::sScroll = false;

void Star::Scroll(bool b) {
	sScroll = b;
}


Star::Star(int layer)
	: Texture("stars.png", 0, 0, 4, 4)
{

	mTimer = Timer::Instance();

	int startColor = rand() % 4;

	mClipRect.x = startColor * 4;

	Pos(Vector2(rand() % Graphics::Instance()->SCREEN_WIDTH, rand() % Graphics::Instance()->SCREEN_HEIGHT));

	mFlickerTimer = 0.0f;

	//Starts from min to max flicker speed
	mFLickerSpeed = 0.15f + ((float)rand() / RAND_MAX) * 0.45f;

	float invLayer = 1.0f / layer;
	Scale(VEC2_ONE * invLayer);

	// Faster for the closer layers and longer for the further ones
	mScrollSpeed = 4.0f / layer;

}

Star::~Star() {

	mTimer = NULL;
}

void Star::ScrollStar() {

	Translate(VEC2_UP * mScrollSpeed, world);

	Vector2 pos = Pos(local);

	if (pos.y > Graphics::Instance()->SCREEN_HEIGHT) {
		pos.y = 0.0f;
		pos.x = rand() % Graphics::Instance()->SCREEN_WIDTH;
		Pos(pos);
	}
}


void Star::Update() {

	mFlickerTimer += mTimer->DeltaTime();
	if (mFlickerTimer >= mFLickerSpeed) {

		mVisible = !mVisible;
		mFlickerTimer = 0.0f;
	}

	if (sScroll)
		ScrollStar();
}

void Star::Render() {

	if (mVisible)
		Texture::Render();
}

