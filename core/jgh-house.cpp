//-----------------------------------------------------------------------------
// name: jgh-HOUSE.cpp
// desc: a house
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "jgh-house.h"

#define FRONT 0
#define BACK 1
#define LEFT 2
#define RIGHT 3
#define TOP 4
#define BOTTOM 5


//-------------------------------------------------------------------------------
// name: init
// desc: 
//-------------------------------------------------------------------------------
void JGHWall::init(unsigned int pos, Vector3D dim, GLuint tex, Vector3D location)
{
    position = pos;
    dimensions = dim;
    setTexture(tex);
    loc = location;
}



//-------------------------------------------------------------------------------
// name: render
// desc: 
//-------------------------------------------------------------------------------
void JGHWall::render()
{
    GLfloat width = dimensions.x / 2;
    GLfloat height = dimensions.y / 2;
    GLfloat depth = dimensions.z / 2;

    enableTex();
    glBegin(GL_QUADS);
    switch(position)
    {
        case FRONT:
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height,  depth);  // Bottom Left Of The Texture and Quad
            glTexCoord2f(1.0f, 0.0f); glVertex3f( width, -height,  depth);  // Bottom Right Of The Texture and Quad
            glTexCoord2f(1.0f, 1.0f); glVertex3f( width,  height,  depth);  // Top Right Of The Texture and Quad
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-width,  height,  depth);  // Top Left Of The Texture and Quad
            break;
        case BACK:
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height, -depth);  // Bottom Right Of The Texture and Quad
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-width,  height, -depth);  // Top Right Of The Texture and Quad
            glTexCoord2f(0.0f, 1.0f); glVertex3f( width,  height, -depth);  // Top Left Of The Texture and Quad
            glTexCoord2f(0.0f, 0.0f); glVertex3f( width, -height, -depth);  // Bottom Left Of The Texture and Quad
            break;
        case RIGHT:
            glTexCoord2f(1.0f, 0.0f); glVertex3f( width, -height, -depth);  // Bottom Right Of The Texture and Quad
            glTexCoord2f(1.0f, 1.0f); glVertex3f( width,  height, -depth);  // Top Right Of The Texture and Quad
            glTexCoord2f(0.0f, 1.0f); glVertex3f( width,  height, depth);  // Top Left Of The Texture and Quad
            glTexCoord2f(0.0f, 0.0f); glVertex3f( width, -height,  depth);  // Bottom Left Of The Texture and Quad
            break;
        case LEFT:
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, -depth);  // Bottom Left Of The Texture and Quad
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height,  depth);  // Bottom Right Of The Texture and Quad
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-width,  height,  depth);  // Top Right Of The Texture and Quad
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-width,  height, -depth);  // Top Left Of The Texture and Quad
            break;
        case TOP:
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-width,  height, -depth);  // Top Left Of The Texture and Quad
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-width,  height,  depth);  // Bottom Left Of The Texture and Quad
            glTexCoord2f(1.0f, 0.0f); glVertex3f( width,  height,  depth);  // Bottom Right Of The Texture and Quad
            glTexCoord2f(1.0f, 1.0f); glVertex3f( width,  height, -depth);  // Top Right Of The Texture and Quad
            break;
        case BOTTOM:
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, -height, -depth);  // Top Right Of The Texture and Quad
            glTexCoord2f(0.0f, 1.0f); glVertex3f( width, -height, -depth);  // Top Left Of The Texture and Quad
            glTexCoord2f(0.0f, 0.0f); glVertex3f( width, -height,  depth);  // Bottom Left Of The Texture and Quad
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height,  depth);  // Bottom Right Of The Texture and Quad
            break;
    }
    glEnd();
    disableTex();
}

//-------------------------------------------------------------------------------
// name: inside room
// desc: checks if you're inside the house
//-------------------------------------------------------------------------------
bool JGHRoom::insideRoom(Vector3D location)
{
    float x = location.x;
    float x_min = loc.x - dimensions.x / 2;
    float x_max = loc.x + dimensions.x / 2;
    float z = location.z;
    float z_min = loc.z - dimensions.z / 2;
    float z_max = loc.z + dimensions.z / 2;


    //check volumes in the room
    if((x <= x_max) && (x >= x_min))
    {
        if((z <= z_max) && (z >= z_min))
        {
            return true;
        }

    }
    return false;
}

//-------------------------------------------------------------------------------
// name: inside house
// desc: checks if you're inside the house
//-------------------------------------------------------------------------------
bool JGHHouse::insideHouse(Vector3D location)
{


    for(int i = 0; i < rooms.size(); i++)
    {
        

        if(rooms[i] -> insideRoom(location))
        {
            return true;
        }
    }


    return false;
}

//-------------------------------------------------------------------------------
// name: jghHOUSE()
// desc: constructor
//-------------------------------------------------------------------------------
JGHRoom::JGHRoom(Vector3D dim, Vector3D location, std::array<bool, 6> walley, std::array<unsigned int, 6> texes)
{
    dimensions = dim;
    loc = location;

    for(int i = 0; i < walley.size(); i ++)
    {
        if(walley[i])
        {
            JGHWall * wall = new JGHWall();
            wall -> init(i, dimensions, texes[i], loc);
            walls.push_back(wall);
        }
    }
}

//-------------------------------------------------------------------------------
// name: jghRoom()
// desc: deconstructor
//-------------------------------------------------------------------------------
JGHRoom::~JGHRoom()
{
    for(int i = 0; i < walls.size(); i++)
    {
        delete(walls[i]);
    }
}

//-------------------------------------------------------------------------------
// name: ToSim
// desc: adds all the rooms to the simulation
//-------------------------------------------------------------------------------
void JGHRoom::toSim()
{

    for(int i = 0; i < walls.size(); i++)
    {
        Globals::sim -> root().addChild(walls[i]);
    }
}


//-------------------------------------------------------------------------------
// name: houseToSim
// desc: adds all the rooms to the simulation
//-------------------------------------------------------------------------------
void JGHHouse::houseToSim()
{
    for(int i = 0; i < rooms.size(); i++)
    {
        rooms[i] -> toSim();
    }
}

//-------------------------------------------------------------------------------
// name: jghHOUSE()
// desc: constructor
//-------------------------------------------------------------------------------
JGHHouse::JGHHouse()
{  
    //front, back, left, right, top, bottom
    array<bool, 6> entranceWalls = { true, false, false, true, true, true };
    array<unsigned int, 6> entranceTex = { ENTRANCE_ROOM_FRONT_TEX, JGH_TEX_NONE, JGH_TEX_NONE, ENTRANCE_ROOM_RIGHT_TEX, ENTRANCE_ROOM_TOP_TEX, ENTRANCE_ROOM_BOTTOM_TEX};
    JGHRoom * entrance = new JGHRoom(Vector3D(4, 4, 5), Vector3D(0 , 0, 0), entranceWalls, entranceTex);
    rooms.push_back(entrance);

    //front, back, left, right, top, bottom
    array<bool, 6> officeWalls = { false, false, true, true, true, true };
    array<unsigned int, 6> officeTex = { JGH_TEX_NONE,JGH_TEX_NONE, OFFICE_ROOM_LEFT_TEX, OFFICE_ROOM_RIGHT_TEX, OFFICE_ROOM_TOP_TEX, OFFICE_ROOM_BOTTOM_TEX };
    JGHRoom * office = new JGHRoom(Vector3D(4, 4, 5), Vector3D(0, 0, - 5), officeWalls, officeTex);
    rooms.push_back(office);


    //front, back, left, right, top, bottom
    array<bool, 6> livingRoomWalls = { false, true, true, true, true, true };
    array<unsigned int, 6> livingRoomTex = { JGH_TEX_NONE,LIVING_ROOM_BACK_TEX, LIVING_ROOM_LEFT_TEX, LIVING_ROOM_RIGHT_TEX, LIVING_ROOM_TOP_TEX, LIVING_ROOM_BOTTOM_TEX };
    JGHRoom * livingRoom = new JGHRoom(Vector3D(8, 4, 5), Vector3D(-2, 0, - 10), livingRoomWalls, livingRoomTex);
    rooms.push_back(livingRoom);

    //front,, left,, top, bottom
    array<bool, 6> kitchenEntranceWalls = { true, false, true, false, true, true };
    array<unsigned int, 6> kitchenEntranceTex = { KITCHEN_ENTRANCE_FRONT_TEX,JGH_TEX_NONE, KITCHEN_ENTRANCE_LEFT_TEX, JGH_TEX_NONE,  KITCHEN_ENTRANCE_TOP_TEX, KITCHEN_ENTRANCE_BOTTOM_TEX };
    JGHRoom * kitchenEntrance = new JGHRoom(Vector3D(4, 4, 5), Vector3D(-4, 0, 0), kitchenEntranceWalls, kitchenEntranceTex);
    rooms.push_back(kitchenEntrance);

    //front, back, left, right, top, bottom
    array<bool, 6> kitchenWalls = { false, true, false, false, true, true };
    array<unsigned int, 6> kitchenTex = { JGH_TEX_NONE,KITCHEN_BACK_TEX, JGH_TEX_NONE, JGH_TEX_NONE, KITCHEN_TOP_TEX, KITCHEN_BOTTOM_TEX };
    JGHRoom * kitchen= new JGHRoom(Vector3D(4, 4, 5), Vector3D(-4, 0, -5), kitchenWalls, kitchenTex);
    rooms.push_back(kitchen);

    //front, back, left, right, top, bottom
    array<bool, 6> hallway1Walls = { false, true, false, false, true, true };
    array<unsigned int, 6> hallway1Tex = { JGH_TEX_NONE, HALL1_BACK_TEX, JGH_TEX_NONE, JGH_TEX_NONE, HALL1_TOP_TEX,  HALL1_BOTTOM_TEX};
    JGHRoom * hallway1= new JGHRoom(Vector3D(4, 4, 5), Vector3D(-8, 0, -5), hallway1Walls, hallway1Tex);
    rooms.push_back(hallway1);

    //front, back, left, right, top, bottom
    array<bool, 6> hallway2Walls = { false, true, false, false, true, true };
    array<unsigned int, 6> hallway2Tex = { JGH_TEX_NONE, HALL2_BACK_TEX, JGH_TEX_NONE, JGH_TEX_NONE, HALL2_TOP_TEX,  HALL2_BOTTOM_TEX };
    JGHRoom * hallway2= new JGHRoom(Vector3D(4, 4, 5), Vector3D(-12, 0, -5), hallway2Walls, hallway2Tex);
    rooms.push_back(hallway2);

    //front, back, left, right, top, bottom
    array<bool, 6> hallway3Walls = { false, true, true, false, true, true };
    array<unsigned int, 6> hallway3Tex = { JGH_TEX_NONE, HALL3_BACK_TEX, H3_LEFT, JGH_TEX_NONE, HALL3_TOP_TEX,  HALL3_BOTTOM_TEX};
    JGHRoom * hallway3= new JGHRoom(Vector3D(4, 4, 5), Vector3D(-16, 0, -5), hallway3Walls, hallway3Tex);
    rooms.push_back(hallway3);

    //front, back, left, right, top, bottom
    array<bool, 6> alexWalls = { true, false, true, false, true, true };
    array<unsigned int, 6> alexTex = { ALEX_ROOM_FRONT_TEX,JGH_TEX_NONE, ALEX_ROOM_LEFT_TEX, JGH_TEX_NONE, ALEX_TOP, ALEX_BOTTOM };
    JGHRoom * alex= new JGHRoom(Vector3D(4, 4, 5), Vector3D(-8, 0, 0), alexWalls, alexTex);
    rooms.push_back(alex);

    //front, back, left, right, top, bottom
    array<bool, 6> bathroomWalls = { true, false, true, false, true, true };
    array<unsigned int, 6> bathroomTex = { BATH_ROOM_FRONT_TEX,JGH_TEX_NONE,  BATH_ROOM_LEFT_TEX, JGH_TEX_NONE,  BATH_TOP,  BATH_BOTTOM };
    JGHRoom * bathroom= new JGHRoom(Vector3D(4, 4, 5), Vector3D(-12, 0, 0), bathroomWalls, bathroomTex);
    rooms.push_back(bathroom);

    //front, back, left, right, top, bottom
    array<bool, 6> joshWalls = { true, false, true, false, true, true };
    array<unsigned int, 6> joshTex = { JOSH_ROOM_FRONT_TEX,JGH_TEX_NONE, JOSH_ROOM_LEFT_TEX, JGH_TEX_NONE, JOSH_ROOM_TOP_TEX, JOSH_ROOM_BOTTOM_TEX };
    JGHRoom * josh= new JGHRoom(Vector3D(4, 4, 5), Vector3D(-16, 0, 0), joshWalls, joshTex);
    rooms.push_back(josh);

    // //front, back, left, right, top, bottom
    // array<bool, 6> nicoleWalls = { true, true, true, false, true, true };
    // array<unsigned int, 6> nicoleTex = { NICOLE_ROOM_FRONT_TEX, NICOLE_ROOM_BACK_TEX, NICOLE_ROOM_LEFT_TEX, JGH_TEX_NONE, NICOLE_ROOM_TOP_TEX, NICOLE_ROOM_BOTTOM_TEX };
    // JGHRoom * nicole= new JGHRoom(Vector3D(4, 4, 5), Vector3D(-20, 0, -5), nicoleWalls, nicoleTex);
    // rooms.push_back(nicole);


}

//-------------------------------------------------------------------------------
// name: jghHOUSE()
// desc: deconstructor
//-------------------------------------------------------------------------------
JGHHouse::~JGHHouse()
{
    for(int i = 0; i < rooms.size(); i++)
    {
        delete(rooms[i]);
    }
}