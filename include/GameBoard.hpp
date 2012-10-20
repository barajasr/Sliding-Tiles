#ifndef __GAMEOBJECT_HPP__
#define __GAMEOBJECT_HPP__

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>


const unsigned numOfTiles(4);

class GameBoard{
	private:
	enum SlideDirection{
		none,
		left,
		right,
		down,
		up
	};
	bool allLevelsCompleted = false;
	bool error = false;
	bool toBeUpdated = false;
	sf::RenderWindow* screen = nullptr;
	sf::Sprite* answerBoard[numOfTiles][numOfTiles];
	sf::Sprite* gameBoard[numOfTiles][numOfTiles];
	sf::Texture* texture = nullptr;
	sf::Vector2u blankSpot = {3, 3};
	SlideDirection slideDirection = none;
	unsigned level = 0;
	const unsigned maxLevel = 7;
	const unsigned tileSize = 125;
	
	bool swapTilesAnimation();
	std::string imageToLoad();
	void determineSlideDirection(const sf::Vector2u& tile);
	void loadLevel();
	void setlevel(unsigned level) {this->level = level;}
	void swapTiles();
	void tileAnimation(const sf::Vector2u& tileToMove);
	void tileToMove(sf::Vector2i& tileLocation);
	
	public:
	GameBoard(sf::RenderWindow* root, unsigned levelToLoad=0);
	~GameBoard();
	bool levelsCompleted() const { return allLevelsCompleted;}
	bool hasError() const { return error;}
	bool isSolved() const;
	bool needsUpdate() const{ return toBeUpdated;}
	bool update();
	void draw() const;
	void levelUp();
	void processEvent(const sf::Keyboard::Key& key);
	void processEvent(const sf::Mouse::Button& button);
	void reset();
	void shuffleBoard();
	void setSlideDirection(const SlideDirection& direction){
		slideDirection = direction;
	} 
};

#endif

