/*
 * main.cpp
 * Project 4
 * by Matthew Meyn
 * based on the template...
 */


// TODO:
//
// * Add different textures to landscape, unless it looks ugly
// * Try to find some more meshes to add to landscape
// * Try to fix texture seams in landscape
//



//	A basic OpenGL / GLUT program
//
//	Author:			Joe Graphics


#include "includes.hpp"
#include "consts.hpp"
#include "params.hpp"
#include "setlight.hpp"
#include "xtrRead.hpp"
#include "bmpLoader.hpp"
#include "mesh.hpp"

#include <deque>


// non-constant global variables:

int		ActiveButton;			// current button that is down
int		AxesOn;					// != 0 means to draw the axes
bool    LookFollows;            // true means default: camera follows helicopter
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
float   FollowScale;
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
bool    Freeze;                 // true means pause the animation
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees
float   Time;


// controls stuff

bool    ButtonDown;
int     ButtonDownX, ButtonDownY;
int     InputX, InputY;


// helicopter stuff

float   HeliAltitude;           // desired altitude of helicopter


std::deque<point> heliPositions;

point   HeliForwardVec;         // pointing in helicopter forward direction

// mesh stuff

Mesh*   Helicopter;
Mesh*   HeliBlades;


// textures

GLuint  HeliTexture;            // the helicopter's texture handle
GLuint  GroundTexture;          // ground texture handle
GLuint  SnowTexture;
GLuint  TreeTexture;
GLuint  IceTexture;


// landscape stuff

int     NumElevLong, NumElevLat;
point** LandscapeGrid;                  // 2d array of elevation data
point** LandscapeNormals;               // corresponds to LandscapeGrid
point** LandscapePoints;                // 2d array of points to draw
point** LandscapeVertexNormals;         // corresponds to LandscapePoints
float   LandscapeHeightMin, LandscapeMaxHeightMax;

point*  TreePoints;
bool**  OccupiedLocations;


// display lists

GLuint  LandscapeList;
GLuint  HelicopterList;
GLuint  BladeList;              // helicopter blade list
GLuint  AxesList;
GLuint  TreeList;
GLuint  PancakeList;

MouseAction WhichMouseAction = ROTATE;


// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void	Axes( float );
void    Cross( float v1[3], float v2[3], float vout[3] );
point   FindCross(point p1, point p2);
void	HsvRgb( float[3], float [3] );

void    ChooseTreePoints();
void    ConstructLandscape( );

void    DrawLandscape( );

void    DrawTrees();
void    DrawTree(point p);
void    DrawHelicopter( );
//point   FindNormal(int lon, int lat);       // find normal of point in LandscapeGrid
point   FindNormal(point** grid, int lon, int lat);
void    InitListsFromMesh( );
void    LandscapeVertex(int lon, int lat, int texTileWidth);
void    LoadLandscape( );
void    MakeLandscapeList( );
GLuint  MakeListFromMesh( Mesh* mesh );
GLuint  SetupTexture( char* path );
void    UpdateHelicopterPosition();


/* ------------------------------------------------------------------------------- */

// main program:

int
main( int argc, char *argv[ ] )
{
    glutInit( &argc, argv );    // turn on the glut package:
                                // (do this before checking argc and argv since
                                // it might pull some command line arguments out)
    
    InitGraphics( );            // setup all the graphics stuff:
    
    InitLists( );               // create the display structures that will not change
    
    Reset( );                   // init all the global variables used by Display( )
                                // this will also post a redisplay
    
    InitMenus( );               // setup all the user interface stuff
    
    
    // draw the scene once and wait for some interaction:
    // (this will never return)
    
    glutSetWindow( MainWindow );
    glutMainLoop( );
    
    return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

void
Animate( )
{
    int ms = glutGet( GLUT_ELAPSED_TIME );
    ms %= MS_PER_CYCLE;
    Time = (float)ms / (float)MS_PER_CYCLE;		// [0.,1.)
    
    // force a call to Display( ) next time it is convenient:
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}



void
MakePancakeList() {
    
    PancakeList = glGenLists(1);
    glNewList(PancakeList, GL_COMPILE);
    
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0., 1., 0.);
    glTexCoord2f(0., 0.);
    glVertex3f(0., WATER_LEVEL - 100., 0.);
    for(int i = 0; i <= 500; i++) {
        float ang = 360.f * (float)i / 500.f;
        glNormal3f(0., 1., 0.);
        glTexCoord2f(100.f * cosf(ang), 100.f * sinf(ang));
        glVertex3f(100000.f * cosf(ang), WATER_LEVEL - 100., 100000.f * sinf(ang));
    }
    glEnd();
    glEndList();
}



// draw the complete scene:

void
Display( )
{
    // set which window we want to do the graphics into:
    
    glutSetWindow( MainWindow );
    
    // erase the background:
    
    glDrawBuffer( GL_BACK );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable( GL_DEPTH_TEST );
    
    // set the viewport

    GLsizei wx = glutGet( GLUT_WINDOW_WIDTH );
    GLsizei wy = glutGet( GLUT_WINDOW_HEIGHT );
    glViewport(0., 0., wx, wy);
    
    // set the viewing volume:
    
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    if( WhichProjection == ORTHO )
        glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
    else
        gluPerspective( 90., (double)wx / (double)wy,	0.1, 10000. );
    
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );
    
    // set the eye position, look-at position, and up-vector:
    
    point viewpoint;
    
    UpdateHelicopterPosition();
    
    // get viewpoint from Helicopter forward vector
    // add some height above helicopter y coord. Ugly solution, but...
    
    point c2hVec { HeliForwardVec.x, 0., HeliForwardVec.z };
    c2hVec.y = 0.35;
    
    point HeliCurrentPosition = heliPositions.front();
    viewpoint.x = HeliCurrentPosition.x + CAM_FOLLOW_DIST * c2hVec.x;
    viewpoint.z = HeliCurrentPosition.z + CAM_FOLLOW_DIST * c2hVec.z;
    viewpoint.y = HeliCurrentPosition.y + CAM_FOLLOW_DIST * c2hVec.y;
    
    // set look at
    
    if(LookFollows) {
        gluLookAt(viewpoint.x, viewpoint.y, viewpoint.z,
                  HeliCurrentPosition.x, HeliCurrentPosition.y, HeliCurrentPosition.z,
                  0., 1., 0.);
    }
    else {
        gluLookAt( 0., 10., -25.,     0., 0., 0.,     0., 1., 0. );
    }
    
    // rotate the scene:
    
    glRotatef( (GLfloat)Yrot, 0., 1., 0. );
    glRotatef( (GLfloat)Xrot, 1., 0., 0. );

    // uniformly scale the scene:
    
    if(LookFollows) {
        Scale = DEFAULT_FOLLOW_SCALE;
    }
    else {
        if( Scale < MINSCALE ) {
            Scale = MINSCALE;
        }
    }
    
    glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );
    
    // set the fog parameters:
    
    if( DepthCueOn != 0 )
    {
        glFogi( GL_FOG_MODE, FOGMODE );
        glFogfv( GL_FOG_COLOR, FOGCOLOR );
        glFogf( GL_FOG_DENSITY, FOGDENSITY );
        glFogf( GL_FOG_START, FOGSTART );
        glFogf( GL_FOG_END, FOGEND );
        glEnable( GL_FOG );
    }
    else
    {
        glDisable( GL_FOG );
    }
    
    
    // possibly draw the axes:
    
    if( AxesOn != 0 )
    {
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_LIGHTING);
        glColor3fv( &Colors[WhichColor][0] );
        glCallList( AxesList );
    }
    
    // since we are using glScalef( ), be sure normals get unitized:
    
    glEnable( GL_NORMALIZE );
    
    glShadeModel(GL_SMOOTH);
    
    
    // draw lights
    
    glDisable(GL_LIGHTING);
    glPushMatrix();
        glTranslatef(L0x, L0y, L0z);
        glColor3f(L0r, L0g, L0b);
        glutSolidSphere(100., 10., 10.);
    glPopMatrix();

    
    
    // draw "water"
    
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    
    SetMaterial( 7., 8., 1., 2. );
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, IceTexture);
    
    glBegin(GL_QUADS);
    glTexCoord2f(0., 0.);
    glVertex3f(LandscapePoints[0][0].x, WATER_LEVEL, LandscapePoints[0][0].z);
    glTexCoord2f(0., 10.);
    glVertex3f(LandscapePoints[NumElevLong*LANDSCAPE_RES - 1][0].x,
               WATER_LEVEL,
               LandscapePoints[NumElevLong*LANDSCAPE_RES - 1][0].z);
    glTexCoord2f(10., 10.);
    glVertex3f(LandscapePoints[NumElevLong*LANDSCAPE_RES - 1][NumElevLat*LANDSCAPE_RES - 1].x,
               WATER_LEVEL,
               LandscapePoints[NumElevLong*LANDSCAPE_RES - 1][NumElevLat*LANDSCAPE_RES - 1].z);
    glTexCoord2f(10., 0.);
    glVertex3f(LandscapePoints[0][NumElevLat*LANDSCAPE_RES - 1].x,
               WATER_LEVEL,
               LandscapePoints[0][NumElevLat*LANDSCAPE_RES - 1].z);
    glEnd();
    
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, SnowTexture);
    SetMaterial(1., .9, 1., 0.);
    glColor3f(0.9, 0.925, 1.);
    glCallList(PancakeList);
    
    
    glColor3f(L0r, L0g, L0b);
    glEnable(GL_LIGHTING);
    
    // draw the landscape
    
    glBindTexture(GL_TEXTURE_2D, SnowTexture);
    SetMaterial(1., .9, 1., 0.);
    glCallList(LandscapeList);
    
    


    
    // draw the helicopter
    
    DrawHelicopter();

    
    // draw some trees
    
    DrawTrees();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    
    // if debugging, show percent of animation cycle
    
    if(DebugOn) {
        char buf[128];
        sprintf(buf, "Scale = %.2f", Scale);
        char buf2[128];
        sprintf(buf2, "Xrot = %.2f, Yrot = %.2f", Xrot, Yrot);
        char buf3[128];
        sprintf(buf3, "Percent Anim Cycle: %.2f", Time * 100.f);
        
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0., 100., 0., 100.);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColor3f(1.,1.,1.);
        DoRasterString(2.5, 2.5, 0., buf);
        DoRasterString(2.5, 5., 0., buf2);
        DoRasterString(2.5, 7.5, 0., buf3);
    }
    
    
    
    // swap the double-buffered framebuffers:
    
    glutSwapBuffers( );
    
    
    // be sure the graphics buffer has been sent:
    // note: be sure to use glFlush( ) here, not glFinish( ) !
    
    glFlush( );
}




void DrawTrees() {
    for(int i = 0; i < NUM_TREES; i++) {
        DrawTree(TreePoints[i]);
    }
}



void
DrawTree(point p) {
    
        glPushMatrix();
        
    
            glTranslatef(p.x, p.y, p.z);
    
            glScalef(TREE_SCALE, TREE_SCALE, TREE_SCALE);
    
            SetMaterial( 0.7, 0.8, 0.7, 0. );
            glShadeModel(GL_SMOOTH);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, TreeTexture);
            
            glCallList(TreeList);
            
            glDisable(GL_TEXTURE_2D);
        
        glPopMatrix();

}



void
UpdateHelicopterPosition() {
    
    if(InputX > HELI_TURN_MAX) {
        InputX = HELI_TURN_MAX;
    }
    if(InputX < -HELI_TURN_MAX) {
        InputX = -HELI_TURN_MAX;
    }
    
    if(InputY > HELI_CLIMB_MAX) {
        InputY = HELI_CLIMB_MAX;
    }
    if(InputY < -HELI_CLIMB_MAX) {
        InputY = -HELI_CLIMB_MAX;
    }
    
    float dRad = HELI_TURN_FAC * (float)InputX;
    
    if(HeliAnimRadius - HeliAnimRadiusMin < HeliAnimRadiusMin && dRad < 0.) {
        dRad *= ((HeliAnimRadius - HeliAnimRadiusMin) / HeliAnimRadiusMin);
    }
    
    HeliAltitude += HELI_CLIMB_FAC * (float)InputY;
    //HeliAnimRadius += HELI_TURN_FAC * (float)InputX;
    HeliAnimRadius += dRad;
    
    if(HeliAnimRadius < HeliAnimRadiusMin) {
        HeliAnimRadius = HeliAnimRadiusMin;
    }
    
    if(HeliAnimRadius > HeliAnimRadiusMax) {
        HeliAnimRadius = HeliAnimRadiusMax;
    }

    
    float percent = HeliAnimRevs * Time;
    float r = HeliAnimRadius;
    
    point HeliCurrentPosition;
    
    float x = HeliCurrentPosition.x = r * cosf(2. * M_PI * percent);
    float z = HeliCurrentPosition.z = -r * sinf(2. * M_PI * percent);
    float y = HeliCurrentPosition.y = HeliAltitude;
    
    float dx, dy, dz;
     
    if(heliPositions.size() > HELI_LAG_FRAMES) {
        heliPositions.pop_back();
    }
    
    point prevHeliPosition;
    if(heliPositions.size() > 0) {
        point p = heliPositions.back();
        prevHeliPosition.x = p.x;
        prevHeliPosition.y = p.y;
        prevHeliPosition.z = p.z;
    } else {
        prevHeliPosition.x = HeliCurrentPosition.x;
        prevHeliPosition.y = HeliCurrentPosition.y;
        prevHeliPosition.z = HeliCurrentPosition.z;
    }
    
    float ds = sqrtf(   (x - prevHeliPosition.x) * (x - prevHeliPosition.x) +
               (y - prevHeliPosition.y) * (y - prevHeliPosition.y) +
               (z - prevHeliPosition.z) * (z - prevHeliPosition.z) );

    dx = x - prevHeliPosition.x;
    dy = y - prevHeliPosition.y;
    dz = z - prevHeliPosition.z;
    
    HeliForwardVec.x = -dx / ds;
    HeliForwardVec.y = -dy / ds;
    HeliForwardVec.z = -dz / ds;
    
    heliPositions.push_front(HeliCurrentPosition);
    
}



void
DrawHelicopter( ) {
    
    SetMaterial( 1., 1., 1., 5. );
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, HeliTexture);
    
    glPushMatrix();
    
        // animate and draw the helicopter
        
        float percent = HeliAnimRevs * Time;
        glRotatef(360.f * percent, 0.f, 1.f, 0.f);
        float r = HeliAnimRadius;
        glTranslatef(r, 0.f, 0.f);
        
        glTranslatef(0., HeliAltitude, 0.);
    
    if(heliPositions.size() > 0) {
        
        // tilt the helicopter forwards or backwards in it's going down or up
        
        point prevPos = heliPositions.back();
        float dy = heliPositions.front().y - prevPos.y;
        glRotatef(TILT_X_FAC * dy / HELI_LAG_FRAMES, 1., 0., 0.);
        
        // left or right if turning
        
        float prevR = sqrtf(prevPos.x * prevPos.x + prevPos.z * prevPos.z);
        float dr = HeliAnimRadius - prevR;
        
        float addTiltYFac = (HeliAnimRadiusMax - HeliAnimRadius) /
                            (HeliAnimRadiusMax - HeliAnimRadiusMin);
        addTiltYFac *= addTiltYFac;
        
        if(dr < 0.) {
            addTiltYFac *= 3.;
        }
        else {
            addTiltYFac *= 2.;
        }
        
        glRotatef(addTiltYFac * TILT_Y_FAC * -dr / HELI_LAG_FRAMES, 0., 1., 0.);
        glRotatef(TILT_Z_FAC * -dr / HELI_LAG_FRAMES, 0., 0., 1.);
    
    }
    
        // add a slight tilt toward the center of the circle
    
        glRotatef(TILT_Z_AUTO_MAX * (1. - HeliAnimRadius / (1.25 * HeliAnimRadiusMax)),
                  0., 0., 1.);
    
        glCallList( HelicopterList );

        // draw the blades
         
        glShadeModel(GL_FLAT);
        SetMaterial(1., 1., 1., 9.);
        glRotatef(HeliBladeRevsFac * 360.f * Time, 0., 1., 0.);
        glCallList(BladeList);
    
    
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}



// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics( )
{
    // request the display modes:
    // ask for red-green-blue-alpha color, double-buffering, and z-buffering:
    
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    
    // set the initial window configuration:
    
    glutInitWindowPosition( 0, 0 );
    glutInitWindowSize( INIT_WINDOW_SIZE_W, INIT_WINDOW_SIZE_H );
    
    // open the window and set its title:
    
    MainWindow = glutCreateWindow( WINDOWTITLE );
    glutSetWindowTitle( WINDOWTITLE );
    
    // set the framebuffer clear values:
    
    glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );
    
    // setup the callback functions:
    // DisplayFunc -- redraw the window
    // ReshapeFunc -- handle the user resizing the window
    // KeyboardFunc -- handle a keyboard input
    // MouseFunc -- handle the mouse button going down or up
    // MotionFunc -- handle the mouse moving with a button down
    // PassiveMotionFunc -- handle the mouse moving with a button up
    // VisibilityFunc -- handle a change in window visibility
    // EntryFunc	-- handle the cursor entering or leaving the window
    // SpecialFunc -- handle special keys on the keyboard
    // SpaceballMotionFunc -- handle spaceball translation
    // SpaceballRotateFunc -- handle spaceball rotation
    // SpaceballButtonFunc -- handle spaceball button hits
    // ButtonBoxFunc -- handle button box hits
    // DialsFunc -- handle dial rotations
    // TabletMotionFunc -- handle digitizing tablet motion
    // TabletButtonFunc -- handle digitizing tablet button hits
    // MenuStateFunc -- declare when a pop-up menu is in use
    // TimerFunc -- trigger something to happen a certain time from now
    // IdleFunc -- what to do when nothing else is going on
    
    glutSetWindow( MainWindow );
    glutDisplayFunc( Display );
    glutReshapeFunc( Resize );
    glutKeyboardFunc( Keyboard );
    glutMouseFunc( MouseButton );
    glutMotionFunc( MouseMotion );
    glutPassiveMotionFunc( NULL );
    glutVisibilityFunc( Visibility );
    glutEntryFunc( NULL );
    glutSpecialFunc( NULL );
    glutSpaceballMotionFunc( NULL );
    glutSpaceballRotateFunc( NULL );
    glutSpaceballButtonFunc( NULL );
    glutButtonBoxFunc( NULL );
    glutDialsFunc( NULL );
    glutTabletMotionFunc( NULL );
    glutTabletButtonFunc( NULL );
    glutMenuStateFunc( NULL );
    glutTimerFunc( -1, NULL, 0 );
    glutIdleFunc( Animate );
    
    // init glew (a window must be open to do this):
    
#ifdef WIN32
    GLenum err = glewInit( );
    if( err != GLEW_OK )
    {
        fprintf( stderr, "glewInit Error\n" );
    }
    else
        fprintf( stderr, "GLEW initialized OK\n" );
    fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif
    
    
    // set up landscape from height map
    
    LoadLandscape();
    ConstructLandscape();
    
    
    // set up textures to be used:
    
    HeliTexture = SetupTexture(TEXPATH_HELI);
    if(!HeliTexture) {
        fprintf(stderr, "Couldn't load helicopter texture\n");
        exit(1);
    }
    
    GroundTexture = SetupTexture(TEXPATH_GROUND);
    if(!GroundTexture) {
        fprintf(stderr, "Couldn't load ground texture\n");
        exit(1);
    }
    
    SnowTexture = SetupTexture(TEXPATH_SNOW);
    if(!SnowTexture) {
        fprintf(stderr, "Couldn't load snow texture\n");
        exit(1);
    }
    
    TreeTexture = SetupTexture(TEXPATH_TREE);
    if(!TreeTexture) {
        fprintf(stderr, "Couldn't load tree texture\n");
        exit(1);
    }
    
    IceTexture = SetupTexture(TEXPATH_ICE);
    if(!IceTexture) {
        fprintf(stderr, "Couldn't load ice texture\n");
        exit(1);
    }

    
    
    ChooseTreePoints();
    
    
    // set up light model
    
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT, Array3(AmbR, AmbG, AmbB) );
    glLightModeli ( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
    
    // enable lights
    
    glEnable(GL_LIGHTING);
    SetPointLight(GL_LIGHT0, L0x, L0y, L0z, L0r, L0g, L0b);
    glLightf ( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1);
    glEnable(GL_LIGHT0);
    
    SetPointLight(GL_LIGHT1, L1x, L1y, L1z, L1r, L1g, L1b);
    glLightf ( GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1);
    glEnable(GL_LIGHT1);
    
    SetPointLight(GL_LIGHT2, L2x, L2y, L2z, L2r, L2g, L2b);
    glLightf ( GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1);
    glEnable(GL_LIGHT2);
    
    SetPointLight(GL_LIGHT3, L3x, L3y, L3z, L3r, L3g, L3b);
    glLightf ( GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1);
    glEnable(GL_LIGHT3);
    
    SetPointLight(GL_LIGHT4, L4x, L4y, L4z, L4r, L4g, L4b);
    glLightf ( GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1);
    glEnable(GL_LIGHT4);
}



/*
 * Set up the display lists
 */

void
InitLists( )
{
    glutSetWindow( MainWindow );
    
    MakeLandscapeList();
    
    InitListsFromMesh();
    
    MakePancakeList();
    
    // create the axes:
    
    AxesList = glGenLists( 1 );
    glNewList( AxesList, GL_COMPILE );
    glLineWidth( AXES_WIDTH );
    Axes( 1.5 );
    glLineWidth( 1. );
    glEndList( );
}



void InitListsFromMesh() {
    Mesh heli;
    heli.Load(MODELPATH_HELI);
    HelicopterList = MakeListFromMesh(&heli);
    
    Mesh blades;
    blades.Load( MODELPATH_BLADES );
    BladeList = MakeListFromMesh(&blades);
    
    Mesh tree;
    tree.Load(MODELPATH_TREE);
    TreeList = MakeListFromMesh(&tree);
}



GLuint
MakeListFromMesh( Mesh* mesh ) {
    
    GLuint list = glGenLists(1);
    glNewList(list, GL_COMPILE);
    glColor3f(1., 1., 1.);
    
    std::vector<point> points = mesh->GetPoints();
    std::vector<tri> tris = mesh->GetPolys();
    std::vector<normal> normals = mesh->GetNormals();
    std::vector<texCoord> texCoords = mesh->GetTexCoords();
    
    struct point p0, p1, p2;
    normal n0, n1, n2;
    texCoord tc0, tc1, tc2;
    
    glBegin( GL_TRIANGLES );
    
    for(size_t i = 0; i < mesh->GetNumPolys(); i++) {
        
        p0 = points[ tris[ i ].p0 ];
        p1 = points[ tris[ i ].p1 ];
        p2 = points[ tris[ i ].p2 ];

        n0 = normals[ tris [ i ].n0 ];
        n1 = normals[ tris [ i ].n1 ];
        n2 = normals[ tris [ i ].n2 ];
        
        tc0 = texCoords[ tris [ i ].tc0 ];
        tc1 = texCoords[ tris [ i ].tc1 ];
        tc2 = texCoords[ tris [ i ].tc2 ];
        
        glNormal3f(n0.x, n0.y, n0.z);
        glTexCoord2f(tc0.s, tc0.t);
        glVertex3f( p0.x, p0.y, p0.z );
        
        glNormal3f(n1.x, n1.y, n1.z);
        glTexCoord2f(tc1.s, tc1.t);
        glVertex3f( p1.x, p1.y, p1.z );
        
        glNormal3f(n2.x, n2.y, n2.z);
        glTexCoord2f(tc2.s, tc2.t);
        glVertex3f( p2.x, p2.y, p2.z );
        
    }

    glEnd();
    glEndList();
    
    return list;
}





void
LoadLandscape( ) {
    
    FILE* fp = fopen(PATH_XTR, "r");
    if(!fp) {
        fprintf(stderr, "Couldn't open XTR file.\n\n");
    }
    
    float* Elevations = ReadXtr(&NumElevLong, &NumElevLat,
                                &LandscapeHeightMin, &LandscapeMaxHeightMax,
                                fp);
    if(!Elevations) {
        fprintf(stderr, "Error: couldn't read the height map file.\n\n");
        return;
    }
    
    printf("Num Long: %d\nNum Lat: %d\n\n", NumElevLong, NumElevLat);
    
    // Elevations has rows of longitudes
    // build 2d array of elevations, LandscapeGrid
    
    LandscapeGrid = new point*[NumElevLong];
    LandscapeNormals = new point*[NumElevLong];
    for(int i = 0; i < NumElevLong; i++) {
        LandscapeGrid[i] = new point[NumElevLat];
        LandscapeNormals[i] = new point[NumElevLat];
    }
    
    for(int lat = 0; lat < NumElevLat; lat++ ) {
        
        int offset = lat * NumElevLong;
        
        for(int lon = 0; lon < NumElevLong; lon++) {
            
            LandscapeGrid[lon][lat] = point {
                (float)lon,
                Elevations[offset + lon],
                (float)lat,
                0., 0., 0.
            };
            
            // center the point in xz,
            // shift y coordinate so lowest point is at y = 0
            
            LandscapeGrid[lon][lat].x -= (float)NumElevLong / 2.;
            LandscapeGrid[lon][lat].z -= (float)NumElevLat / 2.;
            LandscapeGrid[lon][lat].y -= (LandscapeHeightMin - ELEV_BASE);
        }
    }
    
    // now that we have all the points, calculate the normals
    
    for(int lat = 1; lat < NumElevLat - 1; lat++) {
        for(int lon = 1; lon < NumElevLong - 1; lon++) {
            //LandscapeNormals[lon][lat] = FindNormal(lon, lat);
        }
    }
}




void
ConstructLandscape( ) {
    
    // initialize LandscapePoints
    
    LandscapePoints = new point*[NumElevLong * LANDSCAPE_RES];
    LandscapeVertexNormals = new point*[NumElevLong * LANDSCAPE_RES];
    for (int i = 0; i < NumElevLong * LANDSCAPE_RES; i++) {
        LandscapePoints[i] = new point[NumElevLat * LANDSCAPE_RES];
        LandscapeVertexNormals[i] = new point[NumElevLat * LANDSCAPE_RES];
    }
    
    // construct points to draw based on actual elevation data in LandscapeGrid
    
    for(int lat = 0; lat < NumElevLat; lat++) {
        for(int lon = 0; lon < NumElevLong; lon++) {
            for(int latSubd = 0; latSubd < LANDSCAPE_RES; latSubd++) {
                for(int lonSubd = 0; lonSubd < LANDSCAPE_RES; lonSubd++) {
                    
                    int z = LANDSCAPE_RES * lat + latSubd;
                    int x = LANDSCAPE_RES * lon + lonSubd;
                    
                    // get average height
                    
                    float height = LandscapeGrid[lon][lat].y;
                    
                    if(latSubd > 0 && latSubd < LANDSCAPE_RES && lonSubd > 0 && lonSubd < LANDSCAPE_RES) {
                        if(lat < NumElevLat - 1 && lon < NumElevLong - 1) {
                            height = (      LandscapeGrid[lon][lat].y +
                                            LandscapeGrid[lon + 1][lat].y +
                                            LandscapeGrid[lon][lat + 1].y +
                                            LandscapeGrid[lon + 1][lat + 1].y) / 4.;
                        }
                    }
                    
                    LandscapePoints[x][z] = point {
                        LandscapeGrid[lon][lat].x + (float)lonSubd / (float)LANDSCAPE_RES,
                        //LandscapeGrid[lon][lat].y * LANDSCAPE_YSCALE,
                        height * LANDSCAPE_YSCALE,
                        LandscapeGrid[lon][lat].z + (float)latSubd / (float)LANDSCAPE_RES,
                        1., 1., 1.
                    };
                    
                    // scale the point in xz
                    
                    LandscapePoints[x][z].x *= LANDSCAPE_XSCALE;
                    LandscapePoints[x][z].z *= LANDSCAPE_ZSCALE;

                }
            }
        }
    }
}




point
FindNormal(point** grid, int lon, int lat) {
    
    return FindCross(
                     point{ grid[lon - 1][lat].x - grid[lon + 1][lat].x,
                         grid[lon - 1][lat].y - grid[lon + 1][lat].y,
                         grid[lon - 1][lat].z - grid[lon + 1][lat].z
                     },
                     point{ grid[lon][lat - 1].x - grid[lon][lat + 1].x,
                         grid[lon][lat - 1].y - grid[lon][lat + 1].y,
                         grid[lon][lat - 1].z - grid[lon][lat + 1].z
                     } );
}



void
LandscapeVertex(int lon, int lat, int texTileWidth) {
    
    float s = (float)((int)(LandscapePoints[lon][lat].x) % texTileWidth) / texTileWidth;
    float t = (float)((int)(LandscapePoints[lon][lat].z) % texTileWidth) / texTileWidth;
    
    glTexCoord2f(s, t);
    
    point normal;
    if(UseNormals) {
        normal = LandscapeVertexNormals[lon][lat];
    } else {
        normal = point {0., 1., 0.};
    }
    
    glNormal3f(normal.x, normal.y, normal.z);
    
    glVertex3f(LandscapePoints[lon][lat].x,
               LandscapePoints[lon][lat].y,
               LandscapePoints[lon][lat].z);

}



void
MakeLandscapeList( ) {

    for(int lat = 1; lat < NumElevLat * LANDSCAPE_RES - 1; lat++) {
        for(int lon = 1; lon < NumElevLong * LANDSCAPE_RES - 1; lon++) {
            LandscapeVertexNormals[lon][lat] = FindNormal(LandscapePoints, lon, lat);
        }
    }
    
    // make texTileWidth the number of indices in a texture tile
    
    int texTileWidth;
        if(NumElevLat > NumElevLong) {
            texTileWidth = (int)(
                    (LandscapePoints[0][NumElevLat * LANDSCAPE_RES - 1].z -
                     LandscapePoints[0][0].z) / (float)TEX_TILES);
        }
        else {
            texTileWidth = (int)(
                                 (LandscapePoints[NumElevLong * LANDSCAPE_RES - 1][0].x -
                                  LandscapePoints[0][0].x) / (float)TEX_TILES);
        }
    
    printf("texTileWidth: %d\n", texTileWidth);
    
    //LandscapePoints[lon][lat]
    //                 x    z
    
    LandscapeList = glGenLists(1);
    glNewList(LandscapeList, GL_COMPILE);
    
    for(int lon = 1; lon < NumElevLong * LANDSCAPE_RES - 2; lon++) {

        glBegin(GL_TRIANGLE_STRIP);
        
        for(int lat = NumElevLat * LANDSCAPE_RES - 2; lat > 0; lat--) {
            
            LandscapeVertex(lon, lat, texTileWidth);
            LandscapeVertex(lon + 1, lat, texTileWidth);
            
        }
        glEnd();
    }
    glEndList();
}








void ChooseTreePoints() {
    
    // allocate TreePoints and occupiedLocations
    
    TreePoints = new point[NUM_TREES];
    
    OccupiedLocations = new bool*[NumElevLong * LANDSCAPE_RES];
    for(int i = 0; i < NumElevLong * LANDSCAPE_RES; i++) {
        OccupiedLocations[i] = new bool[NumElevLat * LANDSCAPE_RES];
        for(int j = 0; j < NumElevLat * LANDSCAPE_RES; j++) {
            OccupiedLocations[i][j] = false;
        }
    }
    
    for(int i = 0; i < NUM_TREES;) {
        
        int x = rand() % (NumElevLong * LANDSCAPE_RES - 4) + 2;
        int z = rand() % (NumElevLat * LANDSCAPE_RES - 4) + 2;
        
        if(OccupiedLocations[x][z] == false) {
            
            // check whether this location is too high or low
            
            if(LandscapePoints[x][z].y > TREE_ELEV_MAX ||
               LandscapePoints[x][z].y < TREE_ELEV_MIN) {
                continue;
            }
            
            if(LandscapePoints[x + 1][z].y - LandscapePoints[x][z].y > TREE_DELTA_ELEV_MAX ||
               LandscapePoints[x - 1][z].y - LandscapePoints[x][z].y > TREE_DELTA_ELEV_MAX ||
               LandscapePoints[x][z + 1].y - LandscapePoints[x][z].y > TREE_DELTA_ELEV_MAX ||
               LandscapePoints[x][z - 1].y - LandscapePoints[x][z].y > TREE_DELTA_ELEV_MAX) {
                
                continue;
            
            }
            
            OccupiedLocations[x][z] = true;
            TreePoints[i] = point();
            TreePoints[i].x = LandscapePoints[x][z].x;
            TreePoints[i].y = LandscapePoints[x][z].y;
            TreePoints[i].z = LandscapePoints[x][z].z;
            
            i++;
        }
    }
}



GLuint
SetupTexture( char* path )
{
    int width, height;
    
    // use JG's function to lead the texure
    
    unsigned char *Texture = BmpToTexture(path, &width, &height);
    
    if (Texture == NULL) {
        return -1;
    }
    
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    
    GLuint tex;
    glGenTextures(1, &tex);
    
    glBindTexture(GL_TEXTURE_2D, tex);
    
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
    
    int level = 0,
    ncomps = 3,
    border = 0;
    
    glTexImage2D(
                 GL_TEXTURE_2D,
                 level,
                 ncomps,
                 width,
                 height,
                 border,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,
                 Texture
                 );
    
    printf("loaded texture from file %s\n\n", path);
    
    return tex;
}

















// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
    // get # of milliseconds since the start of the program:
    
    int ms = glutGet( GLUT_ELAPSED_TIME );
    
    // convert it to seconds:
    
    return (float)ms / 1000.f;
}


/*
 * Joe Graphics's vector functions
 */

float
Dot( float v1[3], float v2[3] )
{
    return v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
}

void
Cross( float v1[3], float v2[3], float vout[3] )
{
    float tmp[3];
    tmp[0] = v1[1]*v2[2] - v2[1]*v1[2];
    tmp[1] = v2[0]*v1[2] - v1[0]*v2[2];
    tmp[2] = v1[0]*v2[1] - v2[0]*v1[1];
    vout[0] = tmp[0];
    vout[1] = tmp[1];
    vout[2] = tmp[2];
}




point FindCross(point p1, point p2) {
    float v1[] = {p1.x, p1.y, p1.z};
    float v2[] = {p2.x, p2.y, p2.z};
    float res[3];
    
    Cross(v1, v2, res);
    
    return point {res[0], res[1], res[2]};
}




float
Unit( float vin[3], float vout[3] )
{
    float dist = vin[0]*vin[0] + vin[1]*vin[1] + vin[2]*vin[2];
    if( dist > 0.0 )
    {
        dist = sqrt( dist );
        vout[0] = vin[0] / dist;
        vout[1] = vin[1] / dist;
        vout[2] = vin[2] / dist;
    }
    else
    {
        vout[0] = vin[0];
        vout[1] = vin[1];
        vout[2] = vin[2];
    }
    return dist;
}



// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
    if( DebugOn != 0 )
        fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );
    
    switch( c )
    {
        case 'w':
        case 'W':
            HeliAltitude -= 15. * HELI_CLIMB_FAC;
            break;
            
        case 's':
        case 'S':
            HeliAltitude += 15. * HELI_CLIMB_FAC;
            break;
            
        case 'a':
        case 'A':
            HeliAnimRadius -= 25. * HELI_TURN_FAC;
            break;
            
        case 'd':
        case 'D':
            HeliAnimRadius += 25. * HELI_TURN_FAC;
            break;
            
        case 'c':
        case 'C':
            LookFollows = !LookFollows;
            Scale = 1.;
            FollowScale = DEFAULT_FOLLOW_SCALE;
            Xrot = 0.;
            Yrot = 0.;
            break;
            
        case 'e':
        case 'E':
            WhichMouseAction = ROTATE;
            break;
            
        case 'f':
        case 'F':
            Freeze = ! Freeze;
            if( Freeze )
                glutIdleFunc( NULL );
            else
                glutIdleFunc( Animate );
            break;
            
        case 'o':
        case 'O':
            WhichProjection = ORTHO;
            break;
            
        case 'p':
        case 'P':
            WhichProjection = PERSP;
            break;
            
        case 'q':
        case 'Q':
        case ESCAPE:
            DoMainMenu( QUIT );	// will not return here
            break;				// happy compiler
            
            
        case 'r':
        case 'R':
            WhichMouseAction = SCALE;
            break;
            
            
        default:
            fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
    }
    
    // force a call to Display( ):
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
    int b = 0;			// LEFT, MIDDLE, or RIGHT
    
    if( DebugOn != 0 )
        fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );
    
    
    // get the proper button bit mask:
    
    switch( button )
    {
        case GLUT_LEFT_BUTTON:
            b = LEFT;		break;
            
        case GLUT_MIDDLE_BUTTON:
            b = MIDDLE;		break;
            
        case GLUT_RIGHT_BUTTON:
            b = RIGHT;		break;
            
        default:
            b = 0;
            fprintf( stderr, "Unknown mouse button: %d\n", button );
    }
    
    
    // button down sets the bit, up clears the bit:

    if( state == GLUT_DOWN )
    {
        if(!ButtonDown) {
            
            Xmouse = x;
            Ymouse = y;
            
            ButtonDownX = x;
            ButtonDownY = y;
        }
        
        ActiveButton |= b;		// set the proper bit
        
        ButtonDown = true;
    }
    else
    {
        ActiveButton &= ~b;		// clear the proper bit
        
        ButtonDown = false;
        
        ButtonDownX = 0;
        ButtonDownY = 0;
        InputX = 0;
        InputY = 0;
    }
    
}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
    if( DebugOn != 0 )
        fprintf( stderr, "MouseMotion: %d, %d\n", x, y );
    
    
    int dx = x - Xmouse;		// change in mouse coords
    int dy = y - Ymouse;
    
    if(!LookFollows) {
    
#ifdef __APPLE__
        // There's probably no middle mouse button, so use keys insted
        if( ( ActiveButton & LEFT ) != 0 )
        {
            if(WhichMouseAction == ROTATE){
                Xrot += ( ANGFACT*dy );
                Yrot += ( ANGFACT*dx );
            } else {
                Scale += SCLFACT * (float) ( dx - dy );
                
                // keep object from turning inside-out or disappearing:
                
                if( Scale < MINSCALE )
                    Scale = MINSCALE;
                
            }
        }
#else
        if( ( ActiveButton & LEFT ) != 0 )
        {
            Xrot += ( ANGFACT*dy );
            Yrot += ( ANGFACT*dx );
        }
        
        
        if( ( ActiveButton & MIDDLE ) != 0 )
        {
            Scale += SCLFACT * (float) ( dx - dy );
            
            // keep object from turning inside-out or disappearing:
            
            if( Scale < MINSCALE )
                Scale = MINSCALE;
        }
#endif
    }
    
    // if in follow-camera mode
    
    else {
        
        if( ( ActiveButton & LEFT ) != 0 )
        {
            InputX = x - ButtonDownX;
            InputY = y - ButtonDownY;

            //HeliAltitude += HELI_ALT_FAC * (float)dy;
            //HeliAnimRadius += HELI_TURN_FAC * (float)dx;
        }
    }
    
    Xmouse = x;			// new current position
    Ymouse = y;
    

    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
    ActiveButton = 0;
    AxesOn = 1;
    DebugOn = 0;
    DepthCueOn = 0;
    Scale = 1.;
    FollowScale = DEFAULT_FOLLOW_SCALE;
    WhichColor = WHITE;
    WhichProjection = PERSP;
    Xrot = 0.;
    Yrot = 0.;
    LookFollows = true;
    HeliAltitude = HeliInitAlt;
    InputX = InputY = 0;
    ButtonDown = false;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
    if( DebugOn != 0 )
        fprintf( stderr, "ReSize: %d, %d\n", width, height );
    
    // don't really need to do anything since window size is
    // checked each time in Display( ):
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
    if( DebugOn != 0 )
        fprintf( stderr, "Visibility: %d\n", state );
    
    if( state == GLUT_VISIBLE )
    {
        glutSetWindow( MainWindow );
        glutPostRedisplay( );
    }
    else
    {
        // could optimize by keeping track of the fact
        // that the window is not visible and avoid
        // animating or redrawing it ...
    }
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
    0.f, 1.f, 0.f, 1.f
};

static float xy[ ] = {
    -.5f, .5f, .5f, -.5f
};

static int xorder[ ] = {
    1, 2, -3, 4
};

static float yx[ ] = {
    0.f, 0.f, -.5f, .5f
};

static float yy[ ] = {
    0.f, .6f, 1.f, 1.f
};

static int yorder[ ] = {
    1, 2, 3, -2, 4
};

static float zx[ ] = {
    1.f, 0.f, 1.f, 0.f, .25f, .75f
};

static float zy[ ] = {
    .5f, .5f, -.5f, -.5f, 0.f, 0.f
};

static int zorder[ ] = {
    1, 2, 3, 4, -5, 6
};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
    glBegin( GL_LINE_STRIP );
    glVertex3f( length, 0., 0. );
    glVertex3f( 0., 0., 0. );
    glVertex3f( 0., length, 0. );
    glEnd( );
    glBegin( GL_LINE_STRIP );
    glVertex3f( 0., 0., 0. );
    glVertex3f( 0., 0., length );
    glEnd( );
    
    float fact = LENFRAC * length;
    float base = BASEFRAC * length;
    
    glBegin( GL_LINE_STRIP );
    for( int i = 0; i < 4; i++ )
    {
        int j = xorder[i];
        if( j < 0 )
        {
            
            glEnd( );
            glBegin( GL_LINE_STRIP );
            j = -j;
        }
        j--;
        glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
    }
    glEnd( );
    
    glBegin( GL_LINE_STRIP );
    for( int i = 0; i < 5; i++ )
    {
        int j = yorder[i];
        if( j < 0 )
        {
            
            glEnd( );
            glBegin( GL_LINE_STRIP );
            j = -j;
        }
        j--;
        glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
    }
    glEnd( );
    
    glBegin( GL_LINE_STRIP );
    for( int i = 0; i < 6; i++ )
    {
        int j = zorder[i];
        if( j < 0 )
        {
            
            glEnd( );
            glBegin( GL_LINE_STRIP );
            j = -j;
        }
        j--;
        glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
    }
    glEnd( );
    
}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
    // guarantee valid input:
    
    float h = hsv[0] / 60.f;
    while( h >= 6. )	h -= 6.;
    while( h <  0. ) 	h += 6.;
    
    float s = hsv[1];
    if( s < 0. )
        s = 0.;
    if( s > 1. )
        s = 1.;
    
    float v = hsv[2];
    if( v < 0. )
        v = 0.;
    if( v > 1. )
        v = 1.;
    
    // if sat==0, then is a gray:
    
    if( s == 0.0 )
    {
        rgb[0] = rgb[1] = rgb[2] = v;
        return;
    }
    
    // get an rgb from the hue itself:
    
    float i = floor( h );
    float f = h - i;
    float p = v * ( 1.f - s );
    float q = v * ( 1.f - s*f );
    float t = v * ( 1.f - ( s * (1.f-f) ) );
    
    float r, g, b;			// red, green, blue
    switch( (int) i )
    {
        case 0:
            r = v;	g = t;	b = p;
            break;
            
        case 1:
            r = q;	g = v;	b = p;
            break;
            
        case 2:
            r = p;	g = v;	b = t;
            break;
            
        case 3:
            r = p;	g = q;	b = v;
            break;
            
        case 4:
            r = t;	g = p;	b = v;
            break;
            
        case 5:
            r = v;	g = p;	b = q;
            break;
    }
    
    
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
}


// -------------------------------------------------------------------------



// menus

void
DoAxesMenu( int id )
{
    AxesOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
    WhichColor = id - RED;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
    DebugOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
    DepthCueOn = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
    switch( id )
    {
        case RESET:
            Reset( );
            break;
            
        case QUIT:
            // gracefully close out the graphics:
            // gracefully close the graphics window:
            // gracefully exit the program:
            glutSetWindow( MainWindow );
            glFinish( );
            glutDestroyWindow( MainWindow );
            exit( 0 );
            break;
            
        default:
            fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
    }
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
    WhichProjection = id;
    
    glutSetWindow( MainWindow );
    glutPostRedisplay( );
}


void
InitMenus( )
{
    glutSetWindow( MainWindow );
    
    int numColors = sizeof( Colors ) / ( 3*sizeof(int) );
    int colormenu = glutCreateMenu( DoColorMenu );
    for( int i = 0; i < numColors; i++ )
    {
        glutAddMenuEntry( ColorNames[i], i );
    }
    
    int axesmenu = glutCreateMenu( DoAxesMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int depthcuemenu = glutCreateMenu( DoDepthMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int debugmenu = glutCreateMenu( DoDebugMenu );
    glutAddMenuEntry( "Off",  0 );
    glutAddMenuEntry( "On",   1 );
    
    int projmenu = glutCreateMenu( DoProjectMenu );
    glutAddMenuEntry( "Orthographic",  ORTHO );
    glutAddMenuEntry( "Perspective",   PERSP );
    
    int mainmenu = glutCreateMenu( DoMainMenu );
    glutAddSubMenu(   "Axes",          axesmenu);
    glutAddSubMenu(   "Colors",        colormenu);
    glutAddSubMenu(   "Depth Cue",     depthcuemenu);
    glutAddSubMenu(   "Projection",    projmenu );
    glutAddMenuEntry( "Reset",         RESET );
    glutAddSubMenu(   "Debug",         debugmenu);
    glutAddMenuEntry( "Quit",          QUIT );
    
    // attach the pop-up menu to the right mouse button:
    
    glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
    glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );
    
    char c;			// one character to print
    for( ; ( c = *s ) != '\0'; s++ )
    {
        glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
    }
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
    glPushMatrix( );
    glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
    float sf = ht / ( 119.05f + 33.33f );
    glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
    char c;			// one character to print
    for( ; ( c = *s ) != '\0'; s++ )
    {
        glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
    }
    glPopMatrix( );
}



