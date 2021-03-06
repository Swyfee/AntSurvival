#include "../headers/field.h"
field::field()
{
	anthill *acrobat = new anthill(2525);
	anthills.push_back(acrobat);
}

field::field(std::string nameFile)
{
}

field::~field()
{
	for (square *e : squares)
	{
		delete e;
	};
	for (anthill *e : anthills)
	{
		delete e;
	};
}

std::array<std::array<char, width>, height> field::generateRandomMap()
{
	std::srand(std::time(nullptr));
	std::array<std::array<char, width>, height> b;
	int rand, numO = 0, numF = 0;
	for (int i = 0; i < height; i++)
	{
		for (int u = 0; u < width; u++)
		{
			rand = std::rand() % 10;
			if (i == height / 2 && u == width / 2)
			{
				b[i][u] = 'H';
			}
			else if (rand == 0) //food=1/10
			{
				numF++;
				b[i][u] = 'F';
			}
			else if (rand == 1 || rand == 2) //obstacle=2/10
			{
				numO++;
				b[i][u] = 'O';
			}
			else
			{
				b[i][u] = 'E';
			}
		}
	}
	return b;
}

void field::generateSquares()
{
	std::array<std::array<char, width>, height> defaultMap = generateRandomMap();
	for (int i = 0; i < height; i++)
	{
		for (int u = 0; u < width; u++)
		{
			switch (defaultMap[i][u])
			{
			case 'E':
				squares.push_back(new square(i, u, TypeSquare::Empty));
				break;
			case 'O':
				squares.push_back(new square(i, u, TypeSquare::Obstacle));
				break;
			case 'F':
				squares.push_back(new square(i, u, TypeSquare::Food));
				break;
			case 'H':
				squares.push_back(new square(i, u, TypeSquare::Anthill));
				break;

			default:
				break;
			}
		}
	}
}

std::vector<square *> field::getSquares()
{
	return squares;
}

void field::affichesSquares()
{
	for (int i = 0; i < height; i++)
	{
		std::cout << "| ";
		for (int u = 0; u < width; u++)
		{
			switch (squares[(i * width) + u]->getType())
			{
			case TypeSquare::Empty:
				std::cout << "_";
				break;
			case TypeSquare::Obstacle:
				std::cout << "O";
				break;
			case TypeSquare::Food:
				std::cout << "F";
				break;
			case TypeSquare::Anthill:
				std::cout << "H";
				break;
			default:
				break;
			}
		}
		std::cout << " |" << std::endl;
	}
}

void field::afficheDefaut()
{
	generateSquares();

	for (int i = 0; i < height; i++)
	{
		std::cout << "| ";
		for (int u = 0; u < width; u++)
		{
			switch (squares[(i * width) + u]->getType())
			{
			case TypeSquare::Empty:
				std::cout << "_";
				break;
			case TypeSquare::Obstacle:
				std::cout << "O";
				break;
			case TypeSquare::Food:
				std::cout << "F";
				break;
			case TypeSquare::Anthill:
				std::cout << "H";
				break;
			default:
				break;
			}
		}
		std::cout << " |" << std::endl;
	}
}

void field::drawContainer(sf::RenderWindow &window)
{
	for (std::size_t i = 0; i < squares.size(); ++i)
	{
		window.draw(*squares[i]);
	}
}

void field::feedAnt()
{
	for (anthill *e : anthills)
	{
		e->feedAnt();
	}
}

void field::move()
{
	int xwarrior;
	int ywarrior;
	int caseWarrior;
	int mybestCase;
	bool fullFood = false;

	for (anthill *e : anthills)
	{
		std::vector<warrior *> warriors = e->getWarriors();

		for (int i = 0; i < warriors.size(); i++)
		{
			int hisAnthill = e->getahCase();
			fullFood = warriors[i]->getfood_state();
			xwarrior = warriors[i]->getX();
			ywarrior = warriors[i]->getY();
			caseWarrior = xwarrior * width + ywarrior;
			//check des alentours
			mybestCase = bestCase(caseWarrior, fullFood, warriors[i], hisAnthill);
			if (mybestCase == hisAnthill)
			{
				e->refill(warriors[i]);
				fullFood = warriors[i]->getfood_state();
			}

			warriors[i]->move(squares[mybestCase]->getX(), squares[mybestCase]->getY());
			switch (squares[caseWarrior]->getType())
			{
			case TypeSquare::Empty:
				squares[caseWarrior]->setRectangle(sf::Color(255, 255, 255));
				break;
			case TypeSquare::Obstacle:
				squares[caseWarrior]->setRectangle(sf::Color(0, 0, 0));
				break;
			case TypeSquare::Food:
				squares[caseWarrior]->setRectangle(sf::Color(255, 50, 50));
				warriors[i]->getFood(squares[caseWarrior]);
				break;
			case TypeSquare::Anthill:
				squares[caseWarrior]->setRectangle(sf::Color(50, 255, 50));
				break;
			default:
				break;
			}
			squares[caseWarrior]->changeAntInIt();
			squares[mybestCase]->changeAntInIt();
		}
	}
}

void field::endTurn()
{
	for (anthill *e : anthills)
	{
		e->endTurn();
	}
}

bool field::checkLife()
{
	bool allAntDead = true;
	for (anthill *e : anthills)
	{
		bool antHillEnd = e->checkLife();
		allAntDead = allAntDead && antHillEnd;
		if (antHillEnd)
		{
			delete e;
		}
	}
	return allAntDead;
}

void field::antGetBack()
{
}

bool field::checkLivingAnt()
{
	bool allAntDead = true;
	for (anthill *e : anthills)
	{
		allAntDead = allAntDead && e->checkLivingAnt();
	}

	return allAntDead;
}
int field::bestCase(int caseWarrior, bool fullFood, warrior *myWarrior, int hisAnthill)
{
	int best = 0, counter = 0, emptyTab[4] = {0}, FoodCase = 0, anthillCase = 0;
	int caseCheck[4];
	caseCheck[0] = caseWarrior - 1;
	caseCheck[1] = caseWarrior + 1;
	caseCheck[2] = caseWarrior - 50;
	caseCheck[3] = caseWarrior + 50;
	for (int i = 0; i < 8; i++)
	{
		if (caseWarrior % 50 == 0 && caseCheck[i] % 49 == 0)
		{
			break;
		}
		else if (caseWarrior % 49 == 0 && caseCheck[i] % 50 == 0)
		{
			break;
		}
		else if (caseWarrior < 50 && caseCheck[i] < 0)
		{
			break;
		}
		else if (caseWarrior > 4949 && caseCheck[i] > 4999)
		{
			break;
		}
		switch (squares[caseCheck[i]]->getType())
		{
		case TypeSquare::Empty:
			if ((myWarrior->lastCase() != false) && (caseCheck[i] == myWarrior->lastCase()))
			{
				break; // on ne retient pas la dernière case empruntée
			}
			else
			{
				emptyTab[counter] = caseCheck[i];
				counter++;
				break;
			}
		case TypeSquare::Food:
			FoodCase = caseCheck[i];
			break;
		case TypeSquare::Anthill:
			anthillCase = caseCheck[i];
			break;
		default:
			break;
		}
	}
	if (fullFood == false)
	{
		if (FoodCase != 0)
		{
			best = FoodCase;
		}
		else if (emptyTab[0] != 0)
		{
			best = emptyTab[std::rand() % counter];
		}
		else
		{
			best = caseWarrior;
		}
		myWarrior->addCase(best); //ajoute une case dans le chemin ue la fourmis à prise
	}
	else
	{
		myWarrior->deleteLast(); //Delete la dernière case de son chemin
		if (anthillCase != 0 && anthillCase == hisAnthill)
		{
			myWarrior->deleteAll();
			best = anthillCase;
		}
		else
		{
			best = myWarrior->lastCase(); //Montre la dernière case du vector
		}
	}
	if (best == 0)
	{
		std::cout << "AAAHh";
	}
	return best;
}

void field::layEggs()
{
	for (anthill *e : anthills)
	{
		e->layEggs();
	}
}

void field::circleOfLife()
{
	for (anthill *e : anthills)
	{
		e->circleOfLife();
	}
}

void field::printStateLog()
{
	for (anthill *e : anthills)
	{
		e->printStateLog();
	}
}

void field::randomFoodSquare()
{
}