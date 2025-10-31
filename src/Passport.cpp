#include "Passport.h"
#include <iostream>

Passport::Passport() {}
Passport::~Passport() {}


void Passport::setNewTexture(int location)
{
	sprite.setTexture(multiple_textures->at(location), true);
	sprite.setScale(0.6, 0.6);
}