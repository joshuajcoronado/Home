//-----------------------------------------------------------------------------
// name: jgh-sndsrc.cpp
// desc: a sound file object / wrapper updated from ge's sndsrc dispersed in class
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------

#include "jgh-sndsrc.h"
#include <sndfile.h>
#include <iostream>
#include <cmath>

using namespace std;

// read data from file
SAMPLE * readData( const string & filename, int * size, int * srate, int * channels )
{
    // handle
    SNDFILE * sf = NULL;
    // info
    SF_INFO info;
    // because the doc says
    info.format = 0;
    // ...
    SAMPLE * buffer = NULL;
    // zero out
    *size = 0;
    *srate = 0;
    
    // open it
    sf = sf_open( filename.c_str(), SFM_READ, &info );
    // check it
    if( !sf )
    {
        // error message
        cout << "error: cannot open '" << filename << "'" << endl;
        return NULL;
    }
    
    // allocate the whole thing!
    buffer = new SAMPLE[info.frames*info.channels];
    // check it
    if( !buffer )
    {
        // error message
        cout << "error: out of memory... frak" << endl;
        goto done;
    }
    
    // read it
    if( sf_read_float( sf, buffer, info.frames*info.channels ) != info.frames*info.channels )
    {
        // error message
        cout << "error: can't read file..." << endl;
        // free
        delete [] buffer; buffer = NULL;
        goto done;
    }
    
    // set size
    *size = info.frames;
    // set srate
    *srate = info.samplerate;
    // set channels
    *channels = info.channels;
    
done:
    // close sf
    if( sf ) sf_close( sf );
    
    return buffer;
}


JGHSndSrc::JGHSndSrc()
{
    m_size = 0;
    m_srate = 0;
    m_channels = 0;
    m_buffer = NULL;
    m_playHead = 0;
    m_rate = 1;
    m_gain = 1;
    m_pan = .5;
  
}


JGHSndSrc::JGHSndSrc( const string & filename )
{
    JGHSndSrc();
    read( filename );
}


JGHSndSrc::~JGHSndSrc()
{
    cleanup();
}


// clean up
void JGHSndSrc::cleanup()
{
    if( m_buffer )
    {
        delete [] m_buffer;
        m_buffer = NULL;
    }
    
    m_playHead = 0;
}


// read a sound file
bool JGHSndSrc::read( const string & filename )
{
    // clean up
    cleanup();
    // read data
    m_buffer = readData( filename, &m_size, &m_srate, &m_channels );
    // rewind
    rewind();
    
    return (m_buffer != NULL );
}


// rewind
void JGHSndSrc::rewind( int position )
{
    m_playHead = position;
}

// size
int JGHSndSrc::getSize()
{
    return m_size;
}

// rate
void JGHSndSrc::rate( SAMPLE rate )
{
    m_rate = rate;
}

// gain
void JGHSndSrc::gain( SAMPLE gain )
{
    m_gain = gain;
}

// gain
void JGHSndSrc::pan( SAMPLE pan )
{
    if(pan > 1) m_pan = 1.0;
    if(pan < 0) m_pan = 0.0;
    m_pan = pan;
}

//current sample
SAMPLE JGHSndSrc::getCurrSample()
{
    return currSample;
}

// synthesize the next buffer (stereo)
bool JGHSndSrc::synthesize2( SAMPLE * output, int numFrames )
{
    
    // fill
    for( int i = 0; i < numFrames; i++ )
    {
        // have we reached EOF
        if( m_rate >= 0 && m_playHead >= m_size ) return false;
        if( m_rate < 0 && m_playHead < 0 ) return false;
        
        // copy next frame
        output[i*m_channels] = sqrt(1-m_pan) * m_gain * m_buffer[(int)(m_playHead+.5)*m_channels] + output[i*m_channels];
        currSample = m_buffer[(int)(m_playHead+.5)*m_channels];
        if( m_channels == 2 )
            output[i*m_channels+1] =  sqrt(m_pan)  * m_gain * m_buffer[(int)(m_playHead+.5)*m_channels+1] + output[i*m_channels + 1];
        
        // increment by 'rate' frame
        m_playHead += m_rate;
    }
    
    return true;
}
