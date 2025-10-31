#include "Animal.h"
#include <iostream>

Animal::Animal() {}
Animal::~Animal() {}

void Animal::setNewTexture(int location)
{
	sprite.setTexture(multiple_textures->at(location), true);
	sprite.setScale(1.8, 1.8);
}