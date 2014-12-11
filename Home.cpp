//----------------------------------------------------------------------------
// name: JoshGoHome_Home.cpp
// desc: The final frontier
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
// date: fall 2014
//----------------------------------------------------------------------------
#include <iostream>
#include "core/jgh-audio.h"
#include "core/jgh-gfx.h"
#include "core/jgh-globals.h"

using namespace std;

//----------------------------------------------------------------------------
// name: main()
// desc: application entry point
//----------------------------------------------------------------------------
int main( int argc, const char ** argv )
{
       // start real-time audio
    if( !jgh_audio_init( JGH_SRATE, JGH_FRAMESIZE, JGH_NUMCHANNELS ) )
    {
        // error message
        cerr << "[Home]: cannot initialize real-time audio I/O..." << endl;
        return -1;
    }
    
	// invoke graphics setup and loop
    if( !jgh_gfx_init( argc, argv ) )
    {
        // error message
        cerr << "[Home]: cannot initialize graphics/data system..." << endl;
        return -1;
    }   
    
 
    // start audio
    if( !jgh_audio_start() )
    {
        // error message
        cerr << "[Home]: cannot start real-time audio I/O..." << endl;
        return -1;
    }

    // graphics loop
    jgh_gfx_loop();

    return 0;
}