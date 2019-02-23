#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <cmath>
#include <string>
using namespace std;

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
	numLevel = 3;
	score = 0;
	numInfected = 0;
	numFlames = 0;
	numLandmines = 0;
	numVaccines = 0;
	numCitizensToSave = 0;
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
		//cerr << "cannot find " << levelFile << " data file"<<endl;
	}
	else if (result == Level::load_fail_bad_format) {
		//cerr << "level was improperly formatted" << endl;
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
					thing = new Citizen(i, j,this);
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


    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::moveEnts() {
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end(); it++) {
		if (!((*it)->blocker())) {
			(*it)->doSomething();
			//cout << "moving an actor in moveEnts" << endl;
		}
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
	//cerr << "moving peeps!" << endl;
	if (penny->isAlive() == false) {
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	//check if order is correct later 

	//check if things die
	checkTheDead();

	moveEnts();
	penny->doSomething();
	setGameStatText("Score:  "+ to_string(score) + "  Level:  " + to_string(numLevel)
	 + "  Vaccines:  " + to_string(penny->getVaccines()) + "  Flames:  " + to_string(penny->getFlames())
	 + "  Mines:  " + to_string(penny->getLandmines()) + "  Infected:  " + to_string(penny->numberInfected()));
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


}
void StudentWorld::fire(int x, int y, Direction dir) {
	if (penny->getFlames() <= 0) {
		
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
				delete thing;
				break;
			}
			else {
				entities.push_back(thing);
			}
		}
		break;

	case GraphObject::right:
		for (int i = 0; i < 3; i++) {
			thing = new Flame(0, 0,this);
			thing->moveTo(x + ((i + 1) * SPRITE_WIDTH), y);
			if (!checkKillable(thing->getX(), thing->getY())) {
				delete thing;
				break;
			}
			else {
				entities.push_back(thing);
			}
		}
		break;

	case GraphObject::up:
		for (int i = 0; i < 3; i++) {
			thing = new Flame(0, 0,this);
			thing->moveTo(x, y + (i+1)*SPRITE_HEIGHT);
			if (!checkKillable(thing->getX(), thing->getY())) {
				delete thing;
				break;
			}
			else {
				entities.push_back(thing);
			}
		}
		break;

	case GraphObject::down:
		for (int i = 0; i < 3; i++) {
			thing = new Flame(0, 0,this);
			thing->moveTo(x, y - (i + 1)*SPRITE_HEIGHT);
			if (!checkKillable(thing->getX(), thing->getY())) {
				delete thing;
				break;
			}
			else {
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
				return true;
			}
			else if ((*it)->blocker()) {
				//if it blocks stuff, don't make the thing
				return false;
			}
		}

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

bool StudentWorld::checkCollision(int x, int y) {
	if (entities.size() == 0) {
		return true;
	}
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end(); it++) {
	//check every entitity against those 
		//later, check to make sure it's one of Zombie, Wall, or Citizen 
		if (abs(x - (*it)->getX()) * abs(x - (*it)->getX()) + abs(y - (*it)->getY())*abs(y - (*it)->getY()) <= 100 && (*it)->canKill()) {
			penny->die();
			cout << "penny died to flames" << endl;
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
		if (diffX * diffX + diffY*diffY <= 100) {
			//cout << "collision happening!" << endl;
			//cerr << "collided w something" << endl;
			if ((*it)->canKill()) {
				//decLives();
				penny->die();
				return true;
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
				//cout << "yay! onto the next level!" << endl;
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
