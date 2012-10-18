#include <SFML/Graphics.hpp>
#include <new>
#include <iostream>

#include "Game.hpp"
#include "GameBoard.hpp"

Game::Game(){
	window = new(std::nothrow) sf::RenderWindow(sf::VideoMode(screenHeight, screenWidth), 
                                  "Hex Puzzle", 
                                  sf::Style::Close);
	if (!window){
		errors = true;
		std::cerr << "Not enough memory to create screen.\n";
		return;
	}
	window->setFramerateLimit(60);

	icon = new(std::nothrow) sf::Image();
	if(!icon){
		errors = true;
		std::cerr << "Not enough to creae icon.\n";
		return;
	}
	if (!icon->loadFromFile("data/icon.png")){
		std::cerr << "ERROR: Failed to load icon; data/icon.png\n";
			errors = true;
			return;
	}
	window->setIcon(32,32,icon->getPixelsPtr());

	puzzle = new(std::nothrow) GameBoard(window);	
	if(!puzzle){
		errors = true;
		std::cerr << "Not enough to creae puzzle gameboard.\n";
		return;
	}
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
			sf::Time sleepTime = sf::seconds(2.0f);
			sf::sleep(sleepTime);
			return;
		}
		
		while (window->pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window->close();
			else if (event.type == sf::Event::KeyPressed)
				puzzle->processEvent(event.key.code);
			else if (event.type == sf::Event::MouseButtonPressed)
				puzzle->processEvent(event.mouseButton.button);
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

