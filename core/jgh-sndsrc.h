//-----------------------------------------------------------------------------
// name: jgh-sndsrc.h
// desc: a sound file object / wrapper updated from ge's sndsrc dispersed in class
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------

#ifndef __JGH_SNDSRC_H__
#define __JGH_SNDSRC_H__

#include <string>
#include "jgh-globals.h"
using namespace std;

//-----------------------------------------------------------------------------
// name: class JGHSndSrc 
// desc: sndsrc class
//-----------------------------------------------------------------------------
class JGHSndSrc
{
public:
    // default constructor
    JGHSndSrc();
    // overloaded constructor
    JGHSndSrc( const string & filename );
    // destructor
    ~JGHSndSrc();

public:
    // clean up
    void cleanup();
    // read a sound file
    bool read( const string & filename );
    // rewind to pos
    void rewind( int position = 0 );
    // get number of frames in file
    int getSize();
    // rate
    void rate( SAMPLE rate );
    // gain (volume)
    void gain( SAMPLE gain );
    // pan
    void pan( SAMPLE pan);
    // synthesize the next buffer (stereo)
    bool synthesize2( SAMPLE * output, int numFrames );
    SAMPLE getCurrSample();

public:
    

protected: // data
    int m_size; // length of file (in frames)
    int m_srate; // sample rate of file
    int m_channels; // number of channers in file
    SAMPLE * m_buffer; // audio buffer
    SAMPLE m_playHead; // play head
    SAMPLE m_rate; // play rate
    SAMPLE m_gain; // volume
    SAMPLE m_pan; // stereo position
    SAMPLE currSample; // our curr Sample

};




#endif
