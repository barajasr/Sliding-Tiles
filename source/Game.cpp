#include <SFML/Graphics.hpp>
#include <new>
#include <iostream>

#include "Game.hpp"
#include "GameBoard.hpp"
#include "Score.hpp"

Game::Game(){
	try{
		window = new sf::RenderWindow(sf::VideoMode(screenHeight, screenWidth), 
                                  "Hex Puzzle", 
	                             sf::Style::Close);
		icon = new sf::Image();
		puzzle = new GameBoard(window);	
		score = new Score(window);
	}catch(std::bad_alloc& exception){
		std::cerr << "Game class exception raised while creating game members: "
                  << exception.what() << std::endl;
		errors = true;
		return;
	}

	window->setFramerateLimit(60);

	if (!icon->loadFromFile("data/icon.png")){
		std::cerr << "ERROR: Failed to load icon; data/icon.png\n";
			errors = true;
			return;
	}
	window->setIcon(32,32,icon->getPixelsPtr());
}

Game::~Game(){
	delete window;
	delete puzzle;
	delete score;
	delete icon;
	window = nullptr;
	puzzle = nullptr;
	score = nullptr;
	icon = nullptr;
}

void Game::gameFinishedScreen(){
	sf::Event event;
	sf::Clock clock;
	while (window->isOpen()){
		while (window->pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window->close();
			else if (event.type == sf::Event::KeyPressed){
				if (event.key.code == sf::Keyboard::Return)
					return;
				else if (event.key.code == sf::Keyboard::Escape)
					window->close();
			}
		}

		if (clock.getElapsedTime().asSeconds() >= 2.0f){
			puzzle->levelUp();
			clock.restart();
		}

		window->clear();
		score->draw();
		puzzle->draw();
		window->display();
	}
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
			else if (event.type == sf::Event::KeyPressed){
				puzzle->processEvent(event.key.code);
				if (event.key.code == sf::Keyboard::Escape)
					window->close();
			}else if (event.type == sf::Event::MouseButtonPressed)
				puzzle->processEvent(event.mouseButton.button);
		}

		if (puzzle->needsUpdate()){
			if (puzzle->update())
				score->update();
		}

		window->clear();
		score->draw();
		puzzle->draw();
		window->display();
	}
}

void Game::run(){
	if (errors || puzzle->hasError() || score->hasError())
		return;

	sf::Event event;
	while (window->isOpen()){
		while (window->pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window->close();
			else if (event.type == sf::Event::KeyPressed){
				if (event.key.code == sf::Keyboard::Return){
					puzzle->shuffleBoard();
					playLevel();

					if (!window->isOpen())
						return;

					puzzle->levelUp();
					if (puzzle->hasError())
						return;
					if (puzzle->levelsCompleted()){
						gameFinishedScreen();
						if (window->isOpen()){
							puzzle->reset();
							score->reset();
						}
					}	
				}else if (event.key.code == sf::Keyboard::Escape)
					window->close();
			}
		}
		window->clear();
		score->draw();
		puzzle->draw();
		window->display();
	}
}

