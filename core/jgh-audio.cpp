//-----------------------------------------------------------------------------
// name: jgh-audio.cpp
// desc: audio stuffs
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------
#include "jgh-audio.h"
#include "jgh-globals.h"
#include "jgh-sim.h"
#include "y-fft.h"
#include "y-waveform.h"
#include "y-fluidsynth.h"
#include <iostream>
#include "x-fun.h"
#include "y-score-reader.h"
#include <stdio.h>
#include "jgh-sndsrc.h"
#include <sndfile.h>
#include "jgh-pe.h"

using namespace std;

//Globals
double g_now;
XMutex g_mutex;

//-----------------------------------------------------------------------------
// name: audio_callback
// desc: audio callback
//-----------------------------------------------------------------------------
static void audio_callback( SAMPLE * buffer, unsigned int numFrames, void * userData )
{
    g_now += numFrames;
    
    //let's clear memory just to be safe
    memset( buffer, 0, sizeof(SAMPLE)*numFrames*XAudioIO::numChannels() );
    SAMPLE sum = 0;

    // num channels
    unsigned int channels = Globals::lastAudioBufferChannels;
    
    // zero out
    memset( Globals::lastAudioBuffer, 0,
           sizeof(SAMPLE)*Globals::lastAudioBufferFrames*channels );
    memset( Globals::lastAudioBufferMono, 0,
           sizeof(SAMPLE)*Globals::lastAudioBufferFrames );
    
    // copy to global buffer
    memcpy( Globals::lastAudioBuffer, buffer,
           sizeof(SAMPLE)*numFrames*channels );
    
    // copy to mono buffer
    for( int i = 0; i < numFrames; i++ )
    {
        // zero out
        sum = 0;
        // loop over channels
        for( int j = 0; j < channels; j++ )
        {
            // sum
            sum += buffer[i*channels + j];
        }
        // set
        Globals::lastAudioBufferMono[i] = sum / channels;
    }
    
    //window it
    for( int i = 0; i < numFrames; i++ )
    {
        // multiply
        Globals::lastAudioBufferMono[i] *= Globals::audioBufferWindow[i];
    }   

    play_script(buffer, numFrames);
}

//-----------------------------------------------------------------------------
// name: jgh_audio_init()
// desc: initialize audio system
//-----------------------------------------------------------------------------
bool jgh_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels )
{
    // log
    cout << "[Home]: initializing audio system..." << endl;
    // initialize
    if( !XAudioIO::init( 0, 0, srate, frameSize, channels, audio_callback, NULL ) )
    {
        // done
        return false;
    }

    // allocate
    Globals::lastAudioBuffer = new SAMPLE[frameSize*channels];
    // allocate mono buffer
    Globals::lastAudioBufferMono = new SAMPLE[frameSize];
    // allocate window buffer
    Globals::audioBufferWindow = new SAMPLE[frameSize];
    // set frame size (could have changed in XAudioIO::init())
    Globals::lastAudioBufferFrames = frameSize;
    // set num channels
    Globals::lastAudioBufferChannels = channels;
    
    // compute the window
    hanning( Globals::audioBufferWindow, frameSize );
  
    return true;
}


//-----------------------------------------------------------------------------
// name: vq_audio_start()
// desc: start audio system
//-----------------------------------------------------------------------------
bool jgh_audio_start()
{
    // start the audio
    if( !XAudioIO::start() )
    {
        // done
        return false;
    }
    
    return true;
}
