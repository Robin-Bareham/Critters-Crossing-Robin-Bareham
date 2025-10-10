
#ifndef CRITTERSCROSSING_GAME_H
#define CRITTERSCROSSING_GAME_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"

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
	 void loadAnimals();
	 void loadPassports();
	 void newAnimal();
	 void dragSprite(sf::Sprite* sprite, sf::Sprite* stamp_sprite);
	 void dragOffset(sf::Sprite* sprite);
	 void updateStampPos();
	 bool collisionReturnCheck(sf::RectangleShape& rectangle, sf::Vector2f& mouse);

  sf::RenderWindow& window;
  // Game States
  enum game_state {MENU,GAMEPLAY,END};
  game_state current_state;
  bool paused = false;
  bool passport_accepted = false;
  bool passport_rejected = false;
  bool should_accept = false;
  bool started_dragging = false;
  int passports_right = 0;
  int passports_wrong = 0;
  int lives = 3;
  // Text
  sf::Font font;
  sf::Text m_title_txt;
  sf::Text p_pause_txt;
  sf::Text g_lives_txt;
  sf::Text e_end_txt;
  sf::Text e_final_score_txt;
  // Sprites
  sf::Sprite background;
  sf::Texture background_texture;
  sf::RectangleShape pause_rect;
  sf::RectangleShape return_rect;
  GameObject accept_btn;
  GameObject reject_btn;
  GameObject reject_stamp;
  GameObject accept_stamp;
  sf::Sprite* character;
  sf::Sprite* passport;
  sf::Texture* animals = new sf::Texture[3];
  sf::Texture* passports = new sf::Texture[3];
  sf::Sprite* dragged = nullptr;
  sf::Sprite* passport_status = nullptr;
  sf::Vector2f drag_offset;
};

#endif // CRITTERSCROSSING_GAME_H
