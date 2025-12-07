
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

	// AUDIO //
	if (!incorrect_buffer.loadFromFile("../Data/Audio/incorrect.wav")) { std::cout << "Incorrect Audio failed to load\n"; }
	incorrect_sound.setBuffer(incorrect_buffer);
	if (!correct_buffer.loadFromFile("../Data/Audio/correct.wav")) { std::cout << "Correct Audio failed to load\n"; }
	correct_sound.setBuffer(correct_buffer);
	if (!click_buffer.loadFromFile("../Data/Audio/click.wav")) { std::cout << "Click Audio failed to load\n"; }
	click_sound.setBuffer(click_buffer);

	// TEXT //
	if(!font.loadFromFile("../Data/Fonts/OpenSans-Bold.ttf"))
	{
		std::cout << "Font didn't load\n";
	}
	// MENU TEXT //
	createText(m_title_txt, "Critter's Crossing", 100, sf::Color::White);
	m_title_txt.setPosition(window.getSize().x/2-m_title_txt.getGlobalBounds().width/2, 20);
	createText(m_highscore_txt, "Current Highscore: 0", 60, sf::Color::White);
	m_highscore_txt.setPosition(window.getSize().x/2-m_highscore_txt.getGlobalBounds().width/2,m_title_txt.getPosition().y + m_title_txt.getGlobalBounds().height + 20);
	createText(m_instructions_txt, "   Get as much money as possible within 10 days.\nUpgrade to increase day cycles and money gained.\n         You lose money for incorrect stamps.", 40, sf::Color::White);
	m_instructions_txt.setPosition(window.getSize().x/2-m_instructions_txt.getGlobalBounds().width/2, 
		m_highscore_txt.getPosition().y + m_highscore_txt.getGlobalBounds().height + 20);
	
	// Instructions TEXT //
	createText(i_mouse_txt, "Click and drag passports with the mouse \n                into the blue box upon \n               accepting or rejecting it.",50,sf::Color::White);
	i_mouse_txt.setPosition(40,0);  
	createText(i_keyboard_txt, "Which key activates which buttons:"
		"\nENTER - Start/Main Menu"
		"\nESC - Return/Quit/Pause/Play Again."
		"\nI - Instructions", 40, sf::Color::White);
	i_keyboard_txt.setPosition(40,i_mouse_txt.getGlobalBounds().height + 40);
	// GAME TEXT //
	createText(g_timer_txt, "", 30, sf::Color::Magenta);
	g_timer_txt.setPosition(610, 5);

	// END SCREEN TEXT //
	createText(e_end_txt, "End of Day 1", 60, sf::Color::White);
	e_end_txt.setPosition((window.getSize().x + 370) / 2 - e_end_txt.getGlobalBounds().width / 2, 20);
	createText(e_money_earned_txt, "Money: £0", 60, sf::Color::White);
	e_money_earned_txt.setPosition(10, 10);
	createText(e_final_correct_score_txt, "Correct: 0", 50, sf::Color::White);
	e_final_correct_score_txt.setPosition(10, e_money_earned_txt.getPosition().y + e_money_earned_txt.getGlobalBounds().height + 10);
	createText(e_final_wrong_score_txt, "Incorrect: 0", 50, sf::Color::White);
	e_final_wrong_score_txt.setPosition(10, e_final_correct_score_txt.getPosition().y + e_final_correct_score_txt.getGlobalBounds().height + 20);
	createText(e_buy_txt, "Click on an object bellow to buy it.", 30, sf::Color::White);
	e_buy_txt.setPosition((window.getSize().x + 370) / 2 - e_buy_txt.getGlobalBounds().width/2, 30 + e_end_txt.getGlobalBounds().height);

	// SPRITES //
	if(!background_texture.loadFromFile("../Data/Images/CC_BG1.png")) { std::cout << "Gameplay BG didn't load.\n";	}
	background.setTexture(background_texture);
	if (!bg_person_texture.loadFromFile("../Data/Images/CC_BG2.png")) { std::cout << "Gameplay BG2 didn't load.\n"; }
	bg_person.setTexture(bg_person_texture);
	if(!bg_end_texture.loadFromFile("../Data/Images/CC_BG_END.png")) { std::cout << "End BG didn't load\n"; }
	bg_end.setTexture(bg_end_texture);

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

	//End text underneath Buttons
	
	float time_middle_pos = item_buttons[0].get()->getSprite()->getPosition().x + item_buttons[0].get()->getSprite()->getGlobalBounds().width / 2;
	float money_middle_pos = item_buttons[1].get()->getSprite()->getPosition().x + item_buttons[1].get()->getSprite()->getGlobalBounds().width / 2;

	//Time Increase Values
	createText(e_time_txt, "^ Day Time", 30, sf::Color::White);
	e_time_txt.setPosition(
		time_middle_pos - e_time_txt.getGlobalBounds().width/2,
		item_buttons[0].get()->getSprite()->getPosition().y + item_buttons[0].get()->getSprite()->getGlobalBounds().height  + 10);
	createText(e_time_amount_txt, "Costs: £5", 30, sf::Color::White);
	e_time_amount_txt.setPosition(
		time_middle_pos - e_time_amount_txt.getGlobalBounds().width/2,
		e_time_txt.getPosition().y + e_time_txt.getGlobalBounds().height + 10);
	//Money Increase Values
	createText(e_money_txt, "^ Money Gained", 30, sf::Color::White);
	e_money_txt.setPosition(
		money_middle_pos - e_money_txt.getGlobalBounds().width/2,
		e_time_txt.getPosition().y);
	createText(e_money_amount_txt, "Costs: £5", 30, sf::Color::White);
	e_money_amount_txt.setPosition(
		money_middle_pos - e_money_amount_txt.getGlobalBounds().width / 2,
		e_time_amount_txt.getPosition().y);

	
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
		//Updates Timer
		if(!paused)
		{
			std::string temp_time = "";
			if (game_time.getElapsedTime().asSeconds() < 1) { temp_time = ""; }
			updateCycle();
			changeText(g_timer_txt, temp_time, (game_time.getElapsedTime().asSeconds()-pause_delay));
			//Checks if it needs to drag the passport.
			if (started_dragging)
			{
				dragOffset(dragged);
			}
			dragSprite(dragged, passport_status);
		}
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
		window.draw(m_highscore_txt);
		window.draw(m_instructions_txt);
		window.draw(*buttons[0].get()->getSprite());
		window.draw(*buttons[1].get()->getSprite());
		window.draw(*buttons[2].get()->getSprite());
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
		window.draw(*buttons[6].get()->getSprite());
		//window.draw(g_lives_txt);
		window.draw(g_timer_txt);

		if(paused)
		{
			window.draw(pause_rect);
			//window.draw(p_pause_txt);
			window.draw(*buttons[2].get()->getSprite());
			window.draw(*buttons[3].get()->getSprite());
			window.draw(*buttons[4].get()->getSprite());
		}
		
		break;
	case END:
		window.draw(bg_end);
		window.draw(e_end_txt);
		window.draw(e_buy_txt);
		window.draw(e_final_correct_score_txt);
		window.draw(e_final_wrong_score_txt);
		window.draw(e_money_earned_txt);
		window.draw(e_money_txt);
		window.draw(e_money_amount_txt);
		window.draw(e_time_txt);
		window.draw(e_time_amount_txt);
		window.draw(*buttons[4].get()->getSprite());
		window.draw(*buttons[5].get()->getSprite());
		window.draw(*item_buttons[0].get()->getSprite());
		window.draw(*item_buttons[1].get()->getSprite());
		break;
	case INSTRUCTIONS:
		window.draw(i_mouse_txt);
		window.draw(i_keyboard_txt);
		window.draw(*buttons[3].get()->getSprite());
		break;
	case GAMEOVER:
		window.draw(e_money_earned_txt);
		window.draw(m_highscore_txt);
		window.draw(*buttons[4].get()->getSprite());
			break;
	} 
}

void Game::mouseClicked(sf::Event event)
{
	bool button_clicked = false;
	
	switch(current_state)
	{
	case MENU:
		if(event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2i click = sf::Mouse::getPosition(window);
			sf::Vector2f clickf = static_cast<sf::Vector2f>(click);
			
			//If start button is pressed
			if(buttons[0].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				button_clicked = true;
				current_state = GAMEPLAY;
				days = 1;
				updateBtns();
				resetGame();
				money_earned = 0;
			}
			//If quit button is pressed
			else if(buttons[1].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				button_clicked = true;
				window.close();
			}
			//If instructions button is pressed
			else if(buttons[2].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				button_clicked = true;
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
					button_clicked = true;
					dragged = passport.getSprite();
					started_dragging = true;
				}
				else if (accept_btn.getSprite()->getGlobalBounds().contains(clickf))
				{
					if (!option_chosen) 
					{
						button_clicked = true;
						passport_accepted = true;
						passport_rejected = false;
						passport_status = accept_stamp.getSprite();
						option_chosen = true;
					}

				}
				else if (reject_btn.getSprite()->getGlobalBounds().contains(clickf))
				{
					if(!option_chosen)
					{
						button_clicked = true;
						passport_rejected = true;
						passport_accepted = false;
						passport_status = reject_stamp.getSprite();
						option_chosen = true;
					}

				}

				if (buttons[6].get()->getSprite()->getGlobalBounds().contains(clickf))
				{
					button_clicked = true;
					paused = true;
					//Gets the time it left off on.
					pause_delay = game_time.getElapsedTime().asSeconds() - pause_delay;
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
				if (buttons[3].get()->getSprite()->getGlobalBounds().contains(clickf))
				{
					button_clicked = true;
					paused = false;
					pause_delay = game_time.getElapsedTime().asSeconds() - pause_delay;
				}
				//If quit button is pressed
				else if (buttons[4].get()->getSprite()->getGlobalBounds().contains(clickf))
				{
					button_clicked = true;
					paused = false;
					current_state = MENU;
					updateBtns();
				}
				//If instructions button is pressed
				else if (buttons[2].get()->getSprite()->getGlobalBounds().contains(clickf))
				{
					button_clicked = true;
					previous_state = current_state;
					current_state = INSTRUCTIONS;
					updateBtns();
				}
			}
		}
		break;
	case END:
		if (event.mouseButton.button == sf::Mouse::Left && activate_buying)
		{
			sf::Vector2i click = sf::Mouse::getPosition(window);
			sf::Vector2f clickf = static_cast<sf::Vector2f>(click);

			//If main menu button is pressed
			if (buttons[4].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				button_clicked = true;
				current_state = MENU;
				updateBtns();
			}
			//If play again button is pressed
			else if (buttons[5].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				button_clicked = true;
				current_state = GAMEPLAY;
				days += 1;
				updateBtns();
				resetLoop();
			}
			//If Timer Increase is pressed
			else if(item_buttons[0].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				button_clicked = true;
				//If the player has enough money
				if(money_earned >= time_cost && can_upgrade_time)
				{
					float time_middle_pos = item_buttons[0].get()->getSprite()->getPosition().x + item_buttons[0].get()->getSprite()->getGlobalBounds().width / 2;
					money_earned -= time_cost;
					time_cost += 5;
					day_timer += (1 * time_inflation);
					time_inflation += 1;
					changeText(e_money_earned_txt, "Money: £", money_earned);
					if(time_inflation <5)
					{
						changeText(e_time_amount_txt, "Costs: £", time_cost);
					}
					else
					{
						changeText(e_time_amount_txt, "MAX UPGRADE", 9000);
						can_upgrade_time = false;
					}
					
					e_time_amount_txt.setPosition(
						time_middle_pos - e_time_amount_txt.getGlobalBounds().width / 2,
						e_time_txt.getPosition().y + e_time_txt.getGlobalBounds().height + 10);
					

				}
				

			}
			//If Money Increased is pressed
			else if(item_buttons[1].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				button_clicked = true;
				//If the player has enough money
				if (money_earned >= money_cost && can_upgrade_money)
				{
					float money_middle_pos = item_buttons[1].get()->getSprite()->getPosition().x + item_buttons[1].get()->getSprite()->getGlobalBounds().width / 2;
					money_earned -= money_cost;
					money_cost += 5;
					money_increase += (1 * money_inflation);
					money_inflation += 0.7;
					changeText(e_money_earned_txt, "Money: £", money_earned);
					if(money_inflation < 5)
					{
						changeText(e_money_amount_txt, "Costs: £", money_cost);
					}
					else
					{
						changeText(e_money_amount_txt, "MAX UPGRADE", 9000);
						can_upgrade_money = false;
					}
					e_money_amount_txt.setPosition(
						money_middle_pos - e_money_amount_txt.getGlobalBounds().width / 2,
						e_time_amount_txt.getPosition().y);
					
				}
			}
		}
		activate_buying = true;
		break;
	case INSTRUCTIONS:
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			sf::Vector2i click = sf::Mouse::getPosition(window);
			sf::Vector2f clickf = static_cast<sf::Vector2f>(click);

			//If quit button is pressed
			if (buttons[3].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				button_clicked = true;
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
	case GAMEOVER:
		if (event.mouseButton.button == sf::Mouse::Left) 
		{
			sf::Vector2i click = sf::Mouse::getPosition(window);
			sf::Vector2f clickf = static_cast<sf::Vector2f>(click);

			//If quit button is pressed
			if (buttons[4].get()->getSprite()->getGlobalBounds().contains(clickf))
			{
				button_clicked = true;
				current_state = MENU;
				updateBtns();
			}
		}
		break;
	}
  if(button_clicked)
  {
	  click_sound.play();
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
						correct_sound.play();
					}
					else
					{
						passports_wrong += 1;
						incorrect_sound.play();
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
			days = 1;
			updateBtns();
			resetGame();
			money_earned = 0;
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
				pause_delay = game_time.getElapsedTime().asSeconds() - pause_delay;
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
			pause_delay = game_time.getElapsedTime().asSeconds() - pause_delay;
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
			days += 1;
			updateBtns();
			resetLoop();
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
	option_chosen = false;
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
	pause_delay = 0;
	money_earned = 0;
	started_dragging = false;
	dragged = nullptr;
	passports_right = 0;
	passports_wrong = 0;
	newAnimal();
	game_time.restart();
	days = 1;
	day_timer = 5;
	money_increase = 1;
	money_cost = 5;
	time_cost = 5;
	time_inflation = 1;
	money_inflation = 1;
	can_upgrade_time = true;
	can_upgrade_money = true;
	changeText(e_time_amount_txt, "Costs: £5", 9000);
	changeText(e_money_amount_txt, "Costs: £5", 9000);
	e_money_earned_txt.setPosition(10, 10);
}

void Game::resetLoop()
{
	paused = false;
	pause_delay = 0;
	started_dragging = false;
	dragged = nullptr;
	passports_right = 0;
	passports_wrong = 0;
	newAnimal();
	game_time.restart();
}

void Game::updateCycle()
{
	//Check the timer
	//10 seconds for a section - Morning, Miday, Evening end.
	if (game_time.getElapsedTime().asSeconds() - pause_delay < day_timer)
	{
		//std::cout << "MORNING\n";
	}
	else
	{
		//std::cout << "END OF DAY.\n";

		current_state = END;
		activate_buying = false;
		updateBtns();
		calculateMoney();
		changeText(e_final_correct_score_txt, "Correct: ", passports_right);
		changeText(e_final_wrong_score_txt, "Incorrect: ", passports_wrong);
		changeText(e_end_txt, "End of Day ", days);
		if (days >= 10)
		{
			if(money_earned > money_highscore)
			{
				money_highscore = money_earned;
				changeText(m_highscore_txt, "Current Highscore: ", money_highscore);
				m_highscore_txt.setPosition(window.getSize().x / 2 - m_highscore_txt.getGlobalBounds().width / 2, m_highscore_txt.getPosition().y);
			}

			current_state = GAMEOVER;
			e_money_earned_txt.setPosition(window.getSize().x/2 - e_money_amount_txt.getGlobalBounds().width/2, m_highscore_txt.getPosition().y + m_highscore_txt.getGlobalBounds().height + 20);
		}
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
	if(temp_number != 9000)
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
	btn_item_files = { "../Data/Images/CC_clock.png","../Data/Images/CC_Money.png" };
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
	buttons.reserve(btn_files.size());

	//Loops through each path in the buttons and assigns it to button list.
	for (const auto& path: btn_files)
	{
		auto btn_holder = std::make_unique<GameObject>();
		if(!btn_holder->initaliseSprite(path))
		{
			std::cout << "Initalisation failed for path: " << path << std::endl;
		}
		buttons.push_back(std::move(btn_holder));
	}

	buttons[0].get()->getSprite()->setPosition(40,
		window.getSize().y - buttons[0].get()->getSprite()->getGlobalBounds().height - 40);
	buttons[1].get()->getSprite()->setPosition(window.getSize().x - buttons[1].get()->getSprite()->getGlobalBounds().width - 40,
		window.getSize().y - buttons[1].get()->getSprite()->getGlobalBounds().height - 40);
	buttons[2].get()->getSprite()->setPosition(window.getSize().x / 2 - buttons[2].get()->getSprite()->getGlobalBounds().width / 2,
		window.getSize().y - buttons[2].get()->getSprite()->getGlobalBounds().height - 40);
	buttons[3].get()->getSprite()->setPosition(window.getSize().x / 2 - buttons[3].get()->getSprite()->getGlobalBounds().width / 2, 40);
	buttons[4].get()->getSprite()->setPosition(window.getSize().x / 2 - buttons[4].get()->getSprite()->getGlobalBounds().width / 2,
		window.getSize().y - buttons[4].get()->getSprite()->getGlobalBounds().height - 40);
	buttons[5].get()->getSprite()->setPosition(30, window.getSize().y - (buttons[5].get()->getSprite()->getGlobalBounds().height * 2)-40);
	buttons[6].get()->getSprite()->setScale(0.8,0.8);
	buttons[6].get()->getSprite()->setPosition(window.getSize().x - buttons[6].get()->getSprite()->getGlobalBounds().width - 5, 5);
	updateBtns();
	//std::cout << "Buttons Count 2: " << buttonsNew.size() << std::endl;

	item_buttons.reserve(btn_item_files.size());

	for (const auto& path : btn_item_files) 
	{
		auto btn_holder = std::make_unique<GameObject>(); 
		if (!btn_holder->initaliseSprite(path)) 
		{
			std::cout << "Initalisation failed for path: " << path << std::endl; 
		}
		item_buttons.push_back(std::move(btn_holder));
	}

	float es_middle = (window.getSize().x + 370) / 2;

	item_buttons[0].get()->getSprite()->setPosition(es_middle - item_buttons[0].get()->getSprite()->getGlobalBounds().width - 50, 
		e_buy_txt.getPosition().y + e_buy_txt.getGlobalBounds().height + 40);
	item_buttons[1].get()->getSprite()->setPosition(es_middle + 50, 
		e_buy_txt.getPosition().y + e_buy_txt.getGlobalBounds().height * 2);
}

void Game::updateBtns()
{
	switch(current_state)
	{
		case MENU:
			buttons[2].get()->getSprite()->setPosition(window.getSize().x / 2 - buttons[2].get()->getSprite()->getGlobalBounds().width / 2,
			window.getSize().y - buttons[2].get()->getSprite()->getGlobalBounds().height - 40);
		break;
		case GAMEPLAY:
			buttons[2].get()->getSprite()->setPosition(window.getSize().x / 2 - buttons[2].get()->getSprite()->getGlobalBounds().width / 2,
				window.getSize().y / 2 - buttons[2].get()->getSprite()->getGlobalBounds().height / 2);
			buttons[4].get()->getSprite()->setPosition(window.getSize().x / 2 - buttons[4].get()->getSprite()->getGlobalBounds().width / 2,
				window.getSize().y - buttons[4].get()->getSprite()->getGlobalBounds().height - 40);
			buttons[3].get()->getSprite()->setPosition(window.getSize().x / 2 - buttons[3].get()->getSprite()->getGlobalBounds().width / 2, 40);
			break;
		case END:
			buttons[4].get()->getSprite()->setPosition(30, window.getSize().y - buttons[4].get()->getSprite()->getGlobalBounds().height - 20);
			break;
		case INSTRUCTIONS:
			buttons[3].get()->getSprite()->setPosition(10, window.getSize().y - buttons[3].get()->getSprite()->getGlobalBounds().height - 10);
			break;
	}
}

void Game::calculateMoney()
{
	money_earned += (passports_right * money_increase);
	money_earned -= (passports_wrong * money_increase);
	changeText(e_money_earned_txt, "Money: £", money_earned);
	e_money_earned_txt.setPosition(10, 10);
}