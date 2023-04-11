#pragma once

#include "Zombie.h"

int createBackground(sf::VertexArray& rVA, sf::IntRect arena);

Zombie* createHorde(int numZombies, sf::IntRect arena);
