//-----------------------------------------------------------------------------
// name: jgh-localities.h
// desc: a place
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __JGH_LOCALITITES_H__
#define __JGH_LOCALITITES_H__
#include "x-vector3d.h"
#include "jgh-globals.h"
#include <vector>

#define VELOCITY .1

#define JOSH_PATH 1 
#define SIDD_PATH 2
#define NICOLE_PATH 3
#define SAM_PATH 4
#define ROD_PATH 5

class JGHLocalities
{
public: 
	static Vector3D livingRoom;
	static Vector3D livingRoomEntrance;
	static Vector3D kitchen;
	static Vector3D joshRoom;
	static Vector3D entrance;
	static Vector3D kitchenToRoom;
	static Vector3D hall1;
	static Vector3D hall2;
	static Vector3D hall3; 
	static Vector3D bathroom;
public:
	static std::vector<Vector3D> livingRoomToKitchenToRoom();
	static std::vector<Vector3D> livingRoomToBathroom();
	static std::vector<Vector3D> livingRoomToJoshRoom();
	static std::vector<Vector3D> livingRoomToHall2();
	static std::vector<Vector3D> livingRoomToHall3();
};

#endif