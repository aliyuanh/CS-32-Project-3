#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

Actor::Actor(int imageID, double startX, double startY, Direction dir, int depth, double size)
	:GraphObject(imageID, startX, startY, dir, depth, size) {

}

bool Actor::blocker()
{
	return false;
}

bool Actor::isExit()
{
	return false;
}


void Penelope::doSomething() {
	int theKey = -999;
	myWorld->getKey(theKey);
	bool moving = false;
	//now, theKey has what key was pressed
	int toX = getX();
	int toY = getY();
	switch (theKey) {
	case KEY_PRESS_LEFT:
		toX = toX - 4;
		setDirection(left);
		moving = true;
		break;
	case KEY_PRESS_RIGHT:
		toX = toX + 4;
		setDirection(right);
		moving = true;
		break;
	case KEY_PRESS_UP:
		toY = toY + 4;
		setDirection(up);
		moving = true;
		break;
	case KEY_PRESS_DOWN:
		toY = toY - 4;
		setDirection(down);
		moving = true;
		break;
	case KEY_PRESS_SPACE:
	case KEY_PRESS_TAB:
	case KEY_PRESS_ENTER:
	default:
		break;
	}
	if (moving) {
		if (myWorld->checkCollision(toX, toY)) {
			//std::cout << "uh oh! can't move, it's collidin" << std::endl;
			//std::cout << toX << "," << toY << std::endl;
		}
		else {
			moveTo(toX, toY);
		}
	}
}

//All Moving Things

Moving::Moving(int ID, double myX, double myY, Direction dir, int depth) :Actor(ID, SPRITE_WIDTH*myX, SPRITE_HEIGHT * myY, dir, depth, 1.0)
{

}

//Citizens 
Citizen::Citizen(int posX, int posY):Moving(IID_CITIZEN, posX, posY, right, 0)
{

}
void Citizen::doSomething() {

}

Zombie::Zombie(int posX, int posY):Moving(IID_ZOMBIE, posX, posY, right, 0) {

}

DumbZombie::DumbZombie(int posX, int posY) : Zombie(posX, posY) {

}

void DumbZombie::doSomething() {

}

SmartZombie::SmartZombie(int posX, int posY) :Zombie(posX, posY) {

}
void SmartZombie::doSomething() {

}
//All Stationary Things
Stationary::Stationary(int ID, double myX, double myY, Direction dir, int depth) :Actor(ID, SPRITE_WIDTH*myX, SPRITE_HEIGHT * myY, dir, depth, 1.0)
{
}
//Walls
Wall::Wall(int posX, int posY):Stationary(IID_WALL, posX, posY, right, 0)
{
	//doesn't need to do anything else bc it's...a wall
}

bool Wall::blocker()
{
	//the only blocker! 
	return true;
}

//Exits

Exit::Exit(int posX, int posY):Stationary(IID_EXIT, posX, posY, right, 1)
{

}

bool Exit::isExit()
{
	return true;
}
//Goodies

Goodie::Goodie(int ID, int posX, int posY) :Stationary(ID, posX, posY, right, 1) {

}

GasCan::GasCan(int posX, int posY) : Goodie(IID_GAS_CAN_GOODIE, posX, posY) {

}
void GasCan::doSomething() {

}

MineGoodie::MineGoodie(int posX, int posY):Goodie(IID_LANDMINE_GOODIE, posX, posY)
{
}
void MineGoodie::doSomething() {

}

VaccineGoodie::VaccineGoodie(int posX, int posY):Goodie(IID_VACCINE_GOODIE, posX, posY)
{
}
void VaccineGoodie::doSomething() {


}

Trap::Trap(int posX, int posY):Stationary(IID_PIT, posX, posY, right, 1)
{
}
