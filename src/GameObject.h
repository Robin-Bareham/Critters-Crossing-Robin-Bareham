
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

protected:
	sf::Sprite sprite;
	sf::Texture sprite_texture;
};

#endif // CRITTERSCROSSING_GAMEOBJECT_H