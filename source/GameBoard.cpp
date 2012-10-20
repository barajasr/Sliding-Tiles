#include <SFML/Graphics.hpp>
#include <new>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "GameBoard.hpp"

GameBoard::GameBoard(sf::RenderWindow* root, unsigned levelToLoad)
:screen(root), level(levelToLoad){
	try{
		texture = new sf::Texture();
		for(unsigned y(0); y < numOfTiles; y++)
			for(unsigned x(0); x < numOfTiles; x++)
				answerBoard[x][y] = new sf::Sprite();
			
	}catch(std::bad_alloc& exception){
		error = true;
		std::cerr << "Gameboard class exception raised while creating members: "
                  << exception.what() << std::endl;
		return;
	}

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
			answerBoard[x][y]->setTexture(*texture);
			answerBoard[x][y]->setTextureRect(subrect);
		
			answerBoard[x][y]->setPosition(subrect.left, subrect.top);
			gameBoard[x][y] = answerBoard[x][y];

		}
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

bool GameBoard::swapTilesAnimation(){
	sf::Vector2i toMove(blankSpot);
	tileToMove(toMove);
	if (toMove.x < 0 || toMove.x >= (int)numOfTiles ||
        toMove.y < 0 || toMove.y >= (int)numOfTiles)
		return false;

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
	return true;
}


bool GameBoard::update(){
	toBeUpdated = false;
	bool changedBoard = swapTilesAnimation();
	slideDirection = none;
	return changedBoard;
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

void GameBoard::determineSlideDirection(const sf::Vector2u& tile){
	if (blankSpot.y == tile.y && abs(blankSpot.x - tile.x) == 1){
		toBeUpdated = true;
		slideDirection = (blankSpot.x < tile.x ? left : right);
	}else if ( blankSpot.x == tile.x && abs(blankSpot.y - tile.y) == 1){
		toBeUpdated = true;
		slideDirection = (blankSpot.y < tile.y ? up : down);
	}else {
		slideDirection = none;
	}
}

void GameBoard::draw() const{
	for (unsigned x(0); x < numOfTiles; x++)
		for (unsigned y(0); y < numOfTiles; y++)
				screen->draw(*gameBoard[x][y]);
}

void GameBoard::levelUp(){
	++level;
	if (level >= maxLevel){
		allLevelsCompleted = true;
		level = 0;
	}
	loadLevel();
}

void GameBoard::loadLevel(){
		if (!texture->loadFromFile(imageToLoad())){
			error = true;
			std::cerr << "Failed to load image for level.\n";
			return;
		}
}

void GameBoard::processEvent(const sf::Keyboard::Key& key){
	if (key == sf::Keyboard::Up){
		toBeUpdated = true;
		slideDirection = up;
	}else if (key == sf::Keyboard::Down){
		toBeUpdated = true;
		slideDirection = down;
	}else if (key == sf::Keyboard::Left){
		toBeUpdated = true;
		slideDirection = left;
	}else if (key == sf::Keyboard::Right){
		toBeUpdated = true;
		slideDirection = right;
	}
		
}

void GameBoard::processEvent(const sf::Mouse::Button& button){
	if (button == sf::Mouse::Left){
		sf::Vector2i position = sf::Mouse::getPosition(*screen);
		sf::Vector2u tilePosition;
		tilePosition.x = position.x / tileSize;
		tilePosition.y = position.y / tileSize;

		if (tilePosition.x < (tileSize * numOfTiles) 
            && tilePosition.y < (tileSize * numOfTiles))
			determineSlideDirection(tilePosition);	
	}
}

void GameBoard::reset(){
	allLevelsCompleted = false;
	setlevel(0);
	loadLevel();
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


