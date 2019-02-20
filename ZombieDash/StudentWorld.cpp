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
					thing = new GasCan(i, j);
					entities.push_back(thing);
					break;
				case Level::landmine_goodie:
					thing = new MineGoodie(i, j);
					entities.push_back(thing);
					break;
				case Level::vaccine_goodie:
					thing = new VaccineGoodie(i, j);
					entities.push_back(thing);
					break;
				case Level::smart_zombie:
					thing = new SmartZombie(i, j);
					entities.push_back(thing);
					//cerr << "Made a smart zombie!" << endl;
					break;
				case Level::dumb_zombie:
					thing = new DumbZombie(i, j);
					entities.push_back(thing);
					//cerr << "Made a dumb zombie!" << endl;
					break;
				case Level::player:
					//cerr << "made penny" << endl;
					penny = new Penelope(i, j, this);
					break;
				case Level::exit:
					//cerr << "made an exit" << endl;
					thing = new Exit(i, j);
					entities.push_back(thing);
					break;
				case Level::wall:
					//cerr << "made a wall" << endl;
					thing = new Wall(i, j);
					entities.push_back(thing);
					break;
				case Level::pit:
					thing = new Trap(i, j);
					entities.push_back(thing);
					//cerr << "Made a pit!" << endl;
					break;
				case Level::citizen:
					thing = new Citizen(i, j);
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
	if (getLives() == 0) {
		return GWSTATUS_PLAYER_DIED;
	}
	//check if order is correct later 
	moveEnts();
	penny->doSomething();
	setGameStatText("Score:  "+ to_string(score) + "  Level:  " + to_string(numLevel)
	 + "  Vaccines:  " + to_string(numVaccines) + "  Flames:  " + to_string(numFlames)
	 + "  Mines:  " + to_string(numLandmines) + "  Infected:  " + to_string(numInfected));
	//decLives();
	return GWSTATUS_CONTINUE_GAME;
   // return GWSTATUS_PLAYER_DIED;
}
void StudentWorld::placeLandmine(int x, int y) {

}

bool StudentWorld::checkCollision(int x, int y) {
	if (entities.size() == 0) {
		return true;
	}
	for (list<Actor*>::iterator it = entities.begin(); it != entities.end(); it++) {
	//check every entitity against those 
		//later, check to make sure it's one of Zombie, Wall, or Citizen 
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

void StudentWorld::cleanUp()
{
	delete penny;
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
