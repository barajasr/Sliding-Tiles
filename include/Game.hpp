#ifndef GAME_HPP
#define GAME_HPP

class GameBoard;
class Score;

namespace sf{
	class Image;
	class RenderWindow;
}

class Game{
	private:
	bool errors = false;
	sf::RenderWindow* window = nullptr;
	sf::Image* icon = nullptr;
	GameBoard* puzzle = nullptr;
	Score* score = nullptr;
	const unsigned screenHeight = 710;
	const unsigned screenWidth = 500;

	void playLevel();

	public:
	Game();
	~Game();
	void run();
};

#endif

