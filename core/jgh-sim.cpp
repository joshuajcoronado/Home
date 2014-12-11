//-----------------------------------------------------------------------------
// name: jgh-sim.cpp
// desc: jgh
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "jgh-sim.h"
#include "jgh-globals.h"
#include <iostream>
using namespace std;


// up to max fps steps per second, e.g., 1./60
#define STEPTIME (1.0 / getDesiredFrameRate())
// max sim step size in seconds
#define SIM_SKIP_TIME (.25)




//-------------------------------------------------------------------------------
// name: jghSim()
// desc: constructor
//-------------------------------------------------------------------------------
JGHSim::JGHSim()
{
    m_desiredFrameRate = 60;
    m_useFixedTimeStep = false;
    m_timeLeftOver = 0;
    m_simTime = 0;
    m_lastDelta = 0;
    m_first = true;
    m_isPaused = false;
}




//-------------------------------------------------------------------------------
// name: ~jghSim()
// desc: ...
//-------------------------------------------------------------------------------
JGHSim::~JGHSim()
{
    // nothing to do
}


//-------------------------------------------------------------------------------
// name: triggerUpdates()
// desc: trigger system wide update with time steps
//-------------------------------------------------------------------------------
void JGHSim::systemCascade()
{
    // get current time (once per frame)
    XGfx::getCurrentTime( true );
    
    // Timing loop
    YTimeInterval timeElapsed = XGfx::getCurrentTime() - m_simTime;
    m_simTime += timeElapsed;
    
    // special case: first update
    if( m_first )
    {
        // set time just enough for one update
        timeElapsed = STEPTIME;
        // set flag
        m_first = false;
    }
    
    // clamp it
    if( timeElapsed > SIM_SKIP_TIME )
        timeElapsed = SIM_SKIP_TIME;

    // update it
    // check paused
    if( !m_isPaused )
    {
        // update the world with a fixed timestep
        m_gfxRoot.updateAll( timeElapsed );
    }

    // redraw
    m_gfxRoot.drawAll();

    // set
    m_lastDelta = timeElapsed;
}





//-------------------------------------------------------------------------------
// pause the simulation
//-------------------------------------------------------------------------------
void JGHSim::pause() { m_isPaused = true; }
//-------------------------------------------------------------------------------
// resume the simulation
//-------------------------------------------------------------------------------
void JGHSim::resume() { m_isPaused = false; }
//-------------------------------------------------------------------------------
// get is paused
//-------------------------------------------------------------------------------
bool JGHSim::isPaused() const { return m_isPaused; }
//-------------------------------------------------------------------------------
// set desired frame rate
//-------------------------------------------------------------------------------
void JGHSim::setDesiredFrameRate( double frate ) { m_desiredFrameRate = frate; }
//-------------------------------------------------------------------------------
// get it
//-------------------------------------------------------------------------------
double JGHSim::getDesiredFrameRate() const { return m_desiredFrameRate; }
//-------------------------------------------------------------------------------
// get the timestep in effect (fixed or dynamic)
//-------------------------------------------------------------------------------
YTimeInterval JGHSim::delta() const
{ return m_lastDelta; }

