#include "Actor.h"
#include "StudentWorld.h"
#include "GameController.h"

// Students: Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int ID, int x, int y, Direction dir, float size, unsigned int depth)
	: GraphObject(ID, x, y, dir, size, depth)
{
	isAlive = true;
}

void Actor::setAlive(bool alive) {
	isAlive = alive;
}
bool Actor::checkAlive() {
	return isAlive;
}

void Actor::setWorld(StudentWorld &stuWorld) {
	sWorld = &stuWorld;
}

StudentWorld *Actor::getWorld() { 
	return sWorld;
}

bool Actor::canIntersect() {
	return true;
}

Actor::~Actor() {
}

Dirt::Dirt(int x, int y)
	: Actor(IID_DIRT, x, y, right, .25, 3) {
}

void Dirt::setLocation(int x, int y) {
	moveTo(x, y);
}

void Dirt::doSomething() {}

void Dirt::getAnnoyed() {}

bool Dirt::getVisible() {
	return isVisible();
}

Dirt::~Dirt() {};

FrackMan::FrackMan()
	: Actor(IID_PLAYER, 30, 60, right, 1.0, 0) {
	hitPoints = 10;
	waterUnits = 5;
	sonarCharges = 1;
	goldNuggets = 0;
	isAlive = true;
}

void FrackMan::doSomething() {
	if (!isAlive) { return; }
	if (!getWorld()->noBoulder(getX(), getY())) {
		hitPoints -= 10;
	}
	if (hitPoints <= 0) {
		getAnnoyed();
	}
	int ch;
	if (getWorld()->getKey(ch) == true)
	{
		// user hit a key this tick!
		switch (ch)
		{
			case KEY_PRESS_LEFT:
				setDirection(left);
				if(getX() > 0 && getWorld()->noBoulder(getX() - 1, getY()))
					moveTo(getX() - 1, getY());
				break;
			case KEY_PRESS_RIGHT:
				setDirection(right);
				if (getX() < 60 && getWorld()->noBoulder(getX() + 1, getY()))
					moveTo(getX() + 1, getY());
				break;
			case KEY_PRESS_UP:
				setDirection(up);
				if (getY() < 60 && getWorld()->noBoulder(getX(), getY() + 1))
					moveTo(getX(), getY() + 1);
				break;
			case KEY_PRESS_DOWN:
				setDirection(down);
				if (getY() > 0 && getWorld()->noBoulder(getX(), getY() - 1))
					moveTo(getX(), getY() - 1);
				break;
			case KEY_PRESS_SPACE:
				if (waterUnits > 0) {
					switch (getDirection()) {
					case right:
						getWorld()->createSquirt(getX() + 4, getY(), right);
						break;
					case left:
						getWorld()->createSquirt(getX() - 4, getY(), left);
						break;
					case up:
						getWorld()->createSquirt(getX(), getY() + 4, up);
						break;
					case down:
						getWorld()->createSquirt(getX(), getY() - 4, down);
						break;
					}
					waterUnits--;
				}
				break;
			case KEY_PRESS_TAB:
				if (goldNuggets > 0) {
					getWorld()->dropNugget(getX(), getY());
				}
			case 'Z':
			case 'z':
				if (sonarCharges > 0) {
					getWorld()->useSonar(getX(), getY());
					getWorld()->playSound(SOUND_SONAR);
					decSonar();
				}
				break;
			default:
				break;
			// etc...
		}
	}
}

void FrackMan::getAnnoyed() {
	setAlive(false);
}

int FrackMan::getHitPoints() {
	return hitPoints;
}
int FrackMan::getWaterUnits() {
	return waterUnits;
}
int FrackMan::getSonarCharges() {
	return sonarCharges;
}
int FrackMan::getGoldNuggets() {
	return goldNuggets;
}

void FrackMan::incGold() {
	goldNuggets++;
}
void FrackMan::decGold() {
	goldNuggets--;
}

void FrackMan::incSonar() {
	sonarCharges++;
}
void FrackMan::decSonar() {
	sonarCharges--;
}

FrackMan::~FrackMan() {}

Boulder::Boulder(int x, int y) 
	: Actor(IID_BOULDER, x, y, down, 1.0, 1) {
	setVisible(true);
	isStable = true;
	isFalling = false;
	waitTime = 30;
}
void Boulder::doSomething() {
	if (!checkAlive())
		return;
	if (isStable) {
		if (getWorld()->dirtUnderneathGone(getX(), getY())) {
			isStable = false;
			waitTime--;
		}
		return;
	}
	if (waitTime > 0) {
		waitTime--;
	}
	else if (waitTime <= 0) {
		if (isFalling == false) {
			isFalling = true;
			getWorld()->playSound(SOUND_FALLING_ROCK);
		}
		if (getWorld()->dirtUnderneathGone(getX(), getY()) && !getWorld()->boulderCollision(getX(), getY()))
			moveTo(getX(), getY() - 1);
		else
			setAlive(false);
	}
	return;
}
void Boulder::getAnnoyed() {}

bool Boulder::canIntersect() {
	return false;
}
Boulder::~Boulder() {}

Squirt::Squirt(int x, int y, Direction dir) 
	: Actor(IID_WATER_SPURT, x, y, dir, 1.0, 1) {
	setVisible(true);
	lifeTime = 4;
}

void Squirt::doSomething() {
	if (lifeTime > 0) {
		switch (getDirection()) {
			case right:
				(getX() < 60 ? moveTo(getX() + 1, getY()) : setAlive(false));
				break;
			case left:
				(getX() > 0 ? moveTo(getX() - 1, getY()) : setAlive(false));
				break;
			case up:
				(getY() < 60 ? moveTo(getX(), getY() + 1) : setAlive(false));
				break;
			case down:
				(getY() > 0 ? moveTo(getX(), getY() - 1) : setAlive(false));
				break;
		}
		lifeTime--;
	}
	if(getX() < 0 || getX() > 63 || getY() < 0 || getY() > 63 || lifeTime <= 0 || !getWorld()->noBoulderOrDirt(getX(), getY())) {
		setAlive(false);
	}
}
void Squirt::getAnnoyed() {}
Squirt::~Squirt() {}

Goodie::Goodie(int ID, int x, int y, bool permanent, int life = 100)
	: Actor(ID, x, y, right, 1.0, 2) {
	pickedUp = false;
	setVisible(false);
	isPermanent = permanent;
	lifeRemaining = life;
}
void Goodie::pickUp() {
	pickedUp = true;
	setAlive(false);
}
void Goodie::doSomething() {
	if (!checkAlive())
		return;
	if (getWorld()->itemCollision(getX(), getY()))
		pickUp();
	else if (getWorld()->itemNear(getX(), getY()))
		setVisible(true);
	if (!isPermanent) {
		doTempStuff();
	}
}

void Goodie::doTempStuff() {
	if (lifeRemaining <= 0) {
		setAlive(false);
	}
	lifeRemaining--;
}

bool Goodie::isPickedUp() {
	return pickedUp;
}

bool Goodie::getState() {
	return isPermanent;
}

void Goodie::getAnnoyed() {}
Goodie::~Goodie() {}



Barrel::Barrel (int x, int y) 
	: Goodie(IID_BARREL, x, y, true) {
}

void Barrel::doSomething() {
	Goodie::doSomething();
	if (!checkAlive()) {
		getWorld()->increaseScore(1000);
		getWorld()->playSound(SOUND_FOUND_OIL);
		getWorld()->decOil();
	}
}

Barrel::~Barrel() {}

Nugget::Nugget(int x, int y, bool state)
	: Goodie(IID_GOLD, x, y, state) {
	setVisible(!state);
}

void Nugget::doSomething() {
	if (getState())	{
		if (!checkAlive()) {
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->increaseScore(10);
			getWorld()->incGold();
			return;
		}
		else {
			Goodie::doSomething();
			if (!checkAlive()) {
				getWorld()->playSound(SOUND_GOT_GOODIE);
				getWorld()->increaseScore(10);
				getWorld()->incGold();
				return;
			}
		}
	}
	else {
		doTempStuff();
	}
}

Nugget::~Nugget() {}

SonarKit::SonarKit(int x, int y, int life) 
	: Goodie(IID_SONAR, x, y, false, life) {
	setVisible(true);
}
void SonarKit::doSomething() {
	Goodie::doSomething();
	doTempStuff();
	if (!checkAlive()) {
		getWorld()->playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(75);
		getWorld()->incSonar();
	}
}
SonarKit::~SonarKit() {}


