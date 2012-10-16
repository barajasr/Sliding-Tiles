#ifndef GAME_HPP
#define GAME_HPP

class GameBoard;

namespace sf{
	class Image;
	class RenderWindow;
}

class Game{
	private:
	bool errors;
	sf::RenderWindow* window;
	sf::Image* icon;
	GameBoard* puzzle;
	const unsigned screenHeight;
	const unsigned screenWidth;

	void playLevel();

	public:
	Game();
	~Game();
	void run();
};

#endif

