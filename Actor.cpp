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
