//-----------------------------------------------------------------------------
// name: jgh-entity.h
// desc: entities for visquin visualization
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __JGH_ENTITY_H__
#define __JGH_ENTITY_H__

#include "y-entity.h"
#include "x-buffer.h"
#include "jgh-globals.h"
#include <array>
#include "glm.h"
#include <vector>

//-----------------------------------------------------------------------------
// name: class jghTeapot
// desc: for testing
//-----------------------------------------------------------------------------
class JGHEntity : public YEntity
{
public:
    void setTexture( int _texture );
    GLuint texture;
    void enableTex();
    void disableTex();
};



#endif