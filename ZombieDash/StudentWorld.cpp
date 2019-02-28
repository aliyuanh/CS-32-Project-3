#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;
//TODO: sounds
//TODO: scores 
//TODO: format game text - fix 
//TODO: smart zombies 
//TODO: citizens run away from zombies 
GameWorld* createStudentWorld(string assetPath)
{
	//this is where everything is made 

	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::~StudentWorld()
{
	cleanUp();
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
	numLevel = 4;
	score = 0;
	numInfected = 0;
	numFlames = 0;
	numLandmines = 0;
	numVaccines = 0;
	numCitizensToSave = 0;
	myStatus = 0;
	penny = nullptr;
}

int StudentWorld::init()
{
	numInfected = 0;
	numFlames = 0;
	numLandmines = 0;
	numVaccines = 0;
	numCitizensToSave = 0;
	//from page 22 of spec 
	Level lev(assetPath());
	string levelFile = "level0"+to_string(numLevel)+".txt";
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found) {
		cerr << "cannot find " << levelFile << " data file"<<endl;
		myStatus = 1;
		cerr << "returning a level error" << endl;
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_fail_bad_format) {
		cerr << "level was improperly formatted" << endl;
		myStatus = 2;
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (result == Level::load_success) {
		//cerr << "Successfully loaded level" << endl;
		for (int i = 0; i < LEVEL_WIDTH; i++) {
			for (int j = 0; j < LEVEL_HEIGHT; j++) {
				Level::MazeEntry ge = lev.getContentsOf(i, j);
				//cout << "got the contents of the level!" << endl;
				Actor* thing;
				switch (ge) {
				case Level::empty:
					break;
				case Level::gas_can_goodie:
					thing = new GasCan(i, j,this);
					entities.push_back(thing);
					break;
				case Level::landmine_goodie:
					thing = new MineGoodie(i, j,this);
					entities.push_back(thing);
					break;
				case Level::vaccine_goodie:
					thing = new VaccineGoodie(i, j,this);
					entities.push_back(thing);
					break;
				case Level::smart_zombie:
					thing = new SmartZombie(i, j,this);
					entities.push_back(thing);
					//cerr << "Made a smart zombie!" << endl;
					break;
				case Level::dumb_zombie:
					thing = new DumbZombie(i, j,this);
					entities.push_back(thing);
					//cerr << "Made a dumb zombie!" << endl;
					break;
				case Level::player:
					//cerr << "made penny" << endl;
					penny = new Penelope(i, j,this);
					break;
				case Level::exit:
					//cerr << "made an exit" << endl;
					thing = new Exit(i, j,this);
					entities.push_back(thing);
					break;
				case Level::wall:
					//cerr << "made a wall" << endl;
					thing = new Wall(i, j,this);
					entities.push_back(thing);
					break;
				case Level::pit:
					thing = new Trap(i, j,this);
					entities.push_back(thing);
					//cerr << "Made a pit!" << endl;
					break;
				case Level::citizen:
					thing = new Citizen(i, j,this, penny);
					entities.push_back(thing);
					//cerr << "Made a citizen!" << endl;
					numCitizensToSave++;
					break;
				default:
					break;
				}
			}

		}
	}
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end(); it++) {
		(*it)->setPenny(penny);
	}
	//TODO: remove AFTER testing 
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::moveEnts() {
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end(); it++) {
		(*it)->doSomething();
	}
}
void StudentWorld::nextLevel()
{
	numLevel++;
	cleanUp();
	init();
	//entities.clear();
}
int StudentWorld::move()
{
	if (myStatus == 1) {
		return GWSTATUS_LEVEL_ERROR;
	}
	else if (myStatus == 2) {
		return GWSTATUS_LEVEL_ERROR;
	}
	//cerr << "moving peeps!" << endl;
	if (penny->isAlive() == false) {
		decLives();
		cerr << "penny has died! restart >:(" << endl;
		return GWSTATUS_PLAYER_DIED;
	}
	//check if order is correct later 

	//check if things die
	checkTheDead();
	moveEnts();
	//pads strings 
	ostringstream oss;
	oss.fill('0');
	oss << setw(6) << score;
	oss.fill('0');
	string padded = oss.str();
	if (penny != nullptr) {
		penny->doSomething();
		if (penny != nullptr) {
			setGameStatText("Score: " + padded + "  Level: " + to_string(numLevel) + "  Lives: " + to_string(getLives())
				+ "  Vacc: " + to_string(penny->getVaccines()) + "  Flames: " + to_string(penny->getFlames())
				+ "  Mines: " + to_string(penny->getLandmines()) + "  Infected: " + to_string(penny->numberInfected()));
		}
		
	}
	
	//decLives();
	return GWSTATUS_CONTINUE_GAME;
   // return GWSTATUS_PLAYER_DIED;
}
void StudentWorld::placeLandmine(int x, int y) {
	Actor* thing = new Mine(0, 0, this);
	//made a new mine
	if (checkExit(x, y)) {
		cout << "oof is on a place that isn't killable" << endl;
		delete thing;
	}
	else {
		cout << "made a new mine!" << endl;

		thing->moveTo(x, y);
		entities.push_back(thing);
	}
}
void StudentWorld::activateLandmine(int x, int y)
{
	int startX = x - SPRITE_WIDTH;
	int startY = y - SPRITE_HEIGHT;
	Actor* thing;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			thing = new Flame(0, 0, this);
			thing->moveTo(startX + i * SPRITE_WIDTH, startY + j * SPRITE_HEIGHT);
			if (!checkKillable(startX + i * SPRITE_WIDTH + 4, startY + j * SPRITE_HEIGHT + 4)) {
				delete thing;
			}
			else {
				entities.push_back(thing);
			}
		}

	}
	thing = new Trap(0, 0, this);
	thing->moveTo(x, y);
	entities.push_back(thing);


}
void StudentWorld::fire(int x, int y, Direction dir) {
	if (penny->getFlames() <= 0) {
		cout << "no flames left!" << endl;
		return;
	}
	Actor* thing;
	switch (dir) {
	case GraphObject::left:
		//make da flames 
		for (int i = 0; i < 3; i++) {
			thing = new Flame(0 , 0,this);
			thing->moveTo(x - ((i+1)*SPRITE_WIDTH), y);
			if (!checkKillable(thing->getX()+4, thing->getY())) {
				std::cout << "flames are blocked!" << endl;
				delete thing;
				break;
			}
			else {
				thing->setDirection(GraphObject::left);
				entities.push_back(thing);
			}
		}
		break;

	case GraphObject::right:
		for (int i = 0; i < 3; i++) {
			thing = new Flame(0, 0,this);
			thing->moveTo(x + ((i + 1) * SPRITE_WIDTH), y);
			if (!checkKillable(thing->getX(), thing->getY())) {
				std::cout << "flames are blocked!" << endl;
				delete thing;
				break;
			}
			else {
				thing->setDirection(GraphObject::right);
				entities.push_back(thing);
			}
		}
		break;

	case GraphObject::up:
		for (int i = 0; i < 3; i++) {
			thing = new Flame(0, 0,this);
			thing->moveTo(x, y + (i+1)*SPRITE_HEIGHT);
			if (!checkKillable(thing->getX(), thing->getY())) {
				std::cout << "flames are blocked!" << endl;
				delete thing;
				break;
			}
			else {
				thing->setDirection(GraphObject::up);
				entities.push_back(thing);
			}
		}
		break;

	case GraphObject::down:
		for (int i = 0; i < 3; i++) {
			thing = new Flame(0, 0,this);
			thing->moveTo(x, y - (i + 1)*SPRITE_HEIGHT);
			if (!checkKillable(thing->getX(), thing->getY())) {
				std::cout << "flames are blocked!" << endl;
				delete thing;
				break;
			}
			else {
				thing->setDirection(GraphObject::down);
				entities.push_back(thing);
			}
		}
		break; 
	default: 
		break;
	}
	penny->fireFlame();
}

bool StudentWorld::checkKillable(int x, int y) {
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end(); it++) {
		//check every entitity against those 
		int diffX = abs(x - (*it)->getX());
		int diffY = abs(y - (*it)->getY());
		if (diffX * diffX + diffY * diffY <= 100) {
			if ((*it)->canBeKilled() && (*it)->blocker() || (*it)->canPickUp()) {
				//if it can be killed and it's a blocker (ie a citizen or a zombie), make the thing
				cout << "it can be killed and it's a blocker or it can be picked up!" << endl;
				return true;
			}
			else if ((*it)->fullBlock()) {
				//if it blocks stuff, don't make the thing
				cout << "it fully blocks things!" << endl;
				return false;
			}
		}

	}

	int pennyX = penny->getX() - x;
	int pennyY = penny->getY() - y;
	if (pennyX*pennyX + pennyY * pennyY < 256) {
		//return false;
	}
	return true;

}

bool StudentWorld::checkExit(int x, int y) {
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end(); it++) {
		//check every entitity against those 
		int diffX = abs(x - (*it)->getX()) - 4;
		int diffY = abs(y - (*it)->getY()) - 4;
		if (diffX * diffX + diffY * diffY <= 100) {
			if ((*it)->isExit()) {
				cout << "tryna make something on an exit :(" << endl;
				//if it is an exit, say it is 
				return true;
			}
		}

	}
	return false;

}

void StudentWorld::turnCitizenToZombie(int x, int y)
{
	playSound(SOUND_ZOMBIE_BORN);
	score -= 1000;
	int numRand = randInt(1,10);
	Actor* thing;
	if (numRand >= 1 && numRand <= 3) {
		thing = new SmartZombie(0, 0,this);
	}
	else {
		thing = new DumbZombie(0, 0, this);
	}
	thing->moveTo(x, y);

}

bool StudentWorld::checkObjectOverlap(Actor * p)
{
	//if this returns true, kill p
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end(); it++) {
		if ((*it) == p) {
			continue;
		}
		int diffX = abs(p->getX() - (*it)->getX()) - 4;
		int diffY = abs(p->getY() - (*it)->getY()) - 4;
		if (diffX * diffX + diffY * diffY <= 144) {
			if ((*it)->canKill()) {
				p->die();
				return true;
			}
			if ((*it)->blocker()) {
				return true;
			}
			if ((*it)->isExit() && p->canExit()) {
				p->die();
				playSound(SOUND_CITIZEN_SAVED);
				return true;
			}
		}

	}

	return false;
}

bool StudentWorld::personMoveFreely(Actor * p, int x, int y)
{
	//check walls and penny 
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end(); it++) {
		if ((*it) == p) {
			continue;
		}
		int diffX = abs(x - (*it)->getX()); //positive means left
		int diffY = abs(y - (*it)->getY()); //positive means up 
		//check corners!
		//upper left corner
		if ((*it)->fullBlock()) {
			//accounts for corners 
			if (diffX <= 15 && diffY <= 15 && diffX > 0 && diffY >0) {
				//cout << "diffX: "<<diffX << endl;
				//cout << "diffY: " << diffY << endl;
				return false;
			}
		}
		//lower left corner
		//lower right corner 


		if (diffX * diffX + diffY * diffY < 256 && (diffX > 0 ||diffY >0)) {
			if ((*it)->canExplode()) {
				(*it)->explode();
			}
			if ((*it)->isExit() && !p->canVomit()) {
				p->die();
				score += 1000;
				return false;
			}
			if ((*it)->canKill()) {
				p->die();
				return false;
			}
			if ((*it)->fullBlock() || (*it)->canVomit()) {

				//cout << "blocky boi" << p->numTicksHere()<< endl;
				return false;
			}
			if ((*it)->canInfect()) {
				p->infect();
			}
			
		}

	}
	int pennX = abs(x - (penny)->getX()) - 4;
	int pennY = abs(y - (penny)->getY()) - 4;
	if (pennX * pennX + pennY * pennY <= 144) {
		return false;
	}

	return true;
}

void StudentWorld::citizenDie()
{
	numCitizensToSave--;
}

bool StudentWorld::canVomitHere(int x, int y, Actor* p )
{
	//check for if it's a wall or trap -> return false 
	bool foundThing = false;
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end(); it++) {
		if ((*it)  == p) {
			continue;
		}
		int diffX = (*it)->getX() - p->getX();
		int diffY = (*it)->getY() - p->getY();
		if (diffX*diffX + diffY * diffY <= 16*16) {
			if ((*it)->blocksVomit() || (*it)->canKill()) {
				//cout << "yo don't vomit here" << endl;
				return false;
			}
			//cout << "found a boi to vomit on!" << endl;
			//cout << diffX << "," << diffY << endl;
			foundThing = true;
		}
	}
	int pennX = penny->getX() - x;
	int pennY = penny->getY() - y;
	if (pennX * pennX + pennY * pennY < 256) {
		return true;
	}

	if (foundThing) {
		return true;
	}
	return false;
}

void StudentWorld::vomitHere(int x, int y)
{
	//make vomit here
	Actor* thing = new Vomit(0, 0, this);
	thing->moveTo(x, y);
	if (checkObjectOverlap(thing)) {
		delete thing;
	}
	else {
		entities.push_back(thing);
		playSound(SOUND_ZOMBIE_VOMIT);

	}

}

void StudentWorld::increaseScore(int num)
{
	score += num;
}

bool StudentWorld::faceThisWay(Actor * p, Direction&dir)
{
	int smallestDistance = 10000000;
	int diffX = 100000;
	int diffY = 100000;
	bool hasBeenChanged = false;
	Direction toSet = GraphObject::right;
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end(); it++) {

		if ((*it) == p) {
			continue;
		}
		if ((*it)->canVomit()) {
			toSet = p->getDirection();
			continue;
		}
		if ((*it)->fullBlock()) {
			continue;
		}
		if ((*it)->blocker()) {
			continue;
		}
		if ((*it)->canPickUp()) {
			continue;
		}

		diffX = p->getX() - (*it)->getY(); //positive means face left
		diffY = p->getY() - (*it)->getX();//positive means face down
		if (diffX * diffX + diffY * diffY < 80 * 80 && diffX * diffX + diffY * diffY < smallestDistance) {
			smallestDistance = diffX * diffX + diffY * diffY;
			if (diffX == 0) {//same row 
				if (diffY > 0) {
					toSet = GraphObject::down;
					dir = GraphObject::down;
					//cout << "changed to true on line " << 537 << endl;
					//hasBeenChanged = true;
				}
				else if(diffY < 0) {
					//hasBeenChanged = true;
					//cout << "changed to true on line " << 541 << endl;

					toSet = GraphObject::up;
				}
			}
			else if (diffY == 0) {//same column 
				if (diffX > 0) {
					toSet = GraphObject::left;
					//cout << "changed to true on line " << 550 << endl;

					//hasBeenChanged = true;

				}
				else if(diffX<0){
					toSet = GraphObject::right;
					cout << "changed to true on line " << 557 << endl;

					//hasBeenChanged = true;

				}
			}
			else {//not same col or row 
				if (abs(diffX) > abs(diffY)) {//face horizontally 
					if (diffX > 0) {
						toSet = GraphObject::left;
						hasBeenChanged = true;
						cout << "changed to true on line " << 567 << endl;


					}
					else if(diffX<0) {
						toSet = GraphObject::right;
						cout << "changed to true on line " << 575 << endl;
						hasBeenChanged = true;

					}
				}
				else if (abs(diffX) < abs(diffY)) {//face vertically 
					if (diffY > 0) {
						toSet = GraphObject::down;
						//cout << "changed to true on line " << 583 << endl;

						//hasBeenChanged = true;

					}
					else if(diffY<0){
						toSet = GraphObject::up;
						//cout << "changed to true on line " << 589 << endl;

						//hasBeenChanged = true;

					}
				}
			}
		}
		//otherwise, after also checking penny, set it to a random dir

	}
	int pennX = p->getX() - penny->getX();
	int pennY = p->getY() - penny->getY();
	if (pennX * pennX + pennY * pennY < 80 * 80 && pennX * pennX + pennY * pennY < smallestDistance) {
		cout << "penny be close!" << endl;
		if (pennX == 0) {//same row 
			if (pennY > 0) {
				toSet = GraphObject::down;
				cout << "changed to true on line " << 606 << endl;

				hasBeenChanged = true;

			}
			else if(pennY <0){
				toSet = GraphObject::up;
				//cout << "changed to true on line " << 613 << endl;

				//hasBeenChanged = true;

			}
		}
		else if (pennY == 0) {//same column 
			if (pennX > 0) {
				toSet = GraphObject::left;
				cout << "changed to true on line " << 621 << endl;

				hasBeenChanged = true;

			}
			else if(pennX < 0) {
				toSet = GraphObject::right;
				cout << "changed to true on line " << 631 << endl;

				hasBeenChanged = true;

			}
		}
		else {//not same col or row 
			if (abs(pennX) > abs(pennY)) {//face horizontally 
				if (pennX > 0) {
					toSet = GraphObject::left;
					hasBeenChanged = true;
					cout << "changed to true on line " << 640 << endl;


				}
				else if (pennX <0) {
					toSet = GraphObject::right;
					hasBeenChanged = true;
					cout << "changed to true on line " << 646 << endl;


				}
			}
			else {//face vertically 
				if (pennY > 0) {
					toSet = GraphObject::down;
					hasBeenChanged = true;
					cout << "changed to true on line " << 656 << endl;


				}
				else if(pennY < 0){
					toSet = GraphObject::up;
					//cout << "changed to true on line " << 662 << endl;

					//hasBeenChanged = true;

				}
			}
		}
		if (pennX == 0 && pennY == 0) {
			cout << "same spot" << endl;
			hasBeenChanged = false;
			return true;
		}
		p->setDirection(dir);
		dir = toSet;
		return true;
	}
	dir = toSet;
	if (!hasBeenChanged) {
		cout << "has not been changed" << endl;
		return false;
	}
	else {
		cout << "yeee" << endl;
		return true;
	}
	return true;
}

bool StudentWorld::checkCollision(int x, int y) {
	if (entities.size() == 0) {
		return true;
	}
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end(); it++) {
	//check every entitity against those 
		//later, check to make sure it's one of Zombie, Wall, or Citizen 
		if (abs(x - (*it)->getX()) * abs(x - (*it)->getX()) + abs(y - (*it)->getY())*abs(y - (*it)->getY()) <= 144 && (*it)->canKill()) {
			penny->die();
			cout << "penny died" << endl;
			return true;
		}
		int diffX = abs(x - (*it)->getX()) - 4;
		int diffY = abs( y - (*it)->getY()) - 4;
		if ((*it)->getY()-y <= 12 &&(*it)->getY()-y >= 0 && x-(*it)->getX() <= 12 && x-(*it)->getX() >= 0 && (*it)->blocker()) {
			//upper left corner checker
			return true;
		}
		if ((*it)->getY() - y <= 12 && (*it)->getY() - y >= 0 && (*it)->getX()-x <= 12 && (*it)->getX()-x >= 0 && (*it)->blocker()) {
			//upper right corner checker
			return true;
		}
		if (diffX * diffX + diffY*diffY < 144) {
			if ((*it)->canExplode()) {
				(*it)->explode();
			}
			//cout << "collision happening!" << endl;
			//cerr << "collided w something" << endl;
			if ((*it)->canKill()) {
				//decLives();
				penny->die();
				return true;
			}
			//vomit!
			if ((*it)->canInfect()) {
				//cout << "INFECTING PENNYYYY" << endl;
				penny->infect();
			}
			//goodies!
			if ((*it)->canPickUp()) {
				//cerr << "it's a goodie boi " << endl;
				if ((*it)->canFire()) {
					penny->giveFlames();
				}
				if ((*it)->canHeal()) {
					penny->giveVaccines();
				}
				if ((*it)->canDrop()) {
					penny->giveLandmines();
				}
				playSound(SOUND_GOT_GOODIE);
				score += 50;
				(*it)->die();
			}
			if ((*it)->blocker()) {
				return true;
			}
			if ((*it)->isExit() && numCitizensToSave == 0) {
				nextLevel();
				return true;
			}
			
		}
		
	}
	return false; //has not collided with anything :) 

}//if it intersects w any wall, cit, or zomb, then return false 
//need to implement later :( 
bool StudentWorld::hitPenny(int x, int y) {
	int diffX = abs(x - penny->getX())-8;
	int diffY = abs(y - penny->getY())-2;
	if (y-penny->getY()<= 16 && y-penny->getY()>= 0 && penny->getX()-x <= 16 && penny->getX()-x >= 0) {
		//upper left corner checker
		//return true;
	}
	if (y-penny->getY()<= 16 && y-penny->getY()>= 0 && penny->getX() - x <= 16 && penny->getX() - x >= 0 ) {
		//upper right corner checker
		//return true;
	}
	if (diffX * diffX + diffY * diffY <= 100) {
		return true;
	}
	return false;
}
void StudentWorld::checkTheDead() {
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end();) {
		//delete if things die :) 
		if ((*it)->isAlive() == false) {
			delete (*it);
			entities.erase(it);
			it = entities.begin();
		}
		else {
			it++;
		}
	}
}

void StudentWorld::cleanUp()
{
	delete penny;
	penny = nullptr;
	std::list<Actor*>::iterator it1;
	it1 = entities.begin();
	while (it1 != entities.end()) {
		std::list<Actor*>::iterator it2;
		it2 = it1;
		//cout << *it2 << endl;
		delete *it2;
		entities.erase(it2);
		it1 = entities.begin();
	}
	if (entities.size() == 0) {
		//cout << "all the bois were deleted!" << endl;
	}
	
}
