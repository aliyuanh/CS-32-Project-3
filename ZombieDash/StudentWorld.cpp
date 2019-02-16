#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	//this is where everything is made 

	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{

}

int StudentWorld::init()
{
	//penny = new Penelope();
	Actor* wallBoi = new Wall(3, 3);
	//entities.push_back(wallBoi);

	//from page 22 of spec 
	Level lev(assetPath());
	string levelFile = "level01.txt";
	Level::LoadResult result = lev.loadLevel(levelFile);
	if (result == Level::load_fail_file_not_found) {
		cerr << "cannot find " << levelFile << " data file"<<endl;
	}
	else if (result == Level::load_fail_bad_format) {
		cerr << "level was improperly formatted" << endl;
	}
	else if (result == Level::load_success) {
		cerr << "Successfully loaded level" << endl;
		for (int i = 0; i < LEVEL_WIDTH; i++) {
			for (int j = 0; j < LEVEL_HEIGHT; j++) {
				Level::MazeEntry ge = lev.getContentsOf(i, j);
				Actor* thing;
				switch (ge) {
				case Level::empty:
					cout << "Location " << i << "," << j << " is empty" << endl;
					break;
				case Level::smart_zombie:
					break;
				case Level::dumb_zombie:
					break;
				case Level::player:
					penny = new Penelope(i, j);
					cout << "making penny at " << i << "," << j << endl;
					break;
				case Level::exit:
					break;
				case Level::wall:
					thing = new Wall(i, j);
					break;
				case Level::pit:
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



}
int StudentWorld::move()
{
	if (getLives() == 0) {
		return GWSTATUS_PLAYER_DIED;
	}
	//check if order is correct later 
	moveEnts();
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//runs every tick, so dies after 3 ticks
    //decLives();
	int theKey = -999;
	getKey(theKey);
	//now, theKey has what key was pressed
	switch (theKey) {
	case KEY_PRESS_LEFT:
		penny->moveTo(penny->getX() - 1, penny->getY());
		penny->setDirection(180);
		break;
	case KEY_PRESS_RIGHT:
		penny->moveTo(penny->getX() +1, penny->getY());
		penny->setDirection(0);
		break;
	case KEY_PRESS_UP:
		penny->moveTo(penny->getX(), penny->getY()+1);
		penny->setDirection(90);
		break;
	case KEY_PRESS_DOWN:
		penny->moveTo(penny->getX(), penny->getY()-1);
		penny->setDirection(270);
		break;
	case KEY_PRESS_SPACE:
	case KEY_PRESS_TAB:
	case KEY_PRESS_ENTER:
	default:
		break;
	}

	return GWSTATUS_CONTINUE_GAME;
   // return GWSTATUS_PLAYER_DIED;
}

void StudentWorld::cleanUp()
{
}
