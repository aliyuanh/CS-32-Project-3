#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Level.h"
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
	Actor(int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0);
	virtual void doSomething() = 0;
	virtual bool blocker();
	virtual bool isExit();
private:

};


class Penelope : public Actor {
public: 
	Penelope(int myX, int myY, StudentWorld* world):Actor(IID_PLAYER, SPRITE_WIDTH*myX, SPRITE_HEIGHT*myY,  0, 0, 1.0) {
		myWorld = world;
	}
	virtual void doSomething();
private:
	StudentWorld* myWorld;
};

//All Moving things -- Citizens, Dumb Zombies, Smart Zombies
class Moving : public Actor {
public: 
	Moving(int ID, double myX, double myY, Direction dir, int depth);
	virtual bool blocker() {
		return true;
	}//all moving are blockers, so true
private:
};

class Citizen : public Moving {
public:
	Citizen(int posX, int posY);
	virtual void doSomething();
private:


};

class Zombie :public Moving {
public:
	Zombie(int posX, int posY);

};
class DumbZombie : public Zombie {
public: 
	DumbZombie(int posX, int posY);
	virtual void doSomething();
};

class SmartZombie : public Zombie {
public:
	SmartZombie(int posX, int posY);
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
	Wall(int posX, int posY);
	virtual bool blocker();
private:


};

class Exit : public Stationary {
public:
	Exit(int posX, int posY);
	virtual bool isExit();
};

class Trap : public Stationary {
public:
	Trap(int posX, int posY);

};

class Goodie : public Stationary {
public:
	Goodie(int ID, int posX, int posY);
};

class GasCan :public Goodie {
public:
	GasCan(int posX, int posY);
	virtual void doSomething();
};
class MineGoodie :public Goodie {
public:
	MineGoodie(int posX, int posY);
	virtual void doSomething();

};
class VaccineGoodie :public Goodie {
public:
	VaccineGoodie(int posX, int posY);
	virtual void doSomething();

};

//TODO: add Person class (citizen derived, zombie --> dumb and smart)
//TODO: add Trap class (landmines, pits, flames, vomit)
//TODO: add Goodie class (gasCan, Vaccine, Landmine)
//possibly make Object class for traps and goodies? + Walls and objects 
#endif // ACTOR_H_
