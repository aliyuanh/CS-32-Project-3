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
	numTicks = 0;
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
void Actor::incrementInfect()
{
	numInfected++;
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

void Actor::explode()
{
}

int Actor::numTicksHere()
{
	return numTicks;
}

void Actor::incrementTicks()
{
	numTicks++;
}

StudentWorld * Actor::getWorld()
{
	return myWorld;
}

void Actor::setWorld(StudentWorld * world)
{
	myWorld = world;
}

Actor * Actor::getPenny()
{
	return myPenny;
}

void Actor::setPenny(Actor * p)
{
	myPenny = p;
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
Citizen::Citizen(int posX, int posY, StudentWorld* world, Actor* penny):Moving(IID_CITIZEN, posX, posY, right, 0)
{
	setWorld(world);
	dist_p = 10000;
	dist_z = 0;
	setPenny(penny);
}
void Citizen::doSomething() {
	incrementTicks();
	if (!isAlive()) {
		std::cout << "I, a citizen, am dead" << std::endl;
		return;
	}
	if (testInfected()) {
		incrementInfect();
	}
	if (numberInfected() >= 500) {
		die();
		getWorld()->turnCitizenToZombie(getX(), getY());
		return;
	}
	if (numTicksHere() % 2 == 0) {
		return;
	}
	if (pene == nullptr) {
		std::cout << "oh no penny does not exist :(" << std::endl;
		return;
	}
	//std::cout << getPenny()->getX() << std::endl;
	//good enough for now... kinda buggy, but tbh whatever 
	dist_p = (getX() - getPenny()->getX()) * (getX() - getPenny()->getX()) + (getY() - getPenny()->getY()) * (getY() - getPenny()->getY()) - 256;
	//dist_p = (getX() - pene->getX())*(getX() - pene->getX()) + (getY() - pene->getY())*(getY() - pene->getY());
	//std::cout << "my distance to penny is " << dist_p<<std::endl;

	if (dist_p <= 180) {
		if (dist_p < 0) {
			return;
		}
		//std::cout << "I can move towards penny!" << std::endl;
		int diffX = getX() - getPenny()->getX();
		int diffY = getY() - getPenny()->getY();
		//if they're on the same "column"
		if (diffX == 0) {
			if (diffY > 0) {
				setDirection(down);
				if (!getWorld()->checkCollision(getX(), getY())-2) {
					if (getY()-getPenny()->getY() >=18) {
						moveTo(getX(), getY() - 2);
					}
				}
			}
			else {
				setDirection(up);
				if (!getWorld()->checkCollision(getX(), getY()) + 2) {
					if (getPenny()->getY() - getY() >= 18) {
						moveTo(getX(), getY() + 2);
					}
				}
			}
		}
		//if they're on the same "row"
		if (diffY == 0) {
			if (diffX > 0) {
				setDirection(left);
				if (getPenny()->getX() - getX()<= -18) {
					moveTo(getX() - 2, getY());
				}
			}
			else {
				setDirection(right);
				if (getX() - getPenny()->getX() <= -18) {
					moveTo(getX() + 2, getY());
				}
			}
		}

		//now, if they aren't on the same row/col

	}
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
	setWorld(world);
}

void Mine::doSomething()
{
	incrementTicks();
	//if (numTicksAlive >= 30) {
		//getWorld()->activateLandmine(getX(), getY());
		//die();
	//}
	
}

void Mine::explode()
{
	if (numTicksHere() >= 30) {
		getWorld()->activateLandmine(getX(), getY());
		die();
	}
}

bool Mine::canExplode()
{
	return true;
}
