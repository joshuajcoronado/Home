//-----------------------------------------------------------------------------
// name: jgh-pe.h
// desc: play engine
//
// author: Joshua J Coronado
//   date: 2014
//-----------------------------------------------------------------------------
#ifndef __JGH_PLAY_ENGINE_H__
#define __JGH_PLAY_ENGINE_H__

#include "jgh-globals.h"

bool jgh_pe_init();//initialize our playby setting up our characters
void play_script(SAMPLE * output, int numFrames); // plays our script and moves the playhead
void togglePlay(); //toggles the playing of the script

void JGHtestRight(); // testing right
void JGHtestLeft(); //testing left

float jgh_distance(Vector3D object1, Vector3D object2); // calculates the distance between eye and an object
bool areWeInsideTheHouse(Vector3D location);

SAMPLE jgh_getPlayTime(); //gives us our play time!

#endif
