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

bool Actor::fullBlock()
{
	return false;
}

bool Actor::blocksVomit()
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
bool Actor::canInfect()
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
	if (numVaccines > 0) {
		isInfected = false;
		numVaccines--;
		numInfected = 0;
	}
}

void Actor::fireFlame()
{
	numFlames--;
}

void Actor::explode()
{
}

bool Actor::canVomit()
{
	return false;
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

bool Actor::canExit()
{
	return false;
}

void Penelope::doSomething() {
	if (testInfected()) {
		incrementInfect();
	}
	if (numberInfected() > 500) {
		die();
		return;
	}

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
		//std::cout << "firing!" << std::endl;
		getWorld()->fire(toX, toY, getDirection());
		break;
	case KEY_PRESS_TAB:
		getWorld()->placeLandmine(toX, toY);
		break;
	case KEY_PRESS_ENTER:
		cure();
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
	//TODO: check for collisions with zambies!!1
	incrementTicks();
	//if (getWorld()->checkObjectOverlap(this)) {
		//return;
	//}
	if (!isAlive()) {
		//std::cout << "I, a citizen, am dead" << std::endl;
		getWorld()->citizenDie();
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
		//std::cout << "oh no penny does not exist :(" << std::endl;
		return;
	}
	dist_p = (getX() - getPenny()->getX()) * (getX() - getPenny()->getX()) + (getY() - getPenny()->getY()) * (getY() - getPenny()->getY());
	if (dist_p <= 64*64) {
		if (dist_p < 0) {
			return;
		}
		//std::cout << "I can move towards penny!" << std::endl;
		int diffX = getX() - getPenny()->getX();
		int diffY = getY() - getPenny()->getY();
		//if they're on the same "column"
		if (diffX == 0) {
			if (diffY > 0) {
				if (!getWorld()->personMoveFreely(this, getX(), getY() - 2)) {
					return;
				}
				setDirection(down);
				moveTo(getX(), getY() - 2);
				return;
					
				
			}
			else {
				setDirection(up);
				if (!getWorld()->personMoveFreely(this, getX(), getY() + 2)) {
					return;
				}
						moveTo(getX(), getY() + 2);
						return;
					
				
			}
		}
		//if they're on the same "row"
		if (diffY == 0) {
			if (diffX > 0) {
				if (!getWorld()->personMoveFreely(this, getX()-2, getY())) {
					return;
				}
						setDirection(left);
						moveTo(getX() - 2, getY());
						return;
			}
			else {
				if (!getWorld()->personMoveFreely(this, getX()+2, getY())) {
					return;
				}
						setDirection(right);
						moveTo(getX() + 2, getY());
						return;
				
				
			}
		}

		//now, if they aren't on the same row/col
		int randThing = randInt(0, 1);
		if (diffX > 0) {
			//move left
			if (diffY > 0) {
				//move down
				//this is left-down
				if (randThing == 0) {
					//move down
					if (!getWorld()->personMoveFreely(this, getX(), getY() - 2)) {
						return;
					}
					setDirection(down);
					moveTo(getX(), getY() - 2);
					return;
				}
				else {
					//mofe left
					if (!getWorld()->personMoveFreely(this, getX()-2, getY())) {
						return;
					}
						setDirection(left);
						moveTo(getX() - 2, getY());
						return;
				}
			}
			else {
				//move up
				//this is left-up
				if (randThing == 0) {
					//move up
					if (!getWorld()->personMoveFreely(this, getX(), getY()+2)) {
						return;
					}
						setDirection(up);
						moveTo(getX(), getY() + 2);
						return;
					//}
				}
				else {
					//TODO: get rid of these checks???? 
					//mofe left
					if (!getWorld()->personMoveFreely(this, getX()-2, getY())) {
						return;
					}
						setDirection(left);
						moveTo(getX() - 2, getY());
						return;
					//}
				}
			}

		}
		else {
			//move right
			if (diffY > 0) {
				if (randThing == 0) {
					//move down
					if (!getWorld()->personMoveFreely(this, getX(), getY() - 2)) {
						return;
					}
							setDirection(down);
							moveTo(getX(), getY() - 2);
							return;
				}
				else {
					//move right
					if (!getWorld()->personMoveFreely(this, getX()+2, getY())) {
						return;
					}
						setDirection(right);

						moveTo(getX() + 2, getY());
						return;

				}
			}
			else {
				//move up
				//this is up left
				if (randThing == 0) {
					//move up
					if (!getWorld()->personMoveFreely(this, getX(), getY() +2)) {
						return;
					}
						moveTo(getX(), getY() + 2);
						return;
					//}
				}
				else {
					//mofe right
					if (!getWorld()->personMoveFreely(this, getX()+2, getY())) {
						return;
					}
					setDirection(right);
						moveTo(getX() + 2, getY());
						return;
				}
			}
		}
	}
}

bool Citizen::canExit()
{
	return true;
}

void Citizen::die()
{
	getWorld()->citizenDie();
	Actor::die();
}

bool Citizen::fullBlock()
{
	return true;
}

Zombie::Zombie(int posX, int posY):Moving(IID_ZOMBIE, posX, posY, right, 0) {

}



DumbZombie::DumbZombie(int posX, int posY, StudentWorld* world) : Zombie(posX, posY) {
	setWorld(world);
	movementPlan = 0;
}

void DumbZombie::doSomething() {
	incrementTicks();
	if (!isAlive()) {
		return;
	}
	if (numTicksHere() % 2 == 0) {
		return;
	}
	//determine coordinates of the (possible) vomit)
	int vomX = getX();
	int vomY = getY();
	Direction myDir = getDirection();
	switch (myDir) {
		case right:
			vomX += SPRITE_WIDTH;
			break;
		case left:
			vomX -= SPRITE_WIDTH;
			break;
		case down:
			vomY -= SPRITE_HEIGHT;
			break;
		case up:
			vomY += SPRITE_HEIGHT;
			break;
		default:
			//std::cerr << "error!" << std::endl;
			break;
	}
	//check for if there's a person in front  
	if (getWorld()->canVomitHere(vomX, vomY, this) ) {
		int chanceVomit = randInt(1, 3);
		//if it vomits, then immediately return. otherwise, move n shit 
		if (chanceVomit == 1) {
			//getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
			getWorld()->vomitHere(vomX, vomY);
			return;
		}
	}
	int toX = getX();
	int toY = getY();
	if (movementPlan <= 0) {
		//std::cout << "plan is 0!" << std::endl;
		movementPlan = randInt(3, 10);
		int dirToPick = randInt(1, 4);
		switch (dirToPick) {
		case 1:
			setDirection(right);
			toX += 1;
			break;
		case 2:
			setDirection(left);
			toX -= 1;
			break;
		case 3:
			setDirection(up);
			toY += 1;
			break;
		case 4:
			setDirection(down);
			toY += 1;
			break;
		default:
			break;
		}
	}
	else {
		Direction myDirection = getDirection();
		switch (myDirection) {
		case left:
			toX -= 1;
			break;
		case right:
			toX += 1;
			break;
		case up:
			toY += 1;
			break;
		case down:
			toY -= 1;
			break;
		}
	}
	if (getWorld()->personMoveFreely(this, toX, toY) && !getWorld()->checkObjectOverlap(this)) {
		//moveTo(toX, toY);
		movementPlan--;
	}
	else {
		//std::cout << "yo im be blocced" << std::endl;
		//moveTo(getX(), getY());
		movementPlan = 0;
		return;
	}
}

bool DumbZombie::blocksVomit()
{
	//std::cout << "blocking vomit from a dumbo" << std::endl;
	return true;
}

bool DumbZombie::fullBlock()
{
	return true;
}

bool DumbZombie::canExit()
{
	return false;
}

void DumbZombie::die()
{
	getWorld()->increaseScore(1000);
	getWorld()->playSound(SOUND_ZOMBIE_DIE);
	Actor::die();

}

bool DumbZombie::canVomit()
{
	return true;
}

SmartZombie::SmartZombie(int posX, int posY, StudentWorld* world) :Zombie(posX, posY) {
	setWorld(world);
	movementPlan = 0;
}
void SmartZombie::doSomething() {
	incrementTicks();
	if (!isAlive()) {
		return;
	}
	if (numTicksHere() % 2 == 0) {
		return;
	}
	//find out where to vomit!
	int vomX = getX();
	int vomY = getY();
	Direction myDir = getDirection();
	switch (myDir) {
	case right:
		vomX += SPRITE_WIDTH;
		break;
	case left:
		vomX -= SPRITE_WIDTH;
		break;
	case down:
		vomY -= SPRITE_HEIGHT;
		break;
	case up:
		vomY += SPRITE_HEIGHT;
		break;
	default:
		break;
	}
	//check for if there's a person in front  
	if (getWorld()->canVomitHere(vomX, vomY, this)) {
		int chanceVomit = randInt(1, 3);
		//if it vomits, then immediately return. otherwise, move n shit 
		if (chanceVomit == 1) {
			//getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
			getWorld()->vomitHere(vomX, vomY);
			return;
		}
	}
	if (movementPlan <= 0) {
		//std::cout << "a new movement plan" << std::endl;
		movementPlan = randInt(3, 10);
		if (!getWorld()->faceThisWay(this, myDir)) {
			std::cout << "randomizing!" << std::endl;
			int randBoi = randInt(1, 4);
			switch (randBoi) {
			case 1:
				setDirection(right);
				std::cout << "set dir" << std::endl;
				break;
			case 2:
				setDirection(left);
				std::cout << "set dir" << std::endl;

				break;
			case 3:
				setDirection(up);
				std::cout << "set dir" << std::endl;

				break;
			case 4:
				setDirection(down);
				std::cout << "set dir" << std::endl;

				break;
			}
		}
		else {
			setDirection(myDir);
		}
	}
	//movementPlan--;
	myDir = getDirection();
	//std::cout << myDir <<std::endl;
	int toX = getX();
	int toY = getY();
	switch (myDir) {
		case left:
			toX -= 1;
			break;
		case right:
			toX += 1;
			break;
		case up:
			toY += 1;
			break;
		case down:
			toY -= 1;
			break;
		default:
			break;
	}
	if (getWorld()->personMoveFreely(this, toX, toY)) {
		moveTo(toX, toY);
		movementPlan--;
	}
	else {
		movementPlan = 0;
	}
}
bool SmartZombie::blocksVomit()
{
	return true;
}
bool SmartZombie::fullBlock()
{
	return true;
}
bool SmartZombie::canExit()
{
	return false;
}
void SmartZombie::die()
{
	getWorld()->increaseScore(1000);
	getWorld()->playSound(SOUND_ZOMBIE_DIE);

	Actor::die();
}
bool SmartZombie::canVomit()
{
	return true;
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

bool Wall::fullBlock()
{
	return true;
}

bool Wall::blocksVomit()
{
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

bool Trap::blocker()
{
	return true;
}

bool Trap::isKillable()
{
	return true;
}

bool Trap::blocksVomit()
{
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

bool Goodie::blocksVomit()
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
	getWorld()->checkObjectOverlap(this);

}

void Mine::explode()
{
	if (numTicksHere() >= 30) {
		getWorld()->activateLandmine(getX(), getY());
	}
}

bool Mine::canExplode()
{
	return true;
}

void Mine::die()
{
	explode();
	Actor::die();
}

Vomit::Vomit(int posX, int posY, StudentWorld * world) :Stationary(IID_VOMIT, posX, posY, right, 1)
{
	setWorld(world);
	ticksAlive = 0;
}

bool Vomit::canInfect()
{
	return true;
}

void Vomit::doSomething()
{
	ticksAlive++;
	if(ticksAlive>2) 
		die();
}
