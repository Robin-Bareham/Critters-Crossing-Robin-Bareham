#ifndef CRITTERSCROSSING_ANIMAL_H
#define CRITTERSCROSSING_ANIMAL_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Animal : public GameObject
{
public:
	Animal();
	~Animal();
	void setNewTexture(int location);

protected:

};

#endif // CRITTERSCROSSING_ANIMAL_H