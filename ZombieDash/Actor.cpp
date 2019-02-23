#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

Actor::Actor(int imageID, double startX, double startY, StudentWorld* world,Direction dir, int depth, double size)
	:GraphObject(imageID, startX, startY, dir, depth, size) {
	living = true;
	numInfected = 0;
	isInfected = false;
	numFlames = 0;
	numLandmines = 0;
	numVaccines = 0;
	myWorld = world;
}

bool Actor::blocker()
{
	return false;
}

bool Actor::isExit()
{
	return false;
}

bool Actor::canKill() {
	return false;
}
bool Actor::isAlive()
{
	return living;
}
void Actor::die()
{
	living = false;
}
bool Actor::canBeKilled()
{
	return false;
}
bool Actor::canPickUp()
{
	return false;
}
bool Actor::canFire()
{
	return false;
}
bool Actor::canExplode()
{
	return false;
}
bool Actor::canDrop()
{
	return false;
}
bool Actor::canHeal()
{
	return false;
}
int Actor::numberInfected()
{
	return numInfected;
}
bool Actor::testInfected()
{
	return isInfected;
}
void Actor::infect()
{
	isInfected = true;
}

int Actor::getFlames()
{
	return numFlames;
}

int Actor::getLandmines()
{
	return numLandmines;
}

int Actor::getVaccines()
{
	return numVaccines;
}

void Actor::giveFlames()
{
	numFlames += 5;
}

void Actor::giveLandmines()
{
	numLandmines += 2;
}

void Actor::giveVaccines()
{
	numVaccines += 1;
}

void Actor::cure()
{
	isInfected = false;
}

void Actor::fireFlame()
{
	numFlames--;
}

StudentWorld * Actor::getWorld()
{
	return myWorld;
}

void Actor::setWorld(StudentWorld * world)
{
	myWorld = world;
}

void Penelope::doSomething() {
	int theKey = -999;
	getWorld()->getKey(theKey);
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
		getWorld()->fire(toX, toY, getDirection());
		break;
	case KEY_PRESS_TAB:
		getWorld()->placeLandmine(toX, toY);
		break;
	case KEY_PRESS_ENTER:
		break;
	default:
		break;
	}
	if ( getWorld()->checkCollision(toX, toY)) {
		}
	else {
		if (moving) {
			moveTo(toX, toY);
		}
	}
}

bool Penelope::canBeKilled()
{
	return true;
}

//All Moving Things

Moving::Moving(int ID, double myX, double myY, Direction dir, int depth) :Actor(ID, SPRITE_WIDTH*myX, SPRITE_HEIGHT * myY, Actor::getWorld(), dir, depth, 1.0)
{

}

bool Moving::canBeKilled()
{
	return true;
}

//Citizens 
Citizen::Citizen(int posX, int posY, StudentWorld* world):Moving(IID_CITIZEN, posX, posY, right, 0)
{
	setWorld(world);
}
void Citizen::doSomething() {
	
}

Zombie::Zombie(int posX, int posY):Moving(IID_ZOMBIE, posX, posY, right, 0) {

}

DumbZombie::DumbZombie(int posX, int posY, StudentWorld* world) : Zombie(posX, posY) {
	setWorld(world);
}

void DumbZombie::doSomething() {

}

SmartZombie::SmartZombie(int posX, int posY, StudentWorld* world) :Zombie(posX, posY) {
	setWorld(world);
}
void SmartZombie::doSomething() {

}
//All Stationary Things
Stationary::Stationary(int ID, double myX, double myY, Direction dir, int depth) :Actor(ID, SPRITE_WIDTH*myX, SPRITE_HEIGHT * myY,Actor::getWorld(), dir, depth, 1.0)
{
}
//Walls
Wall::Wall(int posX, int posY, StudentWorld* world):Stationary(IID_WALL, posX, posY, right, 0)
{
	setWorld(world);
	//doesn't need to do anything else bc it's...a wall
	
}

bool Wall::blocker()
{
	//the only blocker! 
	return true;
}

//Exits

Exit::Exit(int posX, int posY, StudentWorld* world):Stationary(IID_EXIT, posX, posY, right, 1)
{
	setWorld(world);
}

bool Exit::isExit()
{
	return true;
}

//Trap 
Trap::Trap(int posX, int posY,StudentWorld* world) :Stationary(IID_PIT, posX, posY, right, 1)
{
	setWorld(world);
}

void Trap::doSomething()
{
}

bool Trap::canKill() {
	return true;
}

//Flames
Flame::Flame(int posX, int posY,StudentWorld* world) :Stationary(IID_FLAME, posX, posY, right, 0)
{
	setWorld(world);
	numTicksAlive = 0;
}

void Flame::doSomething()
{
	if (numTicksAlive >= 2) {
		die();
	}
	numTicksAlive++;
}
bool Flame::canKill()
{
	return true;
}
//Goodies

Goodie::Goodie(int ID, int posX, int posY) :Stationary(ID, posX, posY, right, 1) {

}

bool Goodie::canPickUp()
{
	return true;
}

bool Goodie::canBeKilled()
{
	return true;
}

GasCan::GasCan(int posX, int posY,StudentWorld* world) : Goodie(IID_GAS_CAN_GOODIE, posX, posY) {
	setWorld(world);
}

void GasCan::doSomething()
{
}

bool GasCan::canFire()
{
	return true;
}

MineGoodie::MineGoodie(int posX, int posY, StudentWorld* world):Goodie(IID_LANDMINE_GOODIE, posX, posY)
{
	setWorld(world);
}
void MineGoodie::doSomething() {

}

bool MineGoodie::canDrop()
{
	return true;
}

VaccineGoodie::VaccineGoodie(int posX, int posY, StudentWorld* world):Goodie(IID_VACCINE_GOODIE, posX, posY)
{
	setWorld(world);
}
void VaccineGoodie::doSomething() {


}

bool VaccineGoodie::canHeal()
{
	return true;
}

Mine::Mine(int x, int y, StudentWorld * world):Stationary(IID_LANDMINE,x,y,right,1)
{
	numTicksAlive = 0;
	setWorld(world);
}

void Mine::doSomething()
{
	numTicksAlive++;
	if (numTicksAlive >= 30) {
		getWorld()->activateLandmine(getX(), getY());
		die();
	}
	
}
