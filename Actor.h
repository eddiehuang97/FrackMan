#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(int ID, int x, int y, Direction dir, float size, unsigned int depth);
	void setWorld(StudentWorld &stuWorld);
	StudentWorld * getWorld();
	virtual void doSomething() = 0;
	virtual void getAnnoyed() = 0;
	void setAlive(bool alive);
	bool checkAlive();
	virtual bool canIntersect();
	virtual ~Actor();
private:
	bool isAlive;
	StudentWorld* sWorld;
};

class Dirt : public Actor
{
public:
	Dirt(int x = 0, int y = 0);
	void setLocation(int x, int y);
	void doSomething();
	void getAnnoyed();
	bool getVisible();
	virtual ~Dirt();
};

class FrackMan : public Actor
{
public:
	FrackMan();
	void doSomething();
	void getAnnoyed();
	int getHitPoints();
	int getWaterUnits();
	int getSonarCharges();
	int getGoldNuggets();
	void incGold();
	void decGold();
	void incSonar();
	void decSonar();
	virtual ~FrackMan();
private:
	int hitPoints;
	int waterUnits;
	int sonarCharges;
	int goldNuggets;
	bool isAlive;
};

class Boulder : public Actor
{
public:
	Boulder(int x, int y);
	void doSomething();
	void getAnnoyed();
	bool canIntersect();
	virtual ~Boulder();
private:
	bool isStable;
	int waitTime;
	bool isFalling;
};

class Squirt : public Actor
{
public:
	Squirt(int x, int y, Direction dir);
	void doSomething();
	void getAnnoyed();
	virtual ~Squirt();
private:
	int lifeTime;
};

class Goodie : public Actor
{
public:
	Goodie(int ID, int x, int y, bool permanent, int life);
	virtual void pickUp();
	virtual void doSomething();
	void getAnnoyed();
	void doTempStuff();
	bool isPickedUp();
	bool getState();
	virtual ~Goodie();
private:
	bool pickedUp;
	bool isPermanent;
	int lifeRemaining;
};

class Barrel : public Goodie
{
public:
	Barrel(int x, int y);
	void doSomething();
	virtual ~Barrel();
};

class Nugget : public Goodie
{
public:
	Nugget(int x, int y, bool state);
	void doSomething();
	virtual ~Nugget();
};

class SonarKit : public Goodie
{
public:
	SonarKit(int x, int y, int life);
	void doSomething();
	virtual ~SonarKit();
};


#endif // ACTOR_H_
