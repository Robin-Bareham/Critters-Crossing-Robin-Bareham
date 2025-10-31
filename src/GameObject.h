
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
	void setNewTexture(std::string filename);
	void setTextureList(std::vector<sf::Texture>& texture_list);

protected:
	sf::Sprite sprite;
	sf::Texture sprite_texture;
	std::unique_ptr<std::vector<sf::Texture>> multiple_textures;
};

#endif // CRITTERSCROSSING_GAMEOBJECT_H