//-----------------------------------------------------------------------------
// name: jgh-gfx.cpp
// desc: graphics stuff
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------
#include "jgh-gfx.h"
#include "jgh-globals.h"
#include "jgh-audio.h"
#include "jgh-sim.h"
#include "x-fun.h"
#include "x-gfx.h"
#include "x-loadlum.h"
#include "x-vector3d.h"
#include "jgh-pe.h"
#include <iostream>
#include <vector>
#include "y-fluidsynth.h" 
#include <string>
#include <png.h>
#include <cstdio>
using namespace std;
#define TEXTURE_LOAD_ERROR 0

//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
void idleFunc();
void displayFunc();
void reshapeFunc( int width, int height );
void keyboardFunc( unsigned char, int, int );
void mouseFunc( int button, int state, int x, int y );
void specialFunc( int key, int x, int y );

void initialize_graphics();
void initialize_simulation();
bool initialize_data();
void loadTextures();
bool checkTexDim( int dim );
void setupTexture( const char * filename, GLuint texobj,
                  GLenum minFilter, GLenum maxFilter,
                  int useMipMaps = 0 );

void renderBackground();
void blendPane();
void updateNodeEntities();
void renderNodeEntities();


//-----------------------------------------------------------------------------
// name: jgh_gfx_init( )
// desc: graphics/interaction entry point
//-----------------------------------------------------------------------------
bool jgh_gfx_init( int argc, const char ** argv )
{
#ifdef __APPLE__
    // save working dir
    char * cwd = getcwd( NULL, 0 );
    // set path
    // #ifdef __VQ_RELEASE__
#if 1
    // HACK: use this hard-coded path
    Globals::path = "./";
#else
    // store this globally
    Globals::path = cwd;
#endif
    // compute the datapath
    Globals::datapath = Globals::path + Globals::relpath;
#endif
    
    // initialize GLUT
    glutInit( &argc, (char **)argv );
    
#ifdef __APPLE__
    //restore working dir
    chdir( cwd );
    free( cwd );
#endif
    
    // print about
    jgh_about();
    jgh_endline();

    // double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    // initialize the window size
    glutInitWindowSize( Globals::windowWidth, Globals::windowHeight );
    // set the window postion
    glutInitWindowPosition( 100, 100 );
    // create the window
    glutCreateWindow( "Home.");
    // full screen
    if( Globals::fullscreen )
        glutFullScreen();
    
    // set the idle function - called when idle
    glutIdleFunc( idleFunc );
    // set the display function - called when redrawing
    glutDisplayFunc( displayFunc );
    // set the reshape function - called when client area changes
    glutReshapeFunc( reshapeFunc );
    // set the keyboard function - called on keyboard events
    glutKeyboardFunc( keyboardFunc );
    // set the mouse function - called on mouse stuff
    glutMouseFunc( mouseFunc );
    // for arrow keys, etc
	glutSpecialFunc (specialFunc );
    
    // do our own initialization
    initialize_graphics();
    // simulation
    initialize_simulation();
    // do data

    if( !initialize_data() )
    {
        // done
        return false;
    }
    
    // print keys
    jgh_endline();
    jgh_keys();
    jgh_line();
    jgh_endline();

    return true;
}




//-----------------------------------------------------------------------------
// name: jgh_gfx_loop( )
// desc: hand off to graphics loop
//-----------------------------------------------------------------------------
void jgh_gfx_loop()
{
    // let GLUT handle the current thread from here
    glutMainLoop();
}




//-----------------------------------------------------------------------------
// Name: initialize_graphics( )
// Desc: sets initial OpenGL states and initializes any application data
//-----------------------------------------------------------------------------
void initialize_graphics()
{
    // log
    cerr << "[Home]: initializing graphics system..." << endl;
    
    // reset time
    XGfx::resetCurrentTime();
    // set simulation speed
    XGfx::setDeltaFactor( 1.0f );
    // get the first
    XGfx::getCurrentTime( true );
    // random
    XFun::srand();
    
    // set the GL clear color - use when the color buffer is cleared
    glClearColor( Globals::bgColor.actual().x, Globals::bgColor.actual().y, Globals::bgColor.actual().z, 1.0f );
    // set the shading model to 'smooth'
    glShadeModel( GL_SMOOTH );
    // enable depth
    glEnable( GL_DEPTH_TEST );
    // set the front faces of polygons
    glFrontFace( GL_CCW );
    // set fill mode
    glPolygonMode( GL_FRONT_AND_BACK, Globals::fillmode );
    // enable lighting
    glEnable( GL_LIGHTING );
    // enable lighting for front
    glLightModeli( GL_FRONT_AND_BACK, GL_TRUE );
    // material have diffuse and ambient lighting
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    // enable color
    glEnable( GL_COLOR_MATERIAL );
    // normalize (for scaling)
    glEnable( GL_NORMALIZE );
    // line width
    glLineWidth( Globals::linewidth );
    
    // enable light 0
    glEnable( GL_LIGHT0 );
    
    // setup and enable light 1
    glLightfv( GL_LIGHT1, GL_AMBIENT, Globals::light1_ambient );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, Globals::light1_diffuse );
    glLightfv( GL_LIGHT1, GL_SPECULAR, Globals::light1_specular );
    glEnable( GL_LIGHT1 );
    
    // load textures
    loadTextures();
    
    // fog
    Globals::fog_mode[0] = 0;
    Globals::fog_mode[1] = GL_LINEAR;
    // fog_mode[1] = GL_EXP; fog_mode[2] = GL_EXP2;
    Globals::fog_filter = 0;
    Globals::fog_density = .04f;
    
    // fog color
    GLfloat fogColor[4]= {1.0f, 1.0f, 1.0f, 1.0f};
    // fog mode
    if( Globals::fog_filter ) glFogi(GL_FOG_MODE, Globals::fog_mode[Globals::fog_filter]);
    // set fog color
    glFogfv( GL_FOG_COLOR, fogColor );
    // fog density
    glFogf( GL_FOG_DENSITY, Globals::fog_density );
    // fog hint
    glHint( GL_FOG_HINT, GL_DONT_CARE );
    // fog start depth
    glFogf( GL_FOG_START, 1.0f );
    // fog end depth
    glFogf( GL_FOG_END, 10.5f );
    // enable
    if( Globals::fog_filter ) glEnable( GL_FOG );
    
    // check global flag
    if( Globals::fog )
    {
        // fog mode
        glFogi(GL_FOG_MODE, Globals::fog_mode[Globals::fog_filter]);
        // enable
        glEnable(GL_FOG);
    }
    else
    {
        // disable
        glDisable(GL_FOG);
    }
    
    // clear the color buffer once
    glClear( GL_COLOR_BUFFER_BIT );
}

//-----------------------------------------------------------------------------
// name: initialize_simulation( )
// desc: simulation setup
//-----------------------------------------------------------------------------
void initialize_simulation()
{
    // instantiate simulation
    Globals::sim = new JGHSim();
}

//-----------------------------------------------------------------------------
// name: initialize_data( )
// desc: load initial data
//-----------------------------------------------------------------------------
bool initialize_data()
{
    jgh_pe_init();

    return true;
}

//-----------------------------------------------------------------------------
// name: jgh_about()
// desc: ...
//-----------------------------------------------------------------------------
void jgh_about()
{
    jgh_line();
    fprintf( stderr, "[Home]: Home.\n" );
    jgh_line();
    fprintf( stderr, "   | by Joshua J Coronado\n" );
    fprintf( stderr, "   | Stanford University | CCRMA\n" );
    fprintf( stderr, "   | http://ccrma.stanford.edu/~jjcorona/\n" );
    fprintf( stderr, "   | version: %s\n", Globals::version.c_str() );
}


    

//-----------------------------------------------------------------------------
// name: jgh_keys()
// desc: ...
//-----------------------------------------------------------------------------
void jgh_keys()
{
    jgh_line();
    fprintf( stderr, "[Home]: run-time control\n" );
    jgh_line();
    fprintf( stderr, "  'h' - print this help message\n" );
    fprintf( stderr, "  's' - toggle fullscreen\n" );
    fprintf( stderr, "  [UP] / [DOWN] - move back and forth\n" );
    fprintf( stderr, "  [SPACEBAR] - play/pause play\n" );
    fprintf( stderr, "  [LEFT] / [RIGHT] - turn around\n" );
    fprintf( stderr, "  'q' - quit\n" );
}




//-----------------------------------------------------------------------------
// name: jgh_help()
// desc: ...
//-----------------------------------------------------------------------------
void jgh_help()
{
    jgh_endline();
    jgh_keys();
    jgh_endline();
}


//-----------------------------------------------------------------------------
// name: jgh_usage()
// desc: ...
//-----------------------------------------------------------------------------
void jgh_usage()
{
    jgh_line();
    fprintf( stderr, "[Home]: command line arguments\n" );
    jgh_line();
    fprintf( stderr, "usage: Home --[options] [name]\n" );
    fprintf( stderr, "   [options] = help | fullscreen" );
}




//-----------------------------------------------------------------------------
// name: jgh_endline()
// desc: ...
//-----------------------------------------------------------------------------
void jgh_endline()
{
    fprintf( stderr, "\n" );
}




//-----------------------------------------------------------------------------
// name: jgh_line()
// desc: ...
//-----------------------------------------------------------------------------
void jgh_line()
{
    fprintf( stderr, "---------------------------------------------------------\n" );
}




//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void reshapeFunc( int w, int h )
{
    // save the new window size
    Globals::windowWidth = w; Globals::windowHeight = h;
    // map the view port to the client area
    glViewport( 0, 0, w, h );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // set the matrix mode to modelview
    glMatrixMode( GL_PROJECTION  );
    // load the identity matrix
    glLoadIdentity( );
}




//-----------------------------------------------------------------------------
// Name: look( )
// Desc: ...
//-----------------------------------------------------------------------------
void look( )
{
    // go
   // Globals::fov.interp( XGfx::delta() );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // create the viewing frustum
    gluPerspective( Globals::fov.value, (GLfloat)Globals::windowWidth / (GLfloat)Globals::windowHeight, .005, 500.0 );
    
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity();
    // position the view point
    gluLookAt( Globals::cameraLocation.x, 0.0f, Globals::cameraLocation.z,
             Globals::cameraLocation.x + Globals::cameraDirection.x, 0.0f, Globals::cameraLocation.z + Globals::cameraDirection.z,
              0.0f, 1.0f , 0.0f ); // up is up
    
    // set the position of the lights

    glLightfv( GL_LIGHT0, GL_POSITION, Globals::light0_pos );
    glLightfv( GL_LIGHT1, GL_POSITION, Globals::light1_pos );
}




//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{

    // system keys (handled first)
    switch( key )
    {
        case 'q':
        {
            exit( 0 );
            break;
        }

        case ' ':
        {
            togglePlay();
            break;
        }
 
        case 'h':
        {
            jgh_help();
            break;
        }
        
        case 's':
        {
            if( !Globals::fullscreen )
            {
                Globals::lastWindowWidth = Globals::windowWidth;
                Globals::lastWindowHeight = Globals::windowHeight;
                glutFullScreen();
            }
            else
                glutReshapeWindow( Globals::lastWindowWidth, Globals::lastWindowHeight );
            
            Globals::fullscreen = !Globals::fullscreen;
            fprintf( stderr, "[Home]: fullscreen:%s\n", Globals::fullscreen ? "ON" : "OFF" );
            break;
        }
    
    }
    
    // check if something else is handling viewing
    bool handled = false;
    
    // post visualizer handling (if not handled)
    if( !handled )
    {
        switch( key )
        {
            case ']':
                JGHtestRight();

                break;
            case '[':
                JGHtestLeft();
                break;
           
            case '=':
                Globals::viewRadius.y = .975 * Globals::viewRadius.y;
                if( Globals::viewRadius.y < .001 ) Globals::viewRadius.y = .001;
                // fprintf( stderr, "[vismule]: view radius:%f->%f\n", Globals::viewRadius.x, Globals::viewRadius.y );
                break;
            case '-':
                Globals::viewRadius.y = 1.025 * Globals::viewRadius.y;
                // fprintf( stderr, "[vismule]: view radius:%f->%f\n", Globals::viewRadius.x, Globals::viewRadius.y );
                break;
            case '_':
            case '+':
                Globals::viewRadius.y = Globals::viewRadius.x + .7*(Globals::viewRadius.y-Globals::viewRadius.x);
                break;

        }
    }
    
    // do a reshape since viewEyeY might have changed
    reshapeFunc( Globals::windowWidth, Globals::windowHeight );
    // post redisplay
    glutPostRedisplay( );
}




//-----------------------------------------------------------------------------
// Name: mouseFunc( )
// Desc: handles mouse stuff
//-----------------------------------------------------------------------------
void mouseFunc( int button, int state, int x, int y )
{

    glutPostRedisplay( );
}

\

//-----------------------------------------------------------------------------
// Name: specialFunc( )
// Desc: handles arrow stuff
//-----------------------------------------------------------------------------
void specialFunc( int key, int x, int y )
{
    // check
    bool handled = false;
    
    // if not handled
    if( !handled )
    {
        float fraction = 0.1f;

        switch (key) {
            case GLUT_KEY_LEFT :
                Globals::rotationAngle -= 0.05f;
                Globals::cameraDirection.x = sin(Globals::rotationAngle);
                Globals::cameraDirection.z = -cos(Globals::rotationAngle);
                break;
            case GLUT_KEY_RIGHT :
                Globals::rotationAngle += 0.05f;
                Globals::cameraDirection.x = sin(Globals::rotationAngle);
                Globals::cameraDirection.z = -cos(Globals::rotationAngle);
                break;
            case GLUT_KEY_UP :
                if(areWeInsideTheHouse(Vector3D(Globals::cameraLocation.x + Globals::cameraDirection.x * fraction, Globals::cameraLocation.y, Globals::cameraLocation.z + Globals::cameraDirection.z * fraction)))
                {
                    Globals::cameraLocation.x += Globals::cameraDirection.x * fraction;
                    Globals::cameraLocation.z += Globals::cameraDirection.z * fraction;
                }
                break;
            case GLUT_KEY_DOWN :

                if(areWeInsideTheHouse(Vector3D(Globals::cameraLocation.x - Globals::cameraDirection.x * fraction, Globals::cameraLocation.y, Globals::cameraLocation.z - Globals::cameraDirection.z * fraction)))
                {
                    Globals::cameraLocation.x -= Globals::cameraDirection.x * fraction;
                    Globals::cameraLocation.z -= Globals::cameraDirection.z * fraction;
                }
                break;
        }
    }
}




//-----------------------------------------------------------------------------
// Name: idleFunc( )
// Desc: callback from GLUT
//-----------------------------------------------------------------------------
void idleFunc( )
{
    // render the scene
    glutPostRedisplay( );
}




//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void displayFunc( )
{

    XGfx::getCurrentTime( true );

    // update
    Globals::bgColor.interp( XGfx::delta() );
    Globals::blendAlpha.interp( XGfx::delta() );
    
    // clear or blend
    if( Globals::blendScreen && Globals::blendAlpha.value > .0001 )
    {
        // clear the depth buffer
        glClear( GL_DEPTH_BUFFER_BIT );
        // blend screen
        blendPane();
    }
    else
    {
        // set the GL clear color - use when the color buffer is cleared
        glClearColor( Globals::bgColor.actual().x, Globals::bgColor.actual().y, Globals::bgColor.actual().z, 1.0f );
        // clear the color and depth buffers
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    }
    
    // enable depth test
    glEnable( GL_DEPTH_TEST );
    
    // save state
    glPushMatrix();
    
    //Globals::cameraLocation.interp(XGfx::delta());
    look();
    

    Globals::sim->systemCascade();
    
    // pop state
    glPopMatrix();
    
   // draw any HUD here
   // Globals::hud->project();
   // Globals::hud->updateAll( Globals::sim->delta() );
   // Globals::hud->drawAll();
    
    // flush gl commands
    glFlush();
    // swap the buffers
    glutSwapBuffers();
}




//-----------------------------------------------------------------------------
// name: blendPane()
// desc: blends a pane into the current scene
//-----------------------------------------------------------------------------
void blendPane()
{
    // enable blending
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    // disable lighting
    glDisable( GL_LIGHTING );
    // disable depth test
    glDisable( GL_DEPTH_TEST );
    // blend in a polygon
    glColor4f( Globals::bgColor.actual().x, Globals::bgColor.actual().y, Globals::bgColor.actual().z, Globals::blendAlpha.value );

    GLfloat h = 10;
    GLfloat d = -1;
    
    // draw the polyg
    glBegin( GL_QUADS );
    glVertex3f( -h, -h, d );
    glVertex3f( h, -h, d );
    glVertex3f( h, h, d );
    glVertex3f( -h, h, d );
    glEnd();
    
    // enable lighting
    glEnable( GL_LIGHTING );
    // enable depth test
    glEnable( GL_DEPTH_TEST );
    // disable blending
    glDisable( GL_BLEND );
}




//-----------------------------------------------------------------------------
// name: renderBackground()
// desc: ...
//-----------------------------------------------------------------------------
void renderBackground()
{
    // save the current matrix
    glPushMatrix( );
    
    // restore
    glPopMatrix( );
}




//-------------------------------------------------------------------------------
// name: loadTexture()
// desc: load textures
//-------------------------------------------------------------------------------
void loadTextures()
{
    char filename[256];
    GLenum minFilter, maxFilter;
    int i;
    
    // log
    fprintf( stderr, "[Home]: loading textures...\n" );
    
    // set store alignment
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    
    // set filter types
    minFilter = GL_LINEAR;
    maxFilter = GL_LINEAR;
    
// load tng flares
    for( i = JOSH_TEX; i <= H3_LEFT; i++ )
    {   
    
        glGenTextures( 1, &(Globals::textures[i]) );
        string file = "test";
        switch(i)
        {
            case BATH_TOP:
                file = "BATH_TOP";
                break;
            case BATH_BOTTOM:
                file = "BATH_BOTTOM";
                break;
            case ALEX_TOP:
                file = "ALEX_TOP";
                break;
            case ALEX_BOTTOM:
                file = "ALEX_BOTTOM";
                break;
            case H3_LEFT:
                file = "H3_LEFT";
                break;

            case JOSH_TEX:
                file = "JOSH_TEX";
                break;
            case NICOLE_TEX:
                file = "NICOLE_TEX";
                break;
            case SIDD_TEX:
                file = "SIDD_TEX";
                break;
            case SAM_TEX:
                file = "SAM_TEX";
                break;
            case ROD_TEX:
                file = "ROD_TEX";
                break;
            case ENTRANCE_ROOM_TOP_TEX:
                file = "ENTRANCE_ROOM_TOP_TEX";
                break;
            case ENTRANCE_ROOM_FRONT_TEX:
                file = "ENTRANCE_ROOM_FRONT_TEX";
                break;
            case ENTRANCE_ROOM_RIGHT_TEX:
                file = "ENTRANCE_ROOM_RIGHT_TEX";
                break;
            case ENTRANCE_ROOM_BOTTOM_TEX:
                file = "ENTRANCE_ROOM_BOTTOM_TEX";
                break;
            case OFFICE_ROOM_LEFT_TEX:
                file = "OFFICE_ROOM_LEFT_TEX";
                break;
            case OFFICE_ROOM_RIGHT_TEX:
                file = "OFFICE_ROOM_RIGHT_TEX";
                break;
            case OFFICE_ROOM_TOP_TEX:
                file = "OFFICE_ROOM_TOP_TEX";
                break;
            case OFFICE_ROOM_BOTTOM_TEX:
                file = "OFFICE_ROOM_BOTTOM_TEX";
                break;
            case LIVING_ROOM_BACK_TEX:
                file = "LIVING_ROOM_BACK_TEX";
                break;
            case LIVING_ROOM_TOP_TEX:
                file = "LIVING_ROOM_TOP_TEX";
                break;
            case LIVING_ROOM_BOTTOM_TEX:
                file = "LIVING_ROOM_BOTTOM_TEX";
                break;
            case LIVING_ROOM_RIGHT_TEX:
                file = "LIVING_ROOM_RIGHT_TEX";
                break;
            case LIVING_ROOM_LEFT_TEX:
                file = "LIVING_ROOM_LEFT_TEX";
                break; 
            case KITCHEN_ENTRANCE_FRONT_TEX:
                file = "KITCHEN_ENTRANCE_FRONT_TEX";
                break;
            case KITCHEN_ENTRANCE_LEFT_TEX:
                file = "KITCHEN_ENTRANCE_LEFT_TEX";
                break;
            case KITCHEN_ENTRANCE_TOP_TEX:
                file = "KITCHEN_ENTRANCE_TOP_TEX";
                break;
            case KITCHEN_ENTRANCE_BOTTOM_TEX:
                file = "KITCHEN_ENTRANCE_BOTTOM_TEX";
                break;
            case KITCHEN_BACK_TEX:
                file = "KITCHEN_BACK_TEX";
                break;
            case KITCHEN_TOP_TEX:
                file = "KITCHEN_TOP_TEX";
                break;
            case KITCHEN_BOTTOM_TEX:
                file = "KITCHEN_BOTTOM_TEX";
                break;
            case HALL1_BACK_TEX:
                file = "HALL1_BACK_TEX";
                break;
            case HALL1_TOP_TEX:
                file = "HALL1_TOP_TEX";
                break;
            case  HALL1_BOTTOM_TEX:
                file = "HALL1_BOTTOM_TEX";
                break;
            case HALL2_BACK_TEX:
                file = "HALL2_BACK_TEX";
                break;
            case HALL2_TOP_TEX:
                file = "HALL2_TOP_TEX";
                break;
            case  HALL2_BOTTOM_TEX:
                file = "HALL2_BOTTOM_TEX";
                break;
            case HALL3_BACK_TEX:
                file = "HALL3_BACK_TEX";
                break;
            case HALL3_TOP_TEX:
                file = "HALL3_TOP_TEX";
                break;
            case  HALL3_BOTTOM_TEX:
                file = "HALL3_BOTTOM_TEX";
                break;
            case ALEX_ROOM_FRONT_TEX:
                file = "ALEX_ROOM_FRONT_TEX";
                break;
            case ALEX_ROOM_TOP_TEX:
                file = "ALEX_ROOM_TOP_TEX";
                break;
            case ALEX_ROOM_BOTTOM_TEX:
                file = "ALEX_ROOM_BOTTOM_TEX";
                break;
            case ALEX_ROOM_LEFT_TEX:
                file = "ALEX_ROOM_LEFT_TEX";
                break;
            case BATH_ROOM_FRONT_TEX:
                file = "BATH_ROOM_FRONT_TEX";
                break;
            case BATH_ROOM_TOP_TEX:
                file = "BATH_ROOM_TOP_TEX";
                break;
            case BATH_ROOM_BOTTOM_TEX:
                file = "BATH_ROOM_BOTTOM_TEX";
                break;
            case BATH_ROOM_LEFT_TEX:
                file = "BATH_ROOM_LEFT_TEX";
                break;
            case JOSH_ROOM_FRONT_TEX:
                file = "JOSH_ROOM_FRONT_TEX";
                break;
            case JOSH_ROOM_TOP_TEX:
                file = "JOSH_ROOM_TOP_TEX";
                break;
            case JOSH_ROOM_BOTTOM_TEX:
                file = "JOSH_ROOM_BOTTOM_TEX";
                break;
            case JOSH_ROOM_LEFT_TEX:
                file = "JOSH_ROOM_LEFT_TEX";
                break;
            case NICOLE_ROOM_FRONT_TEX:
                file = "NICOLE_ROOM_FRONT_TEX";
                break;
            case NICOLE_ROOM_BACK_TEX:
                file = "NICOLE_ROOM_BACK_TEX";
                break;
            case NICOLE_ROOM_TOP_TEX:
                file = "NICOLE_ROOM_TOP_TEX";
                break;
            case  NICOLE_ROOM_BOTTOM_TEX:
                file = "NICOLE_ROOM_BOTTOM_TEX";
                break;
            case  NICOLE_ROOM_LEFT_TEX:
                file = "NICOLE_ROOM_LEFT_TEX";
                break;
            default:
                file = "JOSH_TEX";
        }
        sprintf( filename, "%s%s.png", Globals::datapath.c_str(), file.c_str() );
        setupTexture( filename, Globals::textures[i], minFilter, maxFilter );
    }

}


//Taken directly from ! https://en.wikibooks.org/wiki/OpenGL_Programming/Intermediate/Textures
//thanks! man!
GLuint loadPNG(const string filename, int &width, int &height , GLuint texture) 
 {
   //header for testing if it is a png
   png_byte header[8];
 
   //open file as binary
   FILE *fp = fopen(filename.c_str(), "rb");
   if (!fp) {
     return TEXTURE_LOAD_ERROR;
   }
 
   //read the header
   fread(header, 1, 8, fp);
 
   //test if png
   int is_png = !png_sig_cmp(header, 0, 8);
   if (!is_png) {
     fclose(fp);
     return TEXTURE_LOAD_ERROR;
   }
 
   //create png struct
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
       NULL, NULL);
   if (!png_ptr) {
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }
 
   //create png info struct
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
     png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }
 
   //create png info struct
   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }
 
   //png error stuff, not sure libpng man suggests this.
   if (setjmp(png_jmpbuf(png_ptr))) {
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return (TEXTURE_LOAD_ERROR);
   }
 
   //init png reading
   png_init_io(png_ptr, fp);
 
   //let libpng know you already read the first 8 bytes
   png_set_sig_bytes(png_ptr, 8);
 
   // read all the info up to the image data
   png_read_info(png_ptr, info_ptr);
 
   //variables to pass to get info
   int bit_depth, color_type;
   png_uint_32 twidth, theight;
 
   // get info about png
   png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
       NULL, NULL, NULL);
 
   //update width and height based on png info
   width = twidth;
   height = theight;
 
   // Update the png info struct.
   png_read_update_info(png_ptr, info_ptr);
 
   // Row size in bytes.
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
 
   // Allocate the image_data as a big block, to be given to opengl
   png_byte *image_data = new png_byte[rowbytes * height];
   if (!image_data) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     return TEXTURE_LOAD_ERROR;
   }
 
   //row_pointers is for pointing to image_data for reading the png with libpng
   png_bytep *row_pointers = new png_bytep[height];
   if (!row_pointers) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     delete[] image_data;
     fclose(fp);
     return TEXTURE_LOAD_ERROR;
   }
   // set the individual row_pointers to point at the correct offsets of image_data
   for (int i = 0; i < height; ++i)
     row_pointers[height - 1 - i] = image_data + i * rowbytes;
 
   //read the png into image_data through row_pointers
   png_read_image(png_ptr, row_pointers);
 
   //Now generate the OpenGL texture object

   glBindTexture(GL_TEXTURE_2D, texture);
   glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, width, height, 0,
       GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 
   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   delete[] image_data;
   delete[] row_pointers;
   fclose(fp);
 
   return texture;
 }



//--------------------------------------------------------------------------------
// name: setupTexture()
// desc: ...
//--------------------------------------------------------------------------------
void setupTexture( const char * filename, GLuint texobj,
                  GLenum minFilter, GLenum maxFilter, int useMipMaps )
{
    int width = 0, height = 0;   
    loadPNG(filename,width, height, texobj); 
}




//-----------------------------------------------------------------------------
// name: checkTexDim( )
// desc: checks to see if a dim is a valid opengl texture dimension
//-----------------------------------------------------------------------------
bool checkTexDim( int dim )
{
    if( dim < 0 ) 
        return false;
    
    int i, count = 0;
    
    // count bits
    for( i = 0; i < 31; i++ )
        if( dim & ( 0x1 << i ) )
            count++;
    
    // this is true only if dim is power of 2
    return count == 1;
}
