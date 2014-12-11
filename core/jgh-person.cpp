//-----------------------------------------------------------------------------
// name: jgh-person.cpp
// desc: a person
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "jgh-person.h"
#include "jgh-sndsrc.h"
#include "jgh-localities.h"
#include "jgh-pe.h"
#include <complex>
#include <math.h>
using namespace std;


//-------------------------------------------------------------------------------
// name: initModel
// desc: sets things up
//-------------------------------------------------------------------------------
void JGHPerson::initModel(char* path)
{
    model = glmReadOBJ(path);
    GLfloat unit =  glmUnitize(model);
    glmFacetNormals(model);
    glmVertexNormals(model, 120);
    glmReverseWinding(model);
    glmLinearTexture(model);
    glmScale(model,  .8);

}


//-------------------------------------------------------------------------------
// name: JGHPerson() 
// desc: constructor
//-------------------------------------------------------------------------------
JGHPerson::JGHPerson(std::string n, std::string m, Vector3D location, std::string s, unsigned int p, SAMPLE t)
{
	name = n;
	
	m = "sam";
	m = "data/obj_files/" + m + ".obj";
	char *obj = new char[m.length() + 1];
	strcpy(obj, m.c_str());
	delete obj;
	initModel(obj);
	
	script = new JGHSndSrc();
	s =  "data/sounds/" + s+ ".wav";
	const char * sound = s.c_str();
	script -> read(sound);


	time = t;
	switch(p)
	{
		case JOSH_PATH:
			path = JGHLocalities::livingRoomToJoshRoom();
			setTexture(JOSH_TEX);
			location = Vector3D(location.x - 1.5, location.y, location.z -1.5);
			break;
		case SIDD_PATH:
			path = JGHLocalities::livingRoomToHall3();
			setTexture(SIDD_TEX);
			location = Vector3D(location.x + 1.5, location.y, location.z -1.5);
			break;
		case NICOLE_PATH:
			path = JGHLocalities::livingRoomToJoshRoom();
			setTexture(NICOLE_TEX);
			location = Vector3D(location.x - 1.5, location.y, location.z +1.5);
			break;
		case SAM_PATH:
			path = JGHLocalities::livingRoomToBathroom();
			setTexture(SAM_TEX);
			location = Vector3D(location.x + 1.5, location.y, location.z +1.5);
			break;
		case ROD_PATH:
			path = JGHLocalities::livingRoomToHall2();
			setTexture(ROD_TEX);
			location = Vector3D(location.x , location.y, location.z);
			break;
	}
	iLoc = iSlew3D(location, .3); //originally .3
	
	goalLocation = location;
	
}



//-------------------------------------------------------------------------------
// name: render()
// desc: ...
//-------------------------------------------------------------------------------
void JGHPerson::render()
{
	
	enableTex();
	glTranslatef(loc.x, loc.y , loc.z);

	float scale = 1;
    scale +=  std::abs(script -> getCurrSample() ) / 2;
    glScalef(scale, scale, scale);
    glRotatef(270, 1, 0, 0);

    glmDraw(model, GLM_TEXTURE | GLM_SMOOTH );
    disableTex();
}

//-------------------------------------------------------------------------------
// name: doPath
// desc: doesPath
//-------------------------------------------------------------------------------
void JGHPerson::doPath()
{
	doingPath = true;
}


//-------------------------------------------------------------------------------
// name: update()
// desc: ...
//-------------------------------------------------------------------------------
void JGHPerson::update( YTimeInterval dt )
{
	if(jgh_getPlayTime() > time && !doingPath)
	{
		doPath();
	}

	float range = .5;
    iLoc.interp( dt );
    loc = iLoc.actual();
    float x_min = iLoc.actual().x - range;
    float x_max = iLoc.actual().x + range;
    float z_min = iLoc.actual().z - range;
    float z_max = iLoc.actual().z + range;
    if(x_min <= goalLocation.x && x_max >= goalLocation.x && z_min <= goalLocation.z && z_max >= goalLocation.z)
    {
    	atLocation = true;
    }else
    {
    	

    	atLocation = false;
    }

    if(doingPath)
    {
    	if(atLocation)
    	{
    		if(path.size() > 0)
    		{
  
    				Vector3D next = path.front();
    				if(name == "josh" && path.size() == 1)
						{
							next = Vector3D(next.x - 1, next.y, next.z);
						}
						if(name == "nicole")
						{
							next = Vector3D(next.x + 1, next.y, next.z);
						}
					goToLocation(next);
    			path.erase(path.begin());
    		}
    	}
    }
}

//-------------------------------------------------------------------------------
// name: goToLocation
// desc: go to a location
//-------------------------------------------------------------------------------
void JGHPerson::goToLocation(Vector3D location)
{
	goalLocation = location;

	iLoc.update(location);
}

//-------------------------------------------------------------------------------
// name: ~JGHPerson() 
// desc: destructor
//-------------------------------------------------------------------------------
JGHPerson::~JGHPerson()
{
	delete model;
	delete script;
}




//-------------------------------------------------------------------------------
// name: playScript
// desc: plays the script if we're nearby
//-------------------------------------------------------------------------------
void JGHPerson::playScript(SAMPLE * output, int numFrames)
{
	script ->  synthesize2( output, numFrames );
}