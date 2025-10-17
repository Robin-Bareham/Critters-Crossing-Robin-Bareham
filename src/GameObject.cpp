#include "GameObject.h"
#include <iostream>

GameObject::GameObject() {}
GameObject::~GameObject() {}

bool GameObject::initaliseSprite(std::string filename)
{
	if(!sprite_texture.loadFromFile(filename))
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

void GameObject::setVisible(bool value)
{
	is_visible = value;
}

bool GameObject::getVisible()
{
	return is_visible;
}