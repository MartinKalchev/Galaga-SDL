#ifndef _SCOREBOARD_H
#define _SCOREBOARD_H
#include "Texture.h"
#include <vector>

using namespace QuickSDL;

class Scoreboard : public GameEntity {

private:

	std::vector<Texture*> mScore;

	SDL_Color mColor;

public:

	Scoreboard();
	Scoreboard(SDL_Color color);
	~Scoreboard();

	void SetScore(int score);

	void Render();

private:

	void ClearScoreboard();
};
#endif // !_SCOREBOARD_H

