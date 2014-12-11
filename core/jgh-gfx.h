//-----------------------------------------------------------------------------
// name: jgh-gfx.h
// desc: graphics stuffs
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------
#ifndef __JGH_GFX_H__
#define __JGH_GFX_H__

#include "x-def.h"
#include "x-gfx.h"
#include <string>


// entry point for graphics
bool jgh_gfx_init( int argc, const char ** argv );
void jgh_gfx_loop();
void jgh_about();
void jgh_keys();
void jgh_help();
void jgh_usage();
void jgh_endline();
void jgh_line();
bool jgh_initTexture( const std::string & filename, XTexture * tex );
XTexture * jgh_loadTexture( const std::string & filename );




#endif
