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