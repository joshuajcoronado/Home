//-----------------------------------------------------------------------------
// name: jgh-globals.h
// desc: global stuffs
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------
#ifndef __JGH_GLOBALS_H__
#define __JGH_GLOBALS_H__


#include "x-def.h"
#include "x-audio.h"
#include "x-gfx.h"
#include "x-vector3d.h"
#include "y-waveform.h"

// c++
#include <string>
#include <map>
#include <vector>
#include <utility>

// defines
#define JGH_SRATE        44100
#define JGH_FRAMESIZE    512
#define JGH_NUMCHANNELS  2
#define JGH_MAX_TEXTURES 32
#define DEFAULT_SPEED 1.0


//-----------------------------------------------------------------------------
// name: enum JoshGoHomeTextureNames
// desc: texture names
//-----------------------------------------------------------------------------
enum JoshGoHomeTextureNames
{
    JGH_TEX_NONE = 0,
    JOSH_TEX,
    NICOLE_TEX,
    SIDD_TEX,
    ROD_TEX,
    SAM_TEX, 
    ENTRANCE_ROOM_BOTTOM_TEX,
    ENTRANCE_ROOM_TOP_TEX,
    ENTRANCE_ROOM_FRONT_TEX,
    ENTRANCE_ROOM_RIGHT_TEX,
    OFFICE_ROOM_LEFT_TEX,
    OFFICE_ROOM_RIGHT_TEX,
    OFFICE_ROOM_TOP_TEX,
    OFFICE_ROOM_BOTTOM_TEX,
    LIVING_ROOM_BACK_TEX,
    LIVING_ROOM_TOP_TEX,
    LIVING_ROOM_BOTTOM_TEX,
    LIVING_ROOM_RIGHT_TEX,
    LIVING_ROOM_LEFT_TEX,
    KITCHEN_ENTRANCE_FRONT_TEX,
    KITCHEN_ENTRANCE_LEFT_TEX,
    KITCHEN_ENTRANCE_TOP_TEX,
    KITCHEN_ENTRANCE_BOTTOM_TEX,
    KITCHEN_BACK_TEX,
    KITCHEN_TOP_TEX,
    KITCHEN_BOTTOM_TEX,
    HALL1_BACK_TEX,
    HALL1_TOP_TEX,
    HALL1_BOTTOM_TEX,
    HALL2_BACK_TEX,
    HALL2_TOP_TEX,
    HALL2_BOTTOM_TEX,
    HALL3_BACK_TEX,
    HALL3_TOP_TEX,
    HALL3_BOTTOM_TEX,
    ALEX_ROOM_FRONT_TEX,
    ALEX_ROOM_TOP_TEX,
    ALEX_ROOM_BOTTOM_TEX,
    ALEX_ROOM_LEFT_TEX,
    BATH_ROOM_FRONT_TEX,
    BATH_ROOM_TOP_TEX,
    BATH_ROOM_BOTTOM_TEX,
    BATH_ROOM_LEFT_TEX,
    JOSH_ROOM_FRONT_TEX,
    JOSH_ROOM_TOP_TEX,
    JOSH_ROOM_BOTTOM_TEX,
    JOSH_ROOM_LEFT_TEX,
    NICOLE_ROOM_FRONT_TEX,
    NICOLE_ROOM_BACK_TEX,
    NICOLE_ROOM_TOP_TEX,
    NICOLE_ROOM_BOTTOM_TEX,
    NICOLE_ROOM_LEFT_TEX, 
    BATH_TOP, 
    BATH_BOTTOM,
    ALEX_BOTTOM,
    ALEX_TOP,
    H3_LEFT
};



// forward reference
class JGHSim;




//-----------------------------------------------------------------------------
// name: class Globals
// desc: the global class
//-----------------------------------------------------------------------------
class Globals
{
public:
    // top level root simulation
    static JGHSim * sim;
    
    // path
    static std::string path;
    // path to datapath
    static std::string relpath;
    // datapath
    static std::string datapath;
    // version
    static std::string version;

    // last audio buffer
    static SAMPLE * lastAudioBuffer;
    static SAMPLE * lastAudioBufferMono;
    static SAMPLE * audioBufferWindow;
    static unsigned int lastAudioBufferFrames;
    static unsigned int lastAudioBufferChannels;

    // waveform
    static YWaveform * waveform;

    // width and height of the window
    static GLsizei windowWidth;
    static GLsizei windowHeight;
    static GLsizei lastWindowWidth;
    static GLsizei lastWindowHeight;

    // graphics fullscreen
    static GLboolean fullscreen;
    // render waveform
    static GLboolean renderWaveform;
    // blend pane instead of clearing screen
    static GLboolean blendScreen;
    // blend screen parameters
    static Vector3D blendAlpha;
    static GLfloat blendRed;
    // fill mode
    static GLenum fillmode;
    // background color
    static iSlew3D bgColor;
    // view stuff
    static Vector3D viewRadius;
    static Vector3D viewEyeY;
    static Vector3D fov;
    
    // textures
    static GLuint textures[];
    
    // light 0 position
    static GLfloat light0_pos[4];
    // light 1 parameters
    static GLfloat light1_ambient[4];
    static GLfloat light1_diffuse[4];
    static GLfloat light1_specular[4];
    static GLfloat light1_pos[4];
    // line width
    static GLfloat linewidth;
    // do
    static GLboolean fog;
    static GLuint fog_mode[4];   // storage for three/four types of fog
    static GLuint fog_filter;    // which fog to use
    static GLfloat fog_density;  // fog density
    
    // colors
    static Vector3D ourWhite;
    static Vector3D ourRed;
    static Vector3D ourBlue;
    static Vector3D ourOrange;
    static Vector3D ourGreen;
    static Vector3D ourGray;
    static Vector3D ourYellow;
    static Vector3D ourSoftYellow;
    static Vector3D ourPurple;
    static Vector3D location; //our location

    //looking parameters
    static float rotationAngle;
    static Vector3D cameraLocation;
    static Vector3D cameraDirection;
};




#endif
