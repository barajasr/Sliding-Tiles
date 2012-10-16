#ifndef __GAMEOBJECT_HPP__
#define __GAMEOBJECT_HPP__

#include <SFML/System/Vector2.hpp>
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
	bool allLevelsCompleted;
	bool loadError;
	bool toBeUpdated;
	sf::RenderWindow* screen;
	sf::Sprite* answerBoard[numOfTiles][numOfTiles];
	sf::Sprite* gameBoard[numOfTiles][numOfTiles];
	sf::Texture* texture;
	sf::Vector2u blankSpot;
	SlideDirection slideDirection;
	unsigned level;
	const unsigned maxLevel;
	const unsigned tileSize;
	
	std::string imageToLoad();
	void loadLevel();
	void swapTiles();
	void swapTilesAnimation();
	void tileAnimation(const sf::Vector2u& tileToMove);
	void tileToMove(sf::Vector2i& tileLocation);
	
	public:
	GameBoard(sf::RenderWindow* root, unsigned levelToLoad=0);
	~GameBoard();
	bool levelsCompleted() const { return allLevelsCompleted;}
	bool hasError() const { return loadError;}
	bool isSolved() const;
	bool needsUpdate() const{ return toBeUpdated;}
	void draw() const;
	void levelUp();
	void processEvent(sf::Event* event);
	void shuffleBoard();
	void setSlideDirection(const SlideDirection& direction){
		slideDirection = direction;
	} 
	void update();
};

#endif

