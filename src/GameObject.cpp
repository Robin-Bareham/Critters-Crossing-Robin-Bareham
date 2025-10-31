#include "GameObject.h"
#include <iostream>

GameObject::GameObject() {}
GameObject::~GameObject() {}

bool GameObject::initaliseSprite(std::string filename)
{
	if (!sprite_texture.loadFromFile(filename))
	{
		std::cout << filename << " didn't load.\n";
	}
	sprite.setTexture(sprite_texture);
	return true;
}

sf::Sprite* GameObject::getSprite()
{
	return &sprite;
}

void GameObject::setSpritePos(float temp_x, float temp_y)
{
	sprite.setPosition(temp_x,temp_y);
}

void GameObject::setNewTexture(std::string filename)
{
	if (!sprite_texture.loadFromFile(filename))
	{
		std::cout << filename << " didn't load.\n";
	}
	sprite.setTexture(sprite_texture);
}

void GameObject::setTextureList(std::vector<sf::Texture>& texture_list)
{
	//Making a pointer to the vector.
	multiple_textures = std::make_unique<std::vector<sf::Texture>>(texture_list);
}

