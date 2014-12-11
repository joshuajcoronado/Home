//-----------------------------------------------------------------------------
// name: jgh-person.h
// desc: a person
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __JGH_PERSON_H__
#define __JGH_PERSON_H__

#include "x-vector3d.h"
#include "jgh-entity.h"
#include "jgh-sndsrc.h"
#include <vector>
#include "jgh-globals.h"
#include <string.h>
#include "jgh-localities.h"


//-----------------------------------------------------------------------------
// name: class JGHPerson
// desc: person class
//-----------------------------------------------------------------------------
class JGHPerson:public JGHEntity
{
public:
    // default constructor
    JGHPerson(std::string n, std::string m, Vector3D locat, std::string s, unsigned int p, SAMPLE t);
    // destructor
    ~JGHPerson();

public:
    void update(YTimeInterval dt);
    void render();
    void initModel(char * path);

public:
    //model
     GLMmodel* model;
    // location
    Vector3D loc;
    //slew
    iSlew3D iLoc;
    //the script
    JGHSndSrc * script;
    //name
    std::string name;
    // goal location
    Vector3D goalLocation;
    //let's you know if you're at the goal location
    bool atLocation;
    //the path they're going to take
    std::vector<Vector3D> path;
    //let's you know if we're in motion
    bool doingPath;
    //the time which to begin moving
    SAMPLE time;

public:
    void playScript(SAMPLE * output, int numFrames);
    void goToLocation(Vector3D location);
    void doPath();
};
#endif