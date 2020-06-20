#ifndef FIELD_H_ /* Include guard */
#define FIELD_H_

#include "square.h"
#include "anthill.h"

#include <array>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

const int height = 100;
const int width = 50;

class field
{
private:
	std::vector<square *> squares;
	std::vector<anthill *> anthills;

	std::array<std::array<char, width>, height> generateRandomMap();

public:
	void generateSquares();
	void affichesSquares();
	void afficheDefaut();
	void drawContainer(sf::RenderWindow &window);
	void move();
	void checkLife();
	bool checkLivingAnt();
	void antGetBack();
	int bestCase(int caseWarrior, bool fullfood);

	field();
	field(std::string nameFile);
	~field();

	std::vector<square *> getSquares();
};

#endif // FIELD_H_