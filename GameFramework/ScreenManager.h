#ifndef _SCREENMANAGER_H
#define _SCREENMANAGER_H
#include "StartScreen.h"
#include "PlayScreen.h"

//Singleton class
class ScreenManager {

private:

	//Identify each screen we are switching
	enum SCREENS {start, play};

	static ScreenManager* sInstance;

	InputManager* mInput;
	BackgroundStars* mBackgroundStars;
	StartScreen* mStartScreen;
	PlayScreen* mPlayScreen;

	SCREENS mCurrentScreen;


public:

	static ScreenManager* Instance();
	static void Release();

	void Update();
	void Render();

private:

	ScreenManager();
	~ScreenManager();
};

#endif // !1

