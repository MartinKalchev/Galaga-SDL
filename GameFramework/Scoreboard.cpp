#include "Scoreboard.h"

Scoreboard::Scoreboard()
		: Scoreboard({230, 230, 230})
{
}

Scoreboard::Scoreboard(SDL_Color color) {

	mColor = color;
	SetScore(0);
}

Scoreboard::~Scoreboard() {

	ClearScoreboard();
}

void Scoreboard::ClearScoreboard() {

	for (int i = 0; i < mScore.size(); i++) {
		delete mScore[i];
		mScore[i] = NULL;
	}

	mScore.clear();
}

void Scoreboard::SetScore(int score) {

	ClearScoreboard();

	if (score == 0) {

		for (int i = 0; i < 2; i++) {

			mScore.push_back(new Texture("0", "emulogic.ttf", 32, mColor));

			mScore[i]->Parent(this);
			mScore[i]->Pos(Vector2(-32.0f*i, 0.0f));
		}
	}
	else {

		std::string str = std::to_string(score);
		int lastIndex = str.length() - 1;

		for (int i = 0; i <= lastIndex; i++) {

			// Adding one character at a time
			mScore.push_back(new Texture(str.substr(i, 1), "emulogic.ttf", 32, mColor));

			mScore[i]->Parent(this);
			mScore[i]->Pos(Vector2(-32.0f*(lastIndex-i), 0.0f));
		}
	}
}

void Scoreboard::Render() {

	for (int i = 0; i < mScore.size(); i++) {

		mScore[i]->Render();
	}
}