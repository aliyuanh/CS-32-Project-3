#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Level.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
	Actor(int imageID, double startX, double startY, StudentWorld* world, Direction dir = 0, int depth = 0, double size = 1.0);
	virtual void doSomething() = 0;
	virtual bool blocker();
	virtual bool fullBlock();
	virtual bool isExit();
	virtual bool canKill();
	virtual bool isAlive();
	virtual void die();
	virtual bool canBeKilled();
	virtual bool canPickUp();
	virtual bool canFire();
	virtual bool canExplode();
	virtual bool canDrop();
	virtual bool canHeal();
	virtual int numberInfected();
	virtual void incrementInfect();
	virtual bool testInfected();
	virtual void infect();
	virtual int getFlames();
	virtual int getLandmines();
	virtual int getVaccines();
	virtual void giveFlames();	
	virtual void giveLandmines();
	virtual void giveVaccines();
	virtual void cure();
	virtual void fireFlame();
	virtual void explode();
	int numTicksHere();
	void incrementTicks();
	virtual StudentWorld* getWorld();
	virtual void setWorld(StudentWorld* world);
	virtual Actor* getPenny();
	virtual void setPenny(Actor* p);
	virtual bool canExit();
private:
	bool living;
	int numInfected;
	bool isInfected;
	int numFlames;
	int numLandmines;
	int numVaccines;
	StudentWorld* myWorld;
	int numTicks;
	Actor* myPenny;
};


class Penelope : public Actor {
public: 
	Penelope(int myX, int myY, StudentWorld* myWorld) :Actor(IID_PLAYER, SPRITE_WIDTH*myX, SPRITE_HEIGHT*myY, myWorld, 0, 0, 1.0) {
	
	}
	virtual void doSomething();
	virtual bool canBeKilled();
private:
};

//All Moving things -- Citizens, Dumb Zombies, Smart Zombies
class Moving : public Actor {
public: 
	Moving(int ID, double myX, double myY, Direction dir, int depth);
	virtual bool blocker() {
		return true;
	}//all moving are blockers, so true
	virtual bool canBeKilled();
private:
};

class Citizen : public Moving {
public:
	Citizen(int posX, int posY, StudentWorld* world, Actor* penny);
	virtual void doSomething();
	virtual bool canExit();
	virtual void die();
	virtual bool fullBlock();
private:
	int dist_p;
	int dist_z;
	Actor* pene;
};

class Zombie :public Moving {
public:
	Zombie(int posX, int posY);

};
class DumbZombie : public Zombie {
public: 
	DumbZombie(int posX, int posY, StudentWorld* world);
	virtual void doSomething();
};

class SmartZombie : public Zombie {
public:
	SmartZombie(int posX, int posY, StudentWorld* world);
	virtual void doSomething();
};

class Stationary : public Actor {
public:
	Stationary(int ID, double myX, double myY, Direction dir, int depth);
	virtual void doSomething() {
		//does nothing bc it's stationary 
	}
private:
};

class Wall : public Stationary {
public:
	Wall(int posX, int posY, StudentWorld* world);
	virtual bool blocker();
	virtual bool fullBlock();
private:
};

class Exit : public Stationary {
public:
	Exit(int posX, int posY, StudentWorld* world);
	virtual bool isExit();
};

class Trap : public Stationary {
public:
	Trap(int posX, int posY, StudentWorld* world);
	virtual void doSomething();
	virtual bool canKill();
	virtual bool blocker();
	virtual bool isKillable();
};

class Flame : public Stationary {
public:
	Flame(int posX, int posY, StudentWorld* world);
	virtual void doSomething();
	virtual bool canKill();
private:
	int numTicksAlive;
};

class Mine :public Stationary {
public:
	Mine(int x, int y, StudentWorld* world);
	virtual void doSomething();
	virtual void explode();
	virtual bool canExplode();
private:
	int numTicksAlive;
};
class Goodie : public Stationary {
public:
	Goodie(int ID, int posX, int posY);
	virtual bool canPickUp();
	virtual bool canBeKilled();
};

class GasCan :public Goodie {
public:
	GasCan(int posX, int posY, StudentWorld* world);
	virtual void doSomething();
	virtual bool canFire();
};
class MineGoodie :public Goodie {
public:
	MineGoodie(int posX, int posY, StudentWorld* world);
	virtual void doSomething();
	virtual bool canDrop();
};
class VaccineGoodie :public Goodie {
public:
	VaccineGoodie(int posX, int posY, StudentWorld* world);
	virtual void doSomething();
	virtual bool canHeal();
};



//TODO: add Trap class (landmines, pits, flames, vomit)
//possibly make Object class for traps and goodies? + Walls and objects 
#endif // ACTOR_H_
