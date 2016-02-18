#include "StudentWorld.h"
#include "Actor.h"
#include "GraphObject.h"
#include <sstream>
#include <string>
#include <list>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

int StudentWorld::init() { 
	fMan = new FrackMan();
	fMan->setWorld(*this);
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			dirtArray[i][j] = new Dirt(i, j);
			if (i < 30 || i > 33 || j < 4) {
				dirtArray[i][j]->setVisible(true);
			}
		}
	}
	fMan->setVisible(true);
	int randX, randY;
	for (int i = 0; i < (getLevel() / 2 + 2 < 6 ? getLevel() / 2 + 2 : 6); i++) {
		do {
			randX = rand() % 61;
			randY = rand() % 37 + 20;
		} while (!withinAllowableDistance(randX, randY));
		actorList.push_front(new Boulder(randX, randY));
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				dirtArray[randX + j][randY + k]->setVisible(false);
	}
/*	actorList.push_front(new Boulder(15, 40));
	for (int j = 0; j < 4; j++)
		for (int k = 0; k < 4; k++)
			dirtArray[15 + j][40 + k]->setVisible(false);
	actorList.push_front(new Boulder(18, 20));
	for (int j = 0; j < 4; j++)
		for (int k = 0; k < 4; k++)
			dirtArray[18 + j][20 + k]->setVisible(false);*/
	return GWSTATUS_CONTINUE_GAME; 
}

int StudentWorld::move() {
	setDisplayText();
	fMan->doSomething();
	if (fMan->getX() <= 60 && fMan->getY() <= 60) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (fMan->getX() + i < 64 && fMan->getY() + j < 60 && dirtArray[fMan->getX() + i][fMan->getY() + j]->getVisible()) {
					playSound(SOUND_DIG);
					dirtArray[fMan->getX() + i][fMan->getY() + j]->setVisible(false);
				}
			}
		}
	}
	list<Actor*>::iterator it;
	it = actorList.begin();
	while (it != actorList.end()) {
		(*it)->setWorld(*this);
		(*it)->doSomething();
		it++;
	}
	it = actorList.begin();
	while (it != actorList.end()) {
		if (!(*it)->checkAlive()) {
			Actor* KillMe = (*it);
			it = actorList.erase(it);
			delete KillMe;
		}
		else {
			it++;
		}
	}
	if (!fMan->checkAlive()) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
	delete fMan;
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 60; j++) {
			delete dirtArray[i][j];
		}
	}
	list<Actor*>::iterator it;
	it = actorList.begin();
	while (it != actorList.end()) {
		Actor* KillMe = (*it);
		it = actorList.erase(it);
		delete KillMe;
	}
}

bool StudentWorld::withinAllowableDistance(int x, int y) {
	if (x >= 26 && x <= 33) {
		return false;
	}
	list<Actor*>::iterator it;
	it = actorList.begin();
	while (it != actorList.end()) {
		if (sqrt(pow((*it)->getX() - x, 2) + pow((*it)->getY() - y, 2)) <= 6)
			return false;
		it++;
	}
	return true;
}

bool StudentWorld::noBoulder(int x, int y) {
	list<Actor*>::iterator it;
	it = actorList.begin();
	while (it != actorList.end()) {
		if (!(*it)->canIntersect())
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					for (int k = 0; k < 4; k++)
						for (int l = 0; l < 4; l++)
							if (x + k == (*it)->getX() + i && y + l == (*it)->getY() + j)
								return false;
		it++;
	}
	return true;
}

bool StudentWorld::dirtUnderneathGone(int x, int y) {
	if (y == 0)
		return false;
	for (int i = 0; i < 4; i++) {
		if (dirtArray[x + i][y - 1]->getVisible()) {
			return false;
		}
	}
	return true;
}

bool StudentWorld::boulderCollision(int x, int y) {
	list<Actor*>::iterator it;
	it = actorList.begin();
	while (it != actorList.end()) {
		if (!(*it)->canIntersect())
			if ((*it)->getX() - 4 < x && (*it)->getX() + 4 > x && (*it)->getY() == y - 3)
				return true;
		it++;
	}
	return false;
}


void StudentWorld::setDisplayText()
{
	int score = getScore();
	if (score > 999999) {
		score = 999999;
	}
	int level = getLevel();
	if (level > 99) {
		level == 99;
	}
	int lives = getLives();
	int health = fMan->getHitPoints();
	int squirts = fMan->getWaterUnits();
	if (squirts > 99) {
		squirts == 99;
	}
	int gold = fMan->getGoldNuggets();
	if (gold > 99) {
		gold == 99;
	}
	int sonar = fMan->getSonarCharges();
	if (sonar > 99) {
		sonar == 99;
	}
	int barrelsLeft = 20; 
	if (barrelsLeft > 99) {
		barrelsLeft == 99;
	}
	bool firstNonZeroReached = false;
	string s = "Scr: ";
	for (int i = 5; i >= 0; i--) {
		int temp = score / pow(10, i);
		stringstream out; 
		out << temp;
		s += out.str();
		score -= temp*pow(10, i);
	}
	s += "  Lvl: ";
	s += convertIntDisplay(level, 2);
	s += "  Lives: ";
	s += convertIntDisplay(lives, 1);
	s += "  Hlth: ";
	s += convertIntDisplay(health*10, 3);
	s += "%  Wtr: ";
	s += convertIntDisplay(squirts, 2);
	s += "  Gld: ";
	s += convertIntDisplay(gold, 2);
	s += "  Sonar: ";
	s += convertIntDisplay(sonar, 2);
	s += "  Oil Left: ";
	s += convertIntDisplay(barrelsLeft, 2);
	setGameStatText(s);
}

std::string StudentWorld::convertIntDisplay(int input, int digits) {
	bool firstNonZeroReached = false;
	string result;
	for (int i = digits - 1; i >= 0; i--) {
		int temp = input / pow(10, i);
		stringstream out;
		out << temp;
		if (temp != 0 || firstNonZeroReached || i == 0) {
			result += out.str();
			firstNonZeroReached = true;
		}
		else {
			result += " ";
		}
		input -= temp*pow(10, i);
	}
	return result;
}

/*StudentWorld::~StudentWorld() {
	cleanUp();
}*/

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

