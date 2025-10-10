
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}

Game::~Game()
{
	delete character;
	delete passport;
	delete[] animals;
	delete[] passports;
}

bool Game::init()
{
	current_state = MENU;

	// TEXT //
	if(!font.loadFromFile("../Data/Fonts/OpenSans-Bold.ttf"))
	{
		std::cout << "Font didn't load\n";
	}
	// MENU TEXT //
	createText(m_title_txt, "Critter's Crossing", 100, sf::Color::White);
	m_title_txt.setPosition(window.getSize().x/2-m_title_txt.getGlobalBounds().width/2, 20);
	createText(p_pause_text, "PAUSED", 100, sf::Color::White);
	p_pause_text.setPosition(window.getSize().x / 2 - p_pause_text.getGlobalBounds().width / 2, 20);
	// SPRITES //
	if(!background_texture.loadFromFile("../Data/Images/WhackaMole Worksheet/background.png"))
	{
		std::cout << "Gameplay BG didn't load.\n";
	}
	background.setTexture(background_texture);
	pause_rect.setSize({ 1080, 720 });
	pause_rect.setFillColor(sf::Color(0, 0, 0, 150));

	character = new sf::Sprite;
	passport = new sf::Sprite;

	loadAnimals();
	loadPassports();
	
	newAnimal();

  return true;
}

void Game::update(float dt)
{
	if(started_dragging)
	{
		dragOffset(dragged);
	}
	dragSprite(dragged);
}

void Game::render()
{
	switch (current_state)
	{
	case MENU:
		//std::cout << "MENU STATE\n";
		window.draw(m_title_txt);
		break;
	case GAMEPLAY:
		//std::cout << "GAMEPLAY STATE\n";
		window.draw(background);
		window.draw(*character);
		window.draw(*passport);

		if(paused)
		{
			window.draw(pause_rect);
			window.draw(p_pause_text);
		}
		break;
	} 
}

void Game::mouseClicked(sf::Event event)
{
  if(event.mouseButton.button == sf::Mouse::Left)
  {
	  sf::Vector2i click = sf::Mouse::getPosition(window);
	  sf::Vector2f clickf = static_cast<sf::Vector2f>(click);

	  //If The click's location is within the passport
	  if (passport->getGlobalBounds().contains(clickf))
	  {
		  dragged = passport;
		  started_dragging = true;
	  }
  }
 
}
void Game::mouseReleased(sf::Event event)
{
	dragged = nullptr;
	started_dragging = false;
  //get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);
}
void Game::mouseReleased(sf::Event event)
{

}

void Game::keyPressed(sf::Event event)
{
	switch (current_state)
	{
	case MENU:
		if(event.key.code == sf::Keyboard::Enter)
		{
			current_state = GAMEPLAY;
		}
		if(event.key.code == sf::Keyboard::Escape)
		{
			window.close();
		}
		break;
	case GAMEPLAY:
		if (paused)
		{
			if(event.key.code == sf::Keyboard::Escape)
			{
				paused = false;
				break;
			}
			if (event.key.code == sf::Keyboard::Enter)
			{
				paused = false;
				current_state = MENU;
				break;
			}
		}
		if (event.key.code == sf::Keyboard::Escape)
		{
			paused = true;
		}
		if (event.key.code == sf::Keyboard::R)
		{
			newAnimal();
		}
		break;
	}
}
void Game::keyReleased(sf::Event event)
{

}

// Private Functions

void Game::newAnimal()
{
	passport_accepted = false;
	passport_rejected = false;

	int animal_index = rand() % 3;
	int passport_index = rand() % 3;

	if(animal_index == passport_index)
	{
		should_accept = true;
	}
	else
	{
		should_accept = false;
	}
	character->setTexture(animals[animal_index], true);
	character->setScale(1.8, 1.8);
	character->setPosition(window.getSize().x / 4 - (character->getGlobalBounds().width/2), window.getSize().y / 4 - (character->getGlobalBounds().height / 2));

	passport->setTexture(passports[passport_index], true);
	passport->setScale(0.6, 0.6);
	passport->setPosition(window.getSize().x/2,window.getSize().y/3);
}

void Game::dragSprite(sf::Sprite* sprite)
{
	if(sprite != nullptr)
	{
		sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
		sf::Vector2f mouse_positionf = static_cast<sf::Vector2f>(mouse_position);
		sf::Vector2f drag_position = mouse_positionf - drag_offset;// (Makes it so it drags from any pos of the mouse on sprite
		sprite->setPosition(drag_position.x, drag_position.y);
	}
}

void Game::dragOffset(sf::Sprite* sprite)
{
	if(sprite != nullptr)
	{
		sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
		sf::Vector2f mouse_positionf = static_cast<sf::Vector2f>(mouse_position);
		float x = mouse_positionf.x - sprite->getPosition().x;
		float y = mouse_positionf.y - sprite->getPosition().y;
		drag_offset = { x,y };
		started_dragging = false;
	}

}


void Game::createText(sf::Text& text_name, std::string text, int size, sf::Color colour)
{
	text_name.setString(text);
	text_name.setFont(font);
	text_name.setCharacterSize(size);
	text_name.setFillColor(colour);
}

void Game::changeText(sf::Text& text_name, std::string new_text, int temp_number)
{
	if(temp_number != NULL)
	{
		new_text += std::to_string(temp_number);
	}
	text_name.setString(new_text);
}

void Game::loadAnimals()
{
	if(!animals[0].loadFromFile("../Data/Images/Critter Crossing Customs/elephant.png"))
	{
		std::cout << "Elephant didn't load\n";
	}
	if (!animals[1].loadFromFile("../Data/Images/Critter Crossing Customs/moose.png"))
	{
		std::cout << "Moose didn't load\n";
	}
	if (!animals[2].loadFromFile("../Data/Images/Critter Crossing Customs/penguin.png"))
	{
		std::cout << "Penguin didn't load\n";
	}
}

void Game::loadPassports()
{
	if (!passports[0].loadFromFile("../Data/Images/Critter Crossing Customs/elephant passport.png"))
	{
		std::cout << "Elephant Passport didn't load\n";
	}
	if (!passports[1].loadFromFile("../Data/Images/Critter Crossing Customs/moose passport.png"))
	{
		std::cout << "Moose Passport didn't load\n";
	}
	if (!passports[2].loadFromFile("../Data/Images/Critter Crossing Customs/penguin passport.png"))
	{
		std::cout << "Penguin Passport didn't load\n";
	}
}