
#ifndef CRITTERSCROSSING_GAMEOBJECT_H
#define CRITTERSCROSSING_GAMEOBJECT_H

#include <SFML/Graphics.hpp>

class GameObject
{
public:
	GameObject();
	~GameObject();
	bool initaliseSprite(std::string filename);
	sf::Sprite* getSprite();
	void setSpritePos(float temp_x, float temp_y);
	void setVisible(bool value);
	bool getVisible();

protected:
	sf::Sprite sprite;
	sf::Texture sprite_texture;
	bool is_visible = false;
};

#endif // CRITTERSCROSSING_GAMEOBJECT_H