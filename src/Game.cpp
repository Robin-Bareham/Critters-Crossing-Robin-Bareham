
#include "Game.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}

Game::~Game()
{


}

bool Game::init()
{
	//buttonsNew.assign(7, std::make_shared<GameObject>());
	current_state = MENU;

	// TEXT //
	if(!font.loadFromFile("../Data/Fonts/OpenSans-Bold.ttf"))
	{
		std::cout << "Font didn't load\n";
	}
	// MENU TEXT //
	createText(m_title_txt, "Critter's Crossing", 100, sf::Color::White);
	m_title_txt.setPosition(window.getSize().x/2-m_title_txt.getGlobalBounds().width/2, 20);
	// Instructions TEXT //
	createText(i_mouse_txt, "Click and drag passports with the mouse \n                into the blue box upon \n               accepting or rejecting it.",50,sf::Color::White);
	i_mouse_txt.setPosition(40,0);  
	createText(i_keyboard_txt, "Which key activates which buttons:"
		"\nENTER - Start/Main Menu"
		"\nESC - Return/Quit/Pause/Play Again."
		"\nI - Instructions", 40, sf::Color::White);
	i_keyboard_txt.setPosition(40,i_mouse_txt.getGlobalBounds().height + 40);
	// GAME TEXT //
	//createText(g_lives_txt, "Lives: 3", 30, sf::Color::Red);
	//g_lives_txt.setPosition(610, 5);
	createText(g_timer_txt, "0", 30, sf::Color::Magenta);
	g_timer_txt.setPosition(610, 5);

	// END SCREEN TEXT //
	createText(e_end_txt, "Time's over.", 60, sf::Color::White);
	e_end_txt.setPosition(window.getSize().x/2 - e_end_txt.getGlobalBounds().width/2, 20);
	createText(e_final_correct_score_txt, "Correct Stamps: 0", 100, sf::Color::White);
	e_final_correct_score_txt.setPosition(window.getSize().x / 2 - e_final_correct_score_txt.getGlobalBounds().width / 2, (window.getSize().y / 2 - e_final_correct_score_txt.getGlobalBounds().height) - 100);
	createText(e_final_wrong_score_txt, "Incorrect Stamps: 0", 100, sf::Color::White);
	e_final_wrong_score_txt.setPosition(window.getSize().x / 2 - e_final_wrong_score_txt.getGlobalBounds().width / 2, (window.getSize().y / 2 + 10) - 100);
	
	// SPRITES //
	if(!background_texture.loadFromFile("../Data/Images/CC_BG1.png"))
	{
		std::cout << "Gameplay BG didn't load.\n";
	}
	background.setTexture(background_texture);
	if (!bg_person_texture.loadFromFile("../Data/Images/CC_BG2.png"))
	{
		std::cout << "Gameplay BG2 didn't load.\n";
	}
	bg_person.setTexture(bg_person_texture);
	pause_rect.setSize({ 1080, 720 });
	pause_rect.setFillColor(sf::Color(0, 0, 0, 150));
	info_rect.setSize({1080 - 600,110});
	info_rect.setFillColor(sf::Color(58, 87, 86));
	info_rect.setPosition(600,0);

	//character = new sf::Sprite;
	//passport = new sf::Sprite;
	loadPaths();
	loadButtons();
	newAnimal();
	
	// STAMPS n BUTTONS //
	reject_btn.initaliseSprite("../Data/Images/Critter Crossing Customs/reject button.png");
	accept_btn.initaliseSprite("../Data/Images/Critter Crossing Customs/accept button.png");
	reject_btn.getSprite()->setScale(0.7,0.7);
	accept_btn.getSprite()->setScale(0.7,0.7);
	accept_btn.getSprite()->setPosition(
		bg_person.getGlobalBounds().width + 35,
		info_rect.getPosition().y + info_rect.getGlobalBounds().height + 20);
	reject_btn.getSprite()->setPosition(
		accept_btn.getSprite()->getPosition().x + accept_btn.getSprite()->getGlobalBounds().width + 20,
		info_rect.getPosition().y + info_rect.getGlobalBounds().height + 20);

	reject_stamp.initaliseSprite("../Data/Images/Critter Crossing Customs/reject.png");
	accept_stamp.initaliseSprite("../Data/Images/Critter Crossing Customs/accept.png");
	updateStampPos();
	//std::cout << "Buttons Count 3: " << buttonsNew.size() << std::endl;

  return true;
}

void Game::update(float dt)
{
	switch (current_state)
	{
	case MENU:
		break;
	case GAMEPLAY:
		std::string temp_time = "";
		if(game_time.getElapsedTime().asSeconds() < 1) {temp_time = "0";}
		updateCycle();
		changeText(g_timer_txt,temp_time, game_time.getElapsedTime().asSeconds());
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
		//std::cout << "Buttons Count 4: " << buttonsNew.size() << std::endl;
		//std::cout << "MENU STATE\n";
		window.draw(m_title_txt);
		window.draw(*buttonsNew[0].get()->getSprite());
		window.draw(*buttonsNew[1].get()->getSprite());
		window.draw(*buttonsNew[2].get()->getSprite());
		break;
	case GAMEPLAY:
		//std::cout << "GAMEPLAY STATE\n";
		window.draw(background);
		window.draw(bg_person);
		window.draw(info_rect);
		window.draw(*character.getSprite());
		window.draw(*passport.getSprite());

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
		window.draw(*buttonsNew[6].get()->getSprite());
		//window.draw(g_lives_txt);
		window.draw(g_timer_txt);

		if(paused)
		{
			window.draw(pause_rect);
			//window.draw(p_pause_txt);
			window.draw(*buttonsNew[2].get()->getSprite());
			window.draw(*buttonsNew[3].get()->getSprite());
			window.draw(*buttonsNew[4].get()->getSprite());
		}
		
		break;
	case END:
		window.draw(e_end_txt);
		window.draw(e_final_correct_score_txt);
		window.draw(e_final_wrong_score_txt);
		window.draw(*buttonsNew[4].get()->getSprite());
		window.draw(*buttonsNew[5].get()->getSprite());
		break;
	case INSTRUCTIONS:
		window.draw(i_mouse_txt);
		window.draw(i_keyboard_txt);
		window.draw(*buttonsNew[3].get()->getSprite());
		break;
	} 
}

void Game::mouseClicked(sf::Event event)
{
	switch(current_state)
	{
	case MENU:
		if(event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2i click = sf::Mouse::getPosition(window);
			sf::Vector2f clickf = static_cast<sf::Vector2f>(click);
			
			//If start button is pressed
			if(buttonsNew[0].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				current_state = GAMEPLAY;
				updateBtns();
				resetGame();
			}
			//If quit button is pressed
			else if(buttonsNew[1].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				window.close();
			}
			//If instructions button is pressed
			else if(buttonsNew[2].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				previous_state = current_state;
				current_state = INSTRUCTIONS;
				updateBtns();
			}
		}
		break;
	case GAMEPLAY:
		if(!paused)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i click = sf::Mouse::getPosition(window);
				sf::Vector2f clickf = static_cast<sf::Vector2f>(click);
				//std::cout << click.x << " " << click.y << std::endl;
				//If The click's location is within the passport
				if (passport.getSprite()->getGlobalBounds().contains(clickf))
				{
					dragged = passport.getSprite();
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

				if (buttonsNew[6].get()->getSprite()->getGlobalBounds().contains(clickf))
				{
					paused = true;
				}
			}
		}
		else
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i click = sf::Mouse::getPosition(window);
				sf::Vector2f clickf = static_cast<sf::Vector2f>(click);

				//If start button is pressed
				if (buttonsNew[3].get()->getSprite()->getGlobalBounds().contains(clickf))
				{
					paused = false;
				}
				//If quit button is pressed
				else if (buttonsNew[4].get()->getSprite()->getGlobalBounds().contains(clickf))
				{
					paused = false;
					current_state = MENU;
					updateBtns();
				}
				//If instructions button is pressed
				else if (buttonsNew[2].get()->getSprite()->getGlobalBounds().contains(clickf))
				{
					previous_state = current_state;
					current_state = INSTRUCTIONS;
					updateBtns();
				}
			}
		}
		break;
	case END:
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2i click = sf::Mouse::getPosition(window);
			sf::Vector2f clickf = static_cast<sf::Vector2f>(click);

			//If quit button is pressed
			if (buttonsNew[4].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				current_state = MENU;
				updateBtns();
			}
			//If instructions button is pressed
			else if (buttonsNew[5].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				current_state = GAMEPLAY;
				updateBtns();
				resetGame();
			}
		}
		break;
	case INSTRUCTIONS:
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2i click = sf::Mouse::getPosition(window);
			sf::Vector2f clickf = static_cast<sf::Vector2f>(click);

			//If quit button is pressed
			if (buttonsNew[3].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				if(previous_state == GAMEPLAY)
				{
					current_state = GAMEPLAY;
					updateBtns();
				}
				else if(previous_state == MENU)
				{
					current_state = MENU;
					updateBtns();
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
				if (bg_person.getGlobalBounds().contains(clickf)) 
				{
					if(passport_accepted && should_accept || passport_rejected && !should_accept)
					{
						//If it's accpeted when it should be or denied when it should be
						passports_right += 1;
					}
					else
					{
						passports_wrong += 1;
						/*lives -= 1;
						changeText(g_lives_txt,"Lives: ", lives);
						if(lives <= 0)
						{
							current_state = END;
							updateBtns();
							changeText(e_final_score_txt, "Correct Stamps: ", passports_right);
						}*/
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
			updateBtns();
			resetGame();
		}
		if(event.key.code == sf::Keyboard::Escape)
		{
			window.close();
		}
		if (event.key.code == sf::Keyboard::I)
		{
			current_state = INSTRUCTIONS;
			updateBtns();
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
				updateBtns();
				break;
			}
			if (event.key.code == sf::Keyboard::I)
			{
				current_state = INSTRUCTIONS;
				updateBtns();
				break;
			}
		}
		if (event.key.code == sf::Keyboard::Escape)
		{
			paused = true;
		}
		if(event.key.code == sf::Keyboard::Enter)
		{
			current_state = MENU;
			updateBtns();
		}
		if (event.key.code == sf::Keyboard::R)
		{
			newAnimal();
		}
		break;
	case END:
		if (event.key.code == sf::Keyboard::Escape)
		{
			current_state = GAMEPLAY;
			updateBtns();
			resetGame();
			break;
		}
		if (event.key.code == sf::Keyboard::Enter)
		{
			current_state = MENU;
			updateBtns();
			break;
		}
		break;
	case INSTRUCTIONS:

		if(event.key.code == sf::Keyboard::Escape)
		{
			if (previous_state == GAMEPLAY)
			{
				current_state = GAMEPLAY;
				updateBtns();
			}
			else if (previous_state == MENU)
			{
				current_state = MENU;
				updateBtns();
			}
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
	character.setNewTexture(animal_index);
	character.getSprite()->setPosition(bg_person.getGlobalBounds().width / 2 - (character.getSprite()->getGlobalBounds().width / 2),
		bg_person.getGlobalBounds().height / 2 - (character.getSprite()->getGlobalBounds().height / 2));

	passport.setNewTexture(passport_index);
	passport.getSprite()->setPosition(bg_person.getGlobalBounds().width + passport.getSprite()->getGlobalBounds().width/2 - 50,
		window.getSize().y - passport.getSprite()->getGlobalBounds().height - 20);
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

void Game::resetGame()
{
	paused = false;
	passport_accepted = false;
	passport_rejected = false;
	should_accept = false;
	started_dragging = false;
	passports_right = 0;
	passports_wrong = 0;
	//lives = 3;
	//changeText(g_lives_txt, "Lives: ", lives);
	game_time.restart();
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

void Game::updateStampPos()
{
	reject_stamp.getSprite()->setPosition(passport.getSprite()->getPosition().x + passport.getSprite()->getGlobalBounds().width / 2 - reject_stamp.getSprite()->getGlobalBounds().width / 2,
		passport.getSprite()->getPosition().y + passport.getSprite()->getGlobalBounds().height / 4 - reject_stamp.getSprite()->getGlobalBounds().height / 2);
	accept_stamp.getSprite()->setPosition(passport.getSprite()->getPosition().x + passport.getSprite()->getGlobalBounds().width / 2 - accept_stamp.getSprite()->getGlobalBounds().width / 2,
		passport.getSprite()->getPosition().y + passport.getSprite()->getGlobalBounds().height / 4 - accept_stamp.getSprite()->getGlobalBounds().height / 2);
}

void Game::loadPaths()
{
	btn_files = { "../Data/Images/Buttons/btn_start.png","../Data/Images/Buttons/btn_quit.png",
	"../Data/Images/Buttons/btn_instructions.png","../Data/Images/Buttons/btn_return.png",
	"../Data/Images/Buttons/btn_mainmenu.png","../Data/Images/Buttons/btn_playagain.png",
	"../Data/Images/Buttons/btn_pause.png" };
	passport_files = { "../Data/Images/Critter Crossing Customs/elephant passport.png",
	"../Data/Images/Critter Crossing Customs/moose passport.png","../Data/Images/Critter Crossing Customs/penguin passport.png" };
	animal_files = { "../Data/Images/Critter Crossing Customs/elephant.png",
	"../Data/Images/Critter Crossing Customs/moose.png","../Data/Images/Critter Crossing Customs/penguin.png" };
	
	//Reserving memory space for the vector's size
	passports.reserve(3);
	animals.reserve(3);

	for(int i = 0; i < 3; i++)
	{
		//Push back makes sure there's something in that space.
		passports.push_back(sf::Texture());
		animals.push_back(sf::Texture());
		if(!passports[i].loadFromFile(passport_files[i]))
		{
			std::cout << passport_files[i] << " Didn't load\n";
		}
		if(!animals[i].loadFromFile(animal_files[i]))
		{
			std::cout << animal_files[i] << " Didn't load\n";
		}
	}
	character.setTextureList(animals);
	passport.setTextureList(passports);
}

void Game::loadButtons()
{


	// Creates the spaces for the pointers in the vector
	//buttonsNew.assign(7, std::make_shared<GameObject>());

	//Assigns space in pointers to the amount of buttons there are
	buttonsNew.reserve(btn_files.size());

	//Loops through each path in the buttons and assigns it to button list.
	for (const auto& path: btn_files)
	{
		auto btn_holder = std::make_unique<GameObject>();
		if(!btn_holder->initaliseSprite(path))
		{
			std::cout << "Initalisation failed for path: " << path << std::endl;
		}
		buttonsNew.push_back(std::move(btn_holder));
	}

	buttonsNew[0].get()->getSprite()->setPosition(40,
		window.getSize().y - buttonsNew[0].get()->getSprite()->getGlobalBounds().height - 40);
	buttonsNew[1].get()->getSprite()->setPosition(window.getSize().x - buttonsNew[1].get()->getSprite()->getGlobalBounds().width - 40,
		window.getSize().y - buttonsNew[1].get()->getSprite()->getGlobalBounds().height - 40);
	buttonsNew[2].get()->getSprite()->setPosition(window.getSize().x / 2 - buttonsNew[2].get()->getSprite()->getGlobalBounds().width / 2,
		window.getSize().y - buttonsNew[2].get()->getSprite()->getGlobalBounds().height - 40);
	buttonsNew[3].get()->getSprite()->setPosition(window.getSize().x / 2 - buttonsNew[3].get()->getSprite()->getGlobalBounds().width / 2, 40);
	buttonsNew[4].get()->getSprite()->setPosition(window.getSize().x / 2 - buttonsNew[4].get()->getSprite()->getGlobalBounds().width / 2,
		window.getSize().y - buttonsNew[4].get()->getSprite()->getGlobalBounds().height - 40);
	buttonsNew[5].get()->getSprite()->setPosition(window.getSize().x / 2 - buttonsNew[5].get()->getSprite()->getGlobalBounds().width - 5,
		window.getSize().y - buttonsNew[5].get()->getSprite()->getGlobalBounds().height - 40);
	buttonsNew[6].get()->getSprite()->setScale(0.8,0.8);
	buttonsNew[6].get()->getSprite()->setPosition(window.getSize().x - buttonsNew[6].get()->getSprite()->getGlobalBounds().width - 5, 5);
	updateBtns();
	//std::cout << "Buttons Count 2: " << buttonsNew.size() << std::endl;
}

void Game::updateBtns()
{
	switch(current_state)
	{
		case MENU:
			buttonsNew[2].get()->getSprite()->setPosition(window.getSize().x / 2 - buttonsNew[2].get()->getSprite()->getGlobalBounds().width / 2,
			window.getSize().y - buttonsNew[2].get()->getSprite()->getGlobalBounds().height - 40);
		break;
		case GAMEPLAY:
			buttonsNew[2].get()->getSprite()->setPosition(window.getSize().x / 2 - buttonsNew[2].get()->getSprite()->getGlobalBounds().width / 2,
				window.getSize().y / 2 - buttonsNew[2].get()->getSprite()->getGlobalBounds().height / 2);
			buttonsNew[4].get()->getSprite()->setPosition(window.getSize().x / 2 - buttonsNew[4].get()->getSprite()->getGlobalBounds().width / 2,
				window.getSize().y - buttonsNew[4].get()->getSprite()->getGlobalBounds().height - 40);
			buttonsNew[3].get()->getSprite()->setPosition(window.getSize().x / 2 - buttonsNew[3].get()->getSprite()->getGlobalBounds().width / 2, 40);
			break;
		case END:
			buttonsNew[4].get()->getSprite()->setPosition(window.getSize().x / 2 + 5,
				window.getSize().y - buttonsNew[4].get()->getSprite()->getGlobalBounds().height - 40);
			break;
		case INSTRUCTIONS:
			buttonsNew[3].get()->getSprite()->setPosition(10, window.getSize().y - buttonsNew[3].get()->getSprite()->getGlobalBounds().height - 10);
			break;
	}
}

void Game::updateCycle()
{
	//Check the timer
	//10 seconds for a section - Morning, Miday, Evening end.
	if (game_time.getElapsedTime().asSeconds() < 5)
	{
		//std::cout << "MORNING\n";
	}
	else if (game_time.getElapsedTime().asSeconds() < 10)
	{
		//std::cout << "MIDAY\n";
	}
	else if (game_time.getElapsedTime().asSeconds() < 15)
	{
		//std::cout << "EVENING\n";
	}
	else
	{
		//std::cout << "END OF DAY.\n";
		
		current_state = END;
		updateBtns();
		changeText(e_final_correct_score_txt, "Correct Stamps: ", passports_right);
		changeText(e_final_wrong_score_txt, "Incorrect Stamps: ", passports_wrong);
	}
}