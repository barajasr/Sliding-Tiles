#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "GameBoard.hpp"

GameBoard::GameBoard(sf::RenderWindow* root, unsigned levelToLoad)
:allLevelsCompleted(false),
loadError(false),
toBeUpdated(false),
screen(root),
texture(nullptr),
blankSpot(0,0),
level(levelToLoad),
maxLevel(7),
tileSize(125){
	texture = new sf::Texture();
	loadLevel();
	if (hasError())
		return;

	// Partition image to individual tiles for gameplay
	sf::IntRect subrect;
	subrect.width = subrect.height = tileSize;
	for (unsigned y(0); y < numOfTiles; y++)
		for (unsigned x(0); x < numOfTiles; x++){
			subrect.left = tileSize * x;
			subrect.top = tileSize * y;
			subrect.width = tileSize;
			subrect.height = tileSize;
			answerBoard[x][y] = new sf::Sprite(*texture, subrect);
			answerBoard[x][y]->setPosition(subrect.left, subrect.top);
			gameBoard[x][y] = answerBoard[x][y];
		}

	// Assign empty tile to both matrices
	blankSpot.x = blankSpot.y = 3;

}

GameBoard::~GameBoard(){
	for (unsigned x(0); x < numOfTiles; x++)
		for (unsigned y(0); y < numOfTiles; y++){
			delete answerBoard[x][y];
			answerBoard[x][y] = gameBoard[x][y] = nullptr;
		}
	delete texture;
}

bool GameBoard::isSolved() const{
	if (answerBoard[numOfTiles-1][numOfTiles-1] != gameBoard[numOfTiles-1][numOfTiles-1])
		return false;
	else{
		for (unsigned y(0); y < numOfTiles; y++)
			for (unsigned x(0); x < numOfTiles; x++)
				if (answerBoard[x][y] != gameBoard[x][y]){
					return false;
				}
	} 
	return true;
}

std::string GameBoard::imageToLoad(){
	std::string filename;
	switch(level){
	default:
		level = 0; 		// reset
	case 0:
		filename = "data/tiles.png";
		break;
	case 1:
		filename = "data/Baby_Yoshi.png";
		break;
	case 2:
		filename = "data/70K.png";
		break;
	case 3:
		filename = "data/Coney.png";
		break;
	case 4:
		filename = "data/Harley.png";
		break;
	case 5:
		filename = "data/Alice_Callie.png";
		break;
	case 6:
		filename = "data/Black_Forest_Cake.png";
		break;
	case 7:
		filename = "data/Blue_Hair.png";
		break;
	}
	return filename;
}

void GameBoard::draw() const{
	for (unsigned x(0); x < numOfTiles; x++)
		for (unsigned y(0); y < numOfTiles; y++)
				screen->draw(*gameBoard[x][y]);
}

void GameBoard::levelUp(){
	++level;
	if (level > maxLevel)
		allLevelsCompleted = true;
	loadLevel();
}

void GameBoard::loadLevel(){
		if (!texture->loadFromFile(imageToLoad())){
			loadError = true;
			std::cerr << "Failed to load image for level.\n";
			return;
		}
}

void GameBoard::processEvent(sf::Event* event){
	if (event->key.code == sf::Keyboard::Up){
		toBeUpdated = true;
		slideDirection = up;
	}else if (event->key.code == sf::Keyboard::Down){
		toBeUpdated = true;
		slideDirection = down;
	}else if (event->key.code == sf::Keyboard::Left){
		toBeUpdated = true;
		slideDirection = left;
	}else if (event->key.code == sf::Keyboard::Right){
		toBeUpdated = true;
		slideDirection = right;
	}
		
}


void GameBoard::shuffleBoard(){
	std::srand(std::time(nullptr));

	for (int i(0); i < 1000; i++){
		slideDirection = SlideDirection(std::rand() % 4 + 1);
		swapTiles();
	}
	slideDirection = none;
}

void GameBoard::swapTiles(){
	sf::Vector2i toMove(blankSpot);
	tileToMove(toMove);
	if (toMove.x < 0 || toMove.x >= (int)numOfTiles ||
        toMove.y < 0 || toMove.y >= (int)numOfTiles)
		return;

	unsigned long address = reinterpret_cast<unsigned long>(gameBoard[blankSpot.x][blankSpot.y]);

	sf::Vector2f oldBlank = gameBoard[blankSpot.x][blankSpot.y]->getPosition();
	sf::Vector2f newBlank = gameBoard[toMove.x][toMove.y]->getPosition();

	gameBoard[blankSpot.x][blankSpot.y] = gameBoard[toMove.x][toMove.y];
	gameBoard[toMove.x][toMove.y] = reinterpret_cast<sf::Sprite*>(address);

	gameBoard[blankSpot.x][blankSpot.y]->setPosition(oldBlank);
	gameBoard[toMove.x][toMove.y]->setPosition(newBlank);

	blankSpot.x = toMove.x;
	blankSpot.y = toMove.y;
}

void GameBoard::swapTilesAnimation(){
	sf::Vector2i toMove(blankSpot);
	tileToMove(toMove);
	if (toMove.x < 0 || toMove.x >= (int)numOfTiles ||
        toMove.y < 0 || toMove.y >= (int)numOfTiles)
		return;

	unsigned long address = reinterpret_cast<unsigned long>(gameBoard[blankSpot.x][blankSpot.y]);

	sf::Vector2f oldBlank = gameBoard[blankSpot.x][blankSpot.y]->getPosition();
	sf::Vector2f newBlank = gameBoard[toMove.x][toMove.y]->getPosition();

	tileAnimation(sf::Vector2u(toMove));

	gameBoard[blankSpot.x][blankSpot.y] = gameBoard[toMove.x][toMove.y];
	gameBoard[toMove.x][toMove.y] = reinterpret_cast<sf::Sprite*>(address);

	gameBoard[blankSpot.x][blankSpot.y]->setPosition(oldBlank);
	gameBoard[toMove.x][toMove.y]->setPosition(newBlank);

	blankSpot.x = toMove.x;
	blankSpot.y = toMove.y;
}

void GameBoard::tileAnimation(const sf::Vector2u& tileToMove){
	sf::RectangleShape background(sf::Vector2f(numOfTiles * tileSize, 
                                               numOfTiles * tileSize));
	background.setFillColor(sf::Color::Black);

	sf::Clock clock;
	unsigned delta(tileSize * 6);
	bool animationDone(false);
	sf::Sprite* slidingTile(gameBoard[tileToMove.x][tileToMove.y]);
	sf::Vector2f blankPosition(gameBoard[blankSpot.x][blankSpot.y]->getPosition());
	sf::Vector2f position; 
	while (!animationDone){
		screen->draw(background);

		for (unsigned x(0); x < numOfTiles; x++)
			for (unsigned y(0); y < numOfTiles; y++){
				if (!((x == blankSpot.x && y == blankSpot.y) ||
				   (x == tileToMove.x && y == tileToMove.y)))
					screen->draw(*gameBoard[x][y]);
			}

		switch(slideDirection){
		case none:
			return;
		case left:
			slidingTile->move(-(delta * clock.getElapsedTime().asSeconds()),0);
			position = slidingTile->getPosition();
			if (position.x <= blankPosition.x)
				animationDone = true;
			break;
		case right:
			slidingTile->move(delta * clock.getElapsedTime().asSeconds(),0);
			position = slidingTile->getPosition();
			if (position.x >= blankPosition.x)
				animationDone = true;
			break;
		case down:
			slidingTile->move(0, delta * clock.getElapsedTime().asSeconds());
			position = slidingTile->getPosition();
			if (position.y >= blankPosition.y)
				animationDone = true;
			break;
		case up:
			slidingTile->move(0, -(delta * clock.getElapsedTime().asSeconds()));
			position = slidingTile->getPosition();
			if (position.y <= blankPosition.y)
				animationDone = true;
			break;
		}
		clock.restart();
		screen->draw(*slidingTile);
		screen->display();
	}
}

void GameBoard::tileToMove(sf::Vector2i& tileLocation){
	switch(slideDirection){
		case none:
			return;
		case up:
			tileLocation.y++;
			break;
		case down:
			tileLocation.y--;
			break;
		case left:
			tileLocation.x++;
			break;
		case right:
			tileLocation.x--;
			break;
	}
}

void GameBoard::update(){
	toBeUpdated = false;
	swapTilesAnimation();
	slideDirection = none;
}

