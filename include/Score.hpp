#ifndef SCORE_HPP
#define SCORE_HPP

#include <vector>

namespace sf{
	class RenderWindow;
	class Sprite;
	class Texture;
}

class Score{
	private:
	bool error = false;
	sf::RenderWindow* screen = nullptr;
	sf::Sprite* ones = nullptr;
	sf::Sprite* tens = nullptr;
	sf::Sprite* hundreds = nullptr;
	sf::Sprite* thousands = nullptr;
	sf::Sprite* background = nullptr;
	std::vector<sf::Sprite*> numbers = {nullptr, nullptr, nullptr, nullptr,
                                       nullptr, nullptr, nullptr, nullptr,
                                       nullptr, nullptr};
	sf::Texture* backInterface = nullptr;
	sf::Texture* numbersList = nullptr;
	unsigned score = 0;

	void incrementScore();
	void incrementOnes();
	void incrementTens();
	void incrementHundreds();
	void incrementThousands();
	
	public:
	Score(sf::RenderWindow* root);
	~Score();
	bool hasError() const {return error;};
	void draw();
	void update(){incrementScore();}

};

#endif

