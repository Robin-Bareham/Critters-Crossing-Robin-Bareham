
#ifndef CRITTERSCROSSING_GAME_H
#define CRITTERSCROSSING_GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameObject.h"
#include "Animal.h"
#include "Passport.h"

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool init();
  void update(float dt);
  void render();
  void mouseClicked(sf::Event event);
  void mouseReleased(sf::Event event);
  void keyPressed(sf::Event event);
  void keyReleased(sf::Event event);

 private:
	 void createText(sf::Text& text_name, std::string text, int size, sf::Color colour);
	 void changeText(sf::Text& text_name, std::string new_text, int temp_number);
	 void loadButtons();
	 void loadPaths();
	 void newAnimal();
	 void dragSprite(sf::Sprite* sprite, sf::Sprite* stamp_sprite);
	 void dragOffset(sf::Sprite* sprite);
	 void updateBtns();
	 void updateStampPos();
	 bool collisionReturnCheck(sf::RectangleShape& rectangle, sf::Vector2f& mouse);
	 void resetGame();
	 void updateCycle();
	 void calculateMoney();

  sf::RenderWindow& window;
  // Game States
  enum game_state {MENU,GAMEPLAY,END,INSTRUCTIONS};
  game_state current_state;
  game_state previous_state;
  bool paused = false;
  // Passport States
  bool passport_accepted = false;
  bool passport_rejected = false;
  bool should_accept = false;
  bool started_dragging = false;
  bool option_chosen = false;
  int passports_right = 0;
  int passports_wrong = 0;
  float money_earned = 0;
  //int lives = 3;
  
  // Audio
  sf::SoundBuffer incorrect_buffer;
  sf::Sound incorrect_sound;
  sf::SoundBuffer correct_buffer;
  sf::Sound correct_sound;
  sf::SoundBuffer click_buffer;
  sf::Sound click_sound;
  // Music
  //sf::Music name;


  // Text
  sf::Font font;
  sf::Text m_title_txt;
  sf::Text m_instructions_txt;
  //sf::Text g_lives_txt;
  sf::Text g_timer_txt;
  sf::Text e_end_txt;
  sf::Text e_final_correct_score_txt;
  sf::Text e_final_wrong_score_txt;
  sf::Text e_money_earned_txt;
  sf::Text i_mouse_txt;
  sf::Text i_keyboard_txt;

  // Sprites
  sf::Sprite background;
  sf::Texture background_texture;
  sf::Sprite bg_person;
  sf::Texture bg_person_texture;
  std::vector<std::string> btn_files;
  std::vector<std::string> passport_files;
  std::vector<std::string> animal_files;
  sf::RectangleShape pause_rect;
  sf::RectangleShape info_rect;
  GameObject accept_btn;
  GameObject reject_btn;
  GameObject reject_stamp;
  GameObject accept_stamp;
  GameObject temp_btn;
  //GameObject* buttons = new GameObject[7]; 
  std::vector<std::unique_ptr<GameObject>> buttonsNew;
  Animal character;
  std::vector<sf::Texture> animals;
  Passport passport;
  std::vector<sf::Texture> passports;
  //sf::Texture* animals = new sf::Texture[3];
  //sf::Texture* passports = new sf::Texture[3];
  sf::Sprite* dragged = nullptr;
  sf::Sprite* passport_status = nullptr;
  sf::Vector2f drag_offset;
  sf::Clock game_time;
};

#endif // CRITTERSCROSSING_GAME_H
