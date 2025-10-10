
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
	createText(p_pause_txt, "PAUSED", 100, sf::Color::White);
	p_pause_txt.setPosition(window.getSize().x / 2 - p_pause_txt.getGlobalBounds().width / 2, 20);
	createText(g_lives_txt, "Lives: 3", 30, sf::Color::Red);
	g_lives_txt.setPosition(5, 5);
	createText(e_end_txt, "You got too many stamps incorrect.", 60, sf::Color::White);
	e_end_txt.setPosition(window.getSize().x/2 - e_end_txt.getGlobalBounds().width/2, 20);
	createText(e_final_score_txt, "Correct Stamps: 0", 100, sf::Color::White);
	e_final_score_txt.setPosition(window.getSize().x / 2 - e_final_score_txt.getGlobalBounds().width / 2, window.getSize().y / 2 - e_final_score_txt.getGlobalBounds().height / 2);
	// SPRITES //
	if(!background_texture.loadFromFile("../Data/Images/WhackaMole Worksheet/background.png"))
	{
		std::cout << "Gameplay BG didn't load.\n";
	}
	background.setTexture(background_texture);
	pause_rect.setSize({ 1080, 720 });
	pause_rect.setFillColor(sf::Color(0, 0, 0, 150));
	return_rect.setSize({1080/2,720/2});
	return_rect.setFillColor(sf::Color(20, 33, 160));

	character = new sf::Sprite;
	passport = new sf::Sprite;
	loadAnimals();
	loadPassports();
	newAnimal();
	// STAMPS n BUTTONS //
	reject_btn.initaliseSprite("../Data/Images/Critter Crossing Customs/reject button.png");
	reject_btn.getSprite()->setPosition(window.getSize().x - reject_btn.getSprite()->getGlobalBounds().width - 20,
		window.getSize().y - reject_btn.getSprite()->getGlobalBounds().height - 20);
	accept_btn.initaliseSprite("../Data/Images/Critter Crossing Customs/accept button.png");
	accept_btn.getSprite()->setPosition(reject_btn.getSprite()->getPosition().x - accept_btn.getSprite()->getGlobalBounds().width - 40,
		window.getSize().y - accept_btn.getSprite()->getGlobalBounds().height - 20);

	reject_stamp.initaliseSprite("../Data/Images/Critter Crossing Customs/reject.png");
	accept_stamp.initaliseSprite("../Data/Images/Critter Crossing Customs/accept.png");
	updateStampPos();

  return true;
}

void Game::update(float dt)
{
	switch (current_state)
	{
	case MENU:
		break;
	case GAMEPLAY:
		if (started_dragging)
		{
			dragOffset(dragged);
		}
		dragSprite(dragged, passport_status);
		break;
	}
	
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
		window.draw(return_rect);
		window.draw(*character);
		window.draw(*passport);

		window.draw(*accept_btn.getSprite());
		window.draw(*reject_btn.getSprite());

		if(passport_rejected)
		{
			window.draw(*reject_stamp.getSprite());
;		}
		else if(passport_accepted)
		{
			window.draw(*accept_stamp.getSprite());
		}

		if(paused)
		{
			window.draw(pause_rect);
			window.draw(p_pause_txt);
		}
		window.draw(g_lives_txt);
		break;
	case END:
		window.draw(e_end_txt);
		window.draw(e_final_score_txt);
		break;
	} 
}

void Game::mouseClicked(sf::Event event)
{
	switch(current_state)
	{
	case MENU:
		break;
	case GAMEPLAY:
		if(!paused)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i click = sf::Mouse::getPosition(window);
				sf::Vector2f clickf = static_cast<sf::Vector2f>(click);

				//If The click's location is within the passport
				if (passport->getGlobalBounds().contains(clickf))
				{
					dragged = passport;
					started_dragging = true;
				}
				else if (accept_btn.getSprite()->getGlobalBounds().contains(clickf))
				{
					passport_accepted = true;
					passport_rejected = false;
					passport_status = accept_stamp.getSprite();
				}
				else if (reject_btn.getSprite()->getGlobalBounds().contains(clickf))
				{
					passport_rejected = true;
					passport_accepted = false;
					passport_status = reject_stamp.getSprite();
				}
			}
		}
		
		break;
	}
  
 
}
void Game::mouseReleased(sf::Event event)
{
	switch(current_state)
	{
	case MENU:
		break;
	case GAMEPLAY:
		if(!paused)
		{
			
			//If the player was dragging a passport and it was accepted/denied
			if(dragged != nullptr && (passport_accepted || passport_rejected))
			{
				sf::Vector2i click = sf::Mouse::getPosition(window);
				sf::Vector2f clickf = static_cast<sf::Vector2f>(click);
				if (collisionReturnCheck(return_rect, clickf)) 
				{
					if(passport_accepted && should_accept || passport_rejected && !should_accept)
					{
						//If it's accpeted when it should be or denied when it should be
						passports_right += 1;
					}
					else
					{
						passports_wrong += 1;
						lives -= 1;
						changeText(g_lives_txt,"Lives: ", lives);
						if(lives <= 0)
						{
							current_state = END;
							changeText(e_final_score_txt, "Correct Stamps: ", passports_right);
						}
					}
					newAnimal();
				}
			}
			dragged = nullptr;
			started_dragging = false;
			updateStampPos();
			break;
		}

	}

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
	case END:
		if (event.key.code == sf::Keyboard::Enter)
		{
			current_state = MENU;
			break;
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
	passport_status = nullptr;

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
	passport->setPosition(window.getSize().x - passport->getGlobalBounds().width*1.5, passport->getGlobalBounds().height/12);
	updateStampPos();
}

void Game::dragSprite(sf::Sprite* sprite, sf::Sprite* stamp_sprite)
{
	if(sprite != nullptr)
	{
		sf::Vector2i mouse_position = sf::Mouse::getPosition(window);
		sf::Vector2f mouse_positionf = static_cast<sf::Vector2f>(mouse_position);
		sf::Vector2f drag_position = mouse_positionf - drag_offset;// (Makes it so it drags from any pos of the mouse on sprite
		sprite->setPosition(drag_position.x, drag_position.y);
		if(stamp_sprite != nullptr)
		{
			stamp_sprite->setPosition(sprite->getPosition().x + sprite->getGlobalBounds().width / 2 - stamp_sprite->getGlobalBounds().width / 2,
				sprite->getPosition().y + sprite->getGlobalBounds().height / 4 - stamp_sprite->getGlobalBounds().height / 2);
		}
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

bool Game::collisionReturnCheck(sf::RectangleShape& rectangle, sf::Vector2f& mouse)
{
	if(rectangle.getPosition().x + rectangle.getGlobalBounds().width >= mouse.x &&
		rectangle.getPosition().x <= mouse.x && rectangle.getPosition().y <= mouse.y &&
		rectangle.getPosition().y + rectangle.getGlobalBounds().height >= mouse.y)
	{
		return true;
	}
	else
	{
		return false;
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

void Game::updateStampPos()
{
	reject_stamp.getSprite()->setPosition(passport->getPosition().x + passport->getGlobalBounds().width / 2 - reject_stamp.getSprite()->getGlobalBounds().width / 2,
		passport->getPosition().y + passport->getGlobalBounds().height / 4 - reject_stamp.getSprite()->getGlobalBounds().height / 2);
	accept_stamp.getSprite()->setPosition(passport->getPosition().x + passport->getGlobalBounds().width / 2 - accept_stamp.getSprite()->getGlobalBounds().width / 2,
		passport->getPosition().y + passport->getGlobalBounds().height / 4 - accept_stamp.getSprite()->getGlobalBounds().height / 2);
}