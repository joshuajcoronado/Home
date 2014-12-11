//-----------------------------------------------------------------------------
// name: jgh-pe.cpp
// desc: play stuff
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------
#include "jgh-pe.h"
#include <stdio.h>
#include "jgh-person.h"
#include <iostream>
#include "jgh-sim.h"
#include "jgh-house.h"
#include <math.h>
#include "jgh-localities.h"

using namespace std;

#define HEARING_OUTER 5.0 // the maximum hearing distance
#define HEARING_INNER 1.0 //the hearing minimum before full
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI) // our conversion

vector<JGHPerson *> people; // all the people
bool isPlaying;  // are we playing fool?
SAMPLE playHead; // play head
JGHSndSrc *backgroundMusic; // music that's playing at the title screen
JGHHouse *house; // our house

//-----------------------------------------------------------------------------
// name: jgh_distance
// desc: calculates the distance between two objects. igore y for now
//-----------------------------------------------------------------------------
float jgh_distance(Vector3D object1, Vector3D object2)
{
	return sqrt(pow(object2.x - object1.x, 2) + pow(object2.z - object1.z, 2));
}

//-----------------------------------------------------------------------------
// name: jgh_angle
// desc: calculates the angle between 2 objects in 2D, ignoring y for now
//-----------------------------------------------------------------------------
float jgh_angle(Vector3D first, Vector3D center, Vector3D second)
{
	float a = pow(second.x-first.x,2) + pow(second.z-first.z,2);
    float b = pow(second.x-center.x,2) + pow(second.z-center.z,2);
    float c = pow(center.x-first.x,2) + pow(center.z-first.z,2);
  	return radiansToDegrees(acos( (a+b-c) / sqrt(4*a*b) ));
}


//-----------------------------------------------------------------------------
// name: jgh_hearing_angle
// desc: calculates the hearing angle
//-----------------------------------------------------------------------------
float jgh_hearing_angle(Vector3D object)
{	
	return jgh_angle(Globals::cameraLocation,  object, Globals::cameraLocation + Globals::cameraDirection);
}

//-----------------------------------------------------------------------------
// name: calculateGain
// desc: calculates how loud a voice should be
//-----------------------------------------------------------------------------
float calculateGain(float distance)
{	
	if(distance > HEARING_OUTER)
	{
		return 0.0;
	}else if(distance < HEARING_INNER)
	{
		return 1.0;
	}else
	{
		distance = distance - HEARING_INNER; // let's just get the left over 
		return  1- (distance / (HEARING_OUTER - HEARING_INNER));
	}
}

//-----------------------------------------------------------------------------
// name: calculatePan
// desc: calulate where the sound should be
//-----------------------------------------------------------------------------
float calculatePan(Vector3D loc)
{
	float pan =jgh_hearing_angle(loc);
	bool isForward = false;
	if(pan > 90.0)
	{
		isForward = true;
		pan -= 90.0; //let's get it all down to 90
	}
	pan /= 90.0;
	if(!isForward)
	{
		pan = 1 - pan;
	}
	return pan;
}

//-----------------------------------------------------------------------------
// name: jgh_getPlayTime
// desc: returns the time to us in the form of seconds
//-----------------------------------------------------------------------------
SAMPLE jgh_getPlayTime()
{
	return playHead / JGH_SRATE;
}

//-----------------------------------------------------------------------------
// name: render
// desc: moves forward in time!
//-----------------------------------------------------------------------------
void play_script(SAMPLE * output, int numFrames)
{	
	if(isPlaying)
	{
		playHead += numFrames;
	
		for( int i =0; i < people.size(); i ++)
		{
			JGHPerson * person = people[i];
			float distance = jgh_distance(Globals::cameraLocation, person -> loc);
			float gain = calculateGain(distance);
			//float pan = calculatePan(person -> loc);
			person -> script -> gain(gain);
			//person -> script -> pan(pan);
			person -> playScript(output, numFrames);
		}
	}
	else
	{
		if(!backgroundMusic -> synthesize2(output, numFrames))
		{
			backgroundMusic->rewind();
		}
		
	}
}



//-----------------------------------------------------------------------------
// name: loadPlay
// desc: switches to the play mc play
//-----------------------------------------------------------------------------
void loadPlay()
{
	Globals::sim -> root().removeAllChildren();
	for(int i = 0; i < people.size(); i ++)
	{
		JGHPerson * person = people[i];
		Globals::sim->root().addChild( person );
	}
	house -> houseToSim();

	isPlaying = true; // begin playing now
}

//-----------------------------------------------------------------------------
// name: loadTitleScreen
// desc: loads the title screen an other things from beyond the world
//-----------------------------------------------------------------------------
void loadTitleScreen()
{
	Globals::sim -> root().removeAllChildren(); // remove what was on the screen
	//Globals::sim->root().addChild( title );
	isPlaying = false;
	house -> houseToSim();
}


//-----------------------------------------------------------------------------
// name: togglePlay
// desc: toggles the playing of the stuff!
//-----------------------------------------------------------------------------
void togglePlay()
{
	 if(isPlaying)
	 {
	 	Globals::sim -> pause();
	 	loadTitleScreen();
	 }else
	 {
	 	Globals::sim -> resume();
	 	loadPlay();
	 }

}

//-----------------------------------------------------------------------------
// name: loadTitleScreen
// desc: initializes the person data
//-----------------------------------------------------------------------------
void initializePeople()
{
	JGHPerson * sam = new JGHPerson("sam","sam",JGHLocalities::livingRoom, "sam", SAM_PATH , 45);
	people.push_back(sam);

	JGHPerson * nicole = new JGHPerson("nicole","nicole",JGHLocalities::livingRoom, "nicole", NICOLE_PATH , 30);
	people.push_back(nicole);

	JGHPerson * sidd = new JGHPerson("sidd","sidd",JGHLocalities::livingRoom, "sidd", SIDD_PATH , 180);
	people.push_back(sidd);

	JGHPerson * josh = new JGHPerson("josh","josh",JGHLocalities::livingRoom, "josh", JOSH_PATH , 35);
	people.push_back(josh);

	JGHPerson * rod = new JGHPerson("rod","rod",JGHLocalities::livingRoom, "rod", ROD_PATH , 200);
	people.push_back(rod);
}

//-----------------------------------------------------------------------------
// name: loadTitleScreen
// desc: initializes the title data
//-----------------------------------------------------------------------------
void initialize_title()
{
	backgroundMusic = new JGHSndSrc();
	backgroundMusic-> read("data/sounds/background.wav");
}



void JGHtestRight()
{
	cout << "right ... " << endl;

	if(isPlaying)
	{
		people[0] -> doPath();
		//people[0] -> goToLocation(Vector3D(people[0]->loc.x, people[0]->loc.y, people[0]->loc.z - .1));
		//title -> iLoc.update(Vector3D( title -> iLoc.actual().x + .1, title -> iLoc.actual().y, title -> iLoc.actual().z ));
		//cout << jgh_distance(Globals::cameraLocation, title -> loc) << endl;
	}
} 
void JGHtestLeft()
{	
	cout << "left ... " << endl;
	if(isPlaying)
	{
		//people[0] -> goToLocation(JGHLocalities::entrance)	;
		//people[0] -> goToLocation(Vector3D(people[0]->loc.x, people[0]->loc.y, people[0]->loc.z + .1));
		//title -> iLoc.update(Vector3D( title -> loc.x - .1, title -> loc.y, title -> loc.z ));
		//cout << jgh_distance(Globals::cameraLocation, title -> loc) << endl;
	}
}

//-----------------------------------------------------------------------------
// name: areWeInsideTheHouse
// desc: let's us know if we're in the house
//-----------------------------------------------------------------------------
bool areWeInsideTheHouse(Vector3D location)
{
	return house -> insideHouse(location);
}

//-----------------------------------------------------------------------------
// name: initializeHouse
// desc: initializes the house Data
//-----------------------------------------------------------------------------
void initializeHouse()
{
	house = new JGHHouse();

}

//-----------------------------------------------------------------------------
// name: initializePlay
// desc: initializes the play data
//-----------------------------------------------------------------------------
void initializePlay()
{
	initializePeople();
	initializeHouse();
}
//-----------------------------------------------------------------------------
// name: jgh_pe_init
// desc: initializes our play by adding people
//-----------------------------------------------------------------------------
bool jgh_pe_init()
{
	cout << "[Home]: initializing home..." << endl;
	
	initializePlay();
	initialize_title();
	loadTitleScreen();
	return true;
}


