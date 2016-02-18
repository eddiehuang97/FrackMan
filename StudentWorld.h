#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <list>
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Dirt;
class FrackMan;
class Actor;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}


	virtual int init();

	virtual int move();

	virtual void cleanUp();

	bool noBoulder(int x, int y);

	bool dirtUnderneathGone(int x, int y);
	bool boulderCollision(int x, int y);

	// ~StudentWorld(); // cause if you close the program instead of pressing q you don't want a memory leak

private:
	Dirt* dirtArray[64][60];
	FrackMan* fMan;
	std::list<Actor*> actorList;
	bool withinAllowableDistance(int x, int y);
	void setDisplayText();
	std::string convertIntDisplay(int input, int digits);
};

#endif // STUDENTWORLD_H_
