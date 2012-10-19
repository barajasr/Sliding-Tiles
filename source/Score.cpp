#include <SFML/Graphics.hpp>
#include <iostream>
#include <new>

#include "Score.hpp"

Score::Score(sf::RenderWindow* root): screen(root){
	backInterface = new(std::nothrow) sf::Texture();
	if (backInterface){
		if (!backInterface->loadFromFile("data/background.png")){
			error = true;
			std::cerr << "Error: Failed to load \"data/background.png\"\n";
			return;
		}

		background = new(std::nothrow) sf::Sprite(*backInterface);
		if (!background){
			error = true;
			std::cerr << "Not enough memory to create background sprite\n";
			return;
		}
	}else{
		error = true;
		std::cerr << "Not enough memory to create backInterface texture.\n";
		return;
	}
	
	numbersList = new(std::nothrow) sf::Texture();
	if (numbersList){
		if (!numbersList->loadFromFile("data/numbers.png")){
			error = true;
			std::cerr << "Error: Failed to load \"data/numbers.png\"\n";
			return;
		}

		unsigned numberImageSize = 28;
		sf::IntRect rect(0, 0, numberImageSize, numberImageSize);
		for ( unsigned index(0); index < numbers.size(); index++){
			rect.left = index * numberImageSize;
			numbers[index] = new(std::nothrow) sf::Sprite(*numbersList, rect);
			if (!numbers[index]){
				error = true;
				std::cerr << "Not enough memory to partition numbersList into sprites\n";
				return;
			}
		}

		ones = new(std::nothrow) sf::Sprite(*numbers.at(0));
		tens = new(std::nothrow) sf::Sprite(*numbers.at(0));
		hundreds = new(std::nothrow) sf::Sprite(*numbers.at(0));
		thousands = new(std::nothrow) sf::Sprite(*numbers.at(0));
		if (ones && tens && hundreds && thousands){
			sf::Vector2f position(560.0f, 202.0f);
			thousands->setPosition(position);
			position.x += numberImageSize;
			hundreds->setPosition(position);
			position.x += numberImageSize;
			tens->setPosition(position);
			position.x += numberImageSize;
			ones->setPosition(position);
		}else{
			error = true;
			std::cerr << "Not enough memory to create individual score sprites.\n";
			return;
		}
	}else{
		error = true;
		std::cerr << "Not enough memory to create numbersList texture.\n";
		return;
	}
}

Score::~Score(){
	delete ones;
	delete tens;
	delete hundreds;
	delete thousands;
	delete background;
	delete backInterface;
	delete numbersList;

	ones = nullptr;
	tens = nullptr;
	hundreds = nullptr;
	thousands = nullptr;
	background = nullptr;
	backInterface = nullptr;
	numbersList = nullptr;
	screen = nullptr;

	for (unsigned index(0); index < numbers.size(); index++){
		delete numbers[index];
		numbers[index] = nullptr;
	}
}

void Score::draw(){
	screen->draw(*background);
	screen->draw(*ones);
	screen->draw(*tens);
	screen->draw(*hundreds);
	screen->draw(*thousands);
}

void Score::incrementScore(){
	score++;
	incrementOnes();
}

void Score::incrementOnes(){
	unsigned index = score % 10;
	sf::Vector2f position(ones->getPosition());
	*ones =  *numbers.at(index);
	ones->setPosition(position);

	if (index == 0)
		incrementTens();
}


void Score::incrementTens(){
	unsigned index = score % 100 / 10;
	sf::Vector2f position(tens->getPosition());
	*tens =  *numbers.at(index);
	tens->setPosition(position);

	if (index == 0)
		incrementHundreds();
}

void Score::incrementHundreds(){
	unsigned index = score % 1000 / 100;
	sf::Vector2f position(hundreds->getPosition());
	*hundreds =  *numbers.at(index);
	hundreds->setPosition(position);

	if (index == 0)
		incrementThousands();
}

void Score::incrementThousands(){
	unsigned index = score % 10000 / 1000;
	sf::Vector2f position(thousands->getPosition());
	*thousands =  *numbers.at(index);
	thousands->setPosition(position);

}

