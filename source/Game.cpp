#include <SFML/Graphics.hpp>
#include <iostream>

#include "Game.hpp"
#include "GameBoard.hpp"

Game::Game():errors(false), screenHeight(500), screenWidth(500){
	window = new sf::RenderWindow(sf::VideoMode(screenHeight, screenWidth), 
                                  "Hex Puzzle", 
                                  sf::Style::Close);
	window->setFramerateLimit(60);

	icon = new sf::Image();
	if (!icon->loadFromFile("data/icon.png")){
		std::cerr << "ERROR: Failed to load icon; data/icon.png\n";
			errors = true;
			return;
	}
	window->setIcon(32,32,icon->getPixelsPtr());

	puzzle = new GameBoard(window);	
}

Game::~Game(){
	delete window;
	delete puzzle;
	delete icon;
	window = nullptr;
	puzzle = nullptr;
	icon = nullptr;
}

void Game::playLevel(){
	sf::Event event;
	while (window->isOpen()){
		if (puzzle->isSolved()){
			return;
		}
		
		while (window->pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window->close();
			else if (event.type == sf::Event::KeyPressed)
				puzzle->processEvent(&event);
		}

		if (puzzle->needsUpdate()){
			puzzle->update();
		}

		window->clear();
		puzzle->draw();
		window->display();
	}
}

void Game::run(){
	if (this->errors || (!window && !puzzle) || puzzle->hasError())
		return;

	sf::Event event;
	while (window->isOpen() && !puzzle->levelsCompleted()){
		while (window->pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window->close();
			else if (event.type == sf::Event::KeyPressed){
				if (event.key.code == sf::Keyboard::Return){
					puzzle->shuffleBoard();
					playLevel();
					if (puzzle->hasError() || !window->isOpen())
						return;
					if (puzzle->isSolved())
						puzzle->levelUp();
				}
			}
		}
		window->clear();
		puzzle->draw();
		window->display();
	}
}

