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


#endif // ACTOR_H_
