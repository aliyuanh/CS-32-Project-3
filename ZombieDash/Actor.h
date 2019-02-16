#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Level.h"
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class Actor : public GraphObject {
public:
	Actor(int imageID, double startX, double startY, Direction dir = 0, int depth = 0, double size = 1.0)
		:GraphObject(imageID, startX, startY, dir, depth, size) {

	}
	virtual void doSomething() = 0;
private:


};


class Penelope : public Actor {
public: 
	Penelope(int myX, int myY):Actor(IID_PLAYER, SPRITE_WIDTH*myX, SPRITE_HEIGHT*myY,  0, 0, 1.0) {
		
	}
	virtual void doSomething();
private:

};

class Wall : public Actor {
public:
	Wall(int posX, int posY) :Actor(IID_WALL, SPRITE_WIDTH*posX, SPRITE_HEIGHT * posY, 0, 0, 1.0) {

	}
	virtual void doSomething() {
		//do nothing...this is a wall 
	}
private:


};
//TODO: add Person class (citizen derived, zombie --> dumb and smart)
//TODO: add Trap class (landmines, pits, flames, vomit)
//TODO: add Goodie class (gasCan, Vaccine, Landmine)
//possibly make Object class for traps and goodies? + Walls and objects 
#endif // ACTOR_H_
