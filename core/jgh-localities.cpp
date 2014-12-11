//-----------------------------------------------------------------------------
// name: jgh-localities.cpp
// desc: a place
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "jgh-localities.h"


Vector3D JGHLocalities::livingRoom( -3.0f , 0.0f , -10.0f );
Vector3D JGHLocalities::livingRoomEntrance( 0.0f , 0.0f , -10.0f  );
Vector3D JGHLocalities::kitchen( -4.0f , 0.0f , 0.0f  );
Vector3D JGHLocalities::entrance( 0.0f , 0.0f , 0.0f  );
Vector3D JGHLocalities::kitchenToRoom( -4.0f , 0.0f , -5.0f  );
Vector3D JGHLocalities::hall1( -8.0f , 0.0f , -5.0f  );
Vector3D JGHLocalities::hall2( -12.0f , 0.0f , -5.0f  );
Vector3D JGHLocalities::hall3( -16.0f , 0.0f , -5.0f  );
Vector3D JGHLocalities::bathroom( -12.0f , 0.0f , 0.0f  );
Vector3D JGHLocalities::joshRoom( -16.0f , 0.0f , 0.0f  );

std::vector<Vector3D> JGHLocalities::livingRoomToKitchenToRoom()
{
	std::vector<Vector3D> path;
	path.push_back(JGHLocalities::livingRoomEntrance);
	path.push_back(JGHLocalities::entrance);
	path.push_back(JGHLocalities::kitchen);
	path.push_back(JGHLocalities::kitchenToRoom);
	return path;
}

std::vector<Vector3D> JGHLocalities::livingRoomToHall2()
{
	std::vector<Vector3D> path = livingRoomToKitchenToRoom();
	path.push_back(JGHLocalities::hall2);

	return path;
}

std::vector<Vector3D> JGHLocalities::livingRoomToBathroom()
{
	std::vector<Vector3D> path = livingRoomToHall2();
	path.push_back(JGHLocalities::bathroom);
	return path;
}

std::vector<Vector3D> JGHLocalities::livingRoomToHall3()
{
	std::vector<Vector3D> path = livingRoomToKitchenToRoom();
	path.push_back(JGHLocalities::hall3);
	return path;
}

std::vector<Vector3D> JGHLocalities::livingRoomToJoshRoom()
{
	std::vector<Vector3D> path = livingRoomToHall3();
	path.push_back(JGHLocalities::joshRoom);
	return path;
}



