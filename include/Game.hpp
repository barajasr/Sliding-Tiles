#ifndef GAME_HPP
#define GAME_HPP

class GameBoard;

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
	const unsigned screenHeight = 500;
	const unsigned screenWidth = 500;

	void playLevel();

	public:
	Game();
	~Game();
	void run();
};

#endif

