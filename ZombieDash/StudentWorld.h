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
	bool checkCollision(int x, int y); //if it intersects w any wall, cit, or zomb, then return false 
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
