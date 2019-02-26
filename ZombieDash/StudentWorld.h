#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	~StudentWorld();
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
	void moveEnts();
	void nextLevel();
	void placeLandmine(int x, int y);
	void activateLandmine(int x, int y);
	void fire(int x, int y, Direction dir);
	bool checkCollision(int x, int y); //if it intersects w any wall, cit, or zomb, then return false 
	bool hitPenny(int x, int y);
	void checkTheDead();
	bool checkKillable(int x, int y);
	bool checkExit(int x, int y);
	void turnCitizenToZombie(int x, int y);
	bool checkObjectOverlap(Actor* p);
	bool personMoveFreely(Actor* p, int x, int y);
	void citizenDie();

private:
	//vector<Actor*> peeps;
	Actor* penny;
	std::list<Actor*> entities;
	int score;
	int numLevel;
	int numInfected;
	int numFlames;
	int numLandmines;
	int numVaccines;
	int numCitizensToSave;
};

#endif // STUDENTWORLD_H_
