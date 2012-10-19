#include <SFML/Graphics.hpp>
#include <iostream>
#include <new>

#include "Score.hpp"

Score::Score(sf::RenderWindow* root): screen(root){
	try{
		backInterface = new sf::Texture();
		background = new sf::Sprite();
		numbersList = new sf::Texture();

		ones = new sf::Sprite();
		tens = new sf::Sprite();
		hundreds = new sf::Sprite();
		thousands = new sf::Sprite();

		for (unsigned index(0); index < numbers.size(); index++)
			numbers[index] = new sf::Sprite();

	}catch(std::bad_alloc& exception){
		std::cerr << "Score class exception raised while creating game members: "
                  << exception.what() << std::endl;
		error = true;
		return;

	}
	
	if (!backInterface->loadFromFile("data/background.png")){
		error = true;
		std::cerr << "Error: Failed to load \"data/background.png\"\n";
		return;
	}
	background->setTexture(*backInterface);

	unsigned numberImageSize = 28;
	if (numbersList->loadFromFile("data/numbers.png")){
		sf::IntRect rect(0, 0, numberImageSize, numberImageSize);
		for (unsigned index(0); index < numbers.size(); index++){
			rect.left = index * numberImageSize;
			numbers[index]->setTexture(*numbersList);
			numbers[index]->setTextureRect(rect);
		}
	}else{
		error = true;
		std::cerr << "Error: Failed to load \"data/numbers.png\"\n";
		return;
	}

	*ones = *numbers.at(0);
	*tens = *numbers.at(0);
	*hundreds = *numbers.at(0);
	*thousands = *numbers.at(0);

	sf::Vector2f position(560.0f, 202.0f);
	thousands->setPosition(position);
	position.x += numberImageSize;
	hundreds->setPosition(position);
	position.x += numberImageSize;
	tens->setPosition(position);
	position.x += numberImageSize;
	ones->setPosition(position);

	
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

