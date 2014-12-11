//-----------------------------------------------------------------------------
// name: jgh-entity.cpp
// desc: entities for bokeh visualization
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "jgh-entity.h"
#include "jgh-globals.h"
#include "x-fun.h"
#include "jgh-audio.h"
#include "y-entity.h"
#include "jgh-sim.h"

using namespace std;


// texture coordinates
static const GLshort g_coord[ ] = { 0, 0, 1, 0, 0, 1, 1, 1 };

//-------------------------------------------------------------------------------
// name: setTexture()
// desc: ...
//-------------------------------------------------------------------------------
void JGHEntity::setTexture( int _texture )
{
    texture = _texture;
}

//-------------------------------------------------------------------------------
// name: enableTex
// desc: enables texture
//-------------------------------------------------------------------------------
void JGHEntity::enableTex()
{
    glPushMatrix();
    //texture = Globals::textures[1] ;
    // enable texture
    glEnable( GL_TEXTURE_2D );
    // set blend function
    //glBlendFunc( GL_ONE, GL_ONE );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    // enable blend
    glEnable( GL_BLEND );

    // bind to texture
    glBindTexture( GL_TEXTURE_2D, Globals::textures[this->texture] );

    // set texture coordinates
    glTexCoordPointer( 2, GL_SHORT, 0, g_coord );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );

}

//-------------------------------------------------------------------------------
// name: diableTex
// desc: disables texture
//-------------------------------------------------------------------------------
void JGHEntity::disableTex()
{
    // disable texture
    glDisable( GL_TEXTURE_2D );
    // disable blend
    glDisable( GL_BLEND );

    // disable client states
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glPopMatrix();
}





