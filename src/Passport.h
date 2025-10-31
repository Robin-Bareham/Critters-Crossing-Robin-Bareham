#ifndef CRITTERSCROSSING_PASSPORT_H
#define CRITTERSCROSSING_PASSPORT_H

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Passport : public GameObject
{
public:
	Passport();
	~Passport();
	void setNewTexture(int location);

protected:

};

#endif // CRITTERSCROSSING_PASSPORT_H