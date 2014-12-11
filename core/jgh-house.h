//-----------------------------------------------------------------------------
// name: jgh-house.h
// desc: a house
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __JGH_HOUSE_H__
#define __JGH_HOUSE_H__

#include "jgh-entity.h"
#include "jgh-sim.h"

using namespace std;

//-----------------------------------------------------------------------------
// name: class JGHWALL
// desc: builds a wall
//-----------------------------------------------------------------------------
class JGHWall : public JGHEntity
{
public:
    void render();
    unsigned int position;
    Vector3D dimensions;
    void init(unsigned int pos, Vector3D dim, GLuint tex, Vector3D location); 
};

//-----------------------------------------------------------------------------
// name: class JGHRoom
// desc: builkds a room
//-----------------------------------------------------------------------------
class JGHRoom
{
public:
    JGHRoom(Vector3D dim, Vector3D location, std::array<bool, 6> walley, std::array<unsigned int, 6> texes);
    ~JGHRoom();
public:
    bool insideRoom(Vector3D location);
    void toSim();
public:
    std::vector<JGHWall *> walls;
    Vector3D dimensions;
    Vector3D loc;
};


//-----------------------------------------------------------------------------
// name: class jghHouse
// desc: ourHouse
//-----------------------------------------------------------------------------
class JGHHouse
{
public: 
      JGHHouse();
      ~JGHHouse();
public: 
    bool insideHouse(Vector3D location);
    void houseToSim();
private:
    std::vector<JGHRoom *> rooms;
};


#endif