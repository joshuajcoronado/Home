//-----------------------------------------------------------------------------
// name: jgh-audio.h
// desc: audio stuff
//
// author: Joshua J Coronado(jjcorona@ccrma.stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------
#ifndef __JGH_AUDIO_H__
#define __JGH_AUDIO_H__

// init audio
bool jgh_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels );
// start audio
bool jgh_audio_start();

#endif
