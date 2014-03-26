#ifndef UTILS_H
#define UTILS_H

#include "Angel.h"
typedef vec2 MyPoint;
typedef vec2 point2;

typedef struct {
	float L;
	float R;
	float T;
	float B;
} Frame;


// Data types
// A single polyline
struct MyPolyline{
	int NumPoints;
	MyPoint* pt; // Pointer to c-allocated array
	struct MyPolyline* prev;
} ;
typedef struct MyPolyline MyPolyline;

// A series of polines
typedef struct {
	int numLines;
	MyPolyline* pl;
	Frame f;
} MyPicture;



// Default world window
#define WINDOW_WIDTH  800 //640
#define WINDOW_HEIGHT 800
#define FIFTY_MILLION 50*1000*1000
#define ONE_MILLION      1000*1000

// CHECK THESE - defaults for reading in GRS files
#define DEFAULT_L 0.0
#define DEFAULT_T 640.0
#define DEFAULT_R 0.0
#define DEFAULT_B 480.0


// Vertex constants
#define NUM_VERTICES_TRI  3
#define NUM_VERTICES_PENT 5


// Colors
#define RED   1
#define BLUE  2
#define GREEN 3
#define RED_VEC   vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN_VEC vec4(0.0, 1.0, 0.0, 1.0)
#define BLUE_VEC  vec4(0.0, 0.0, 1.0, 1.0)


// textfile.cpp
char *textFileRead(char *fn) ;
int textFileWrite(char *fn, char *s) ;

// mainInits.cpp
void genericInit( int argc, char **argv );


// MyPolyline.cpp
MyPolyline* generateEmptyPolyline(int size);
MyPicture* generateEmptyPicture(int numPolyLines);
void copyPolyline(MyPolyline* src, MyPolyline* dst);
void printPolyline(MyPolyline* toPrint);
MyPicture* parseGRS( char *file );


// geometries.cpp
MyPolyline* generateTriangle(void);
MyPolyline* generatePentagon(void);


// keys.cpp
void keyboard( unsigned char key, int x, int y );




// frame.cpp
#define DEFAULT_WORLD_FRAME newFrame(0,  WINDOW_WIDTH , 0,WINDOW_HEIGHT )
Frame newFrame(float L,	float R, float B,float T);
void setGLViewport(Frame toSet);
mat4 Frame_Ortho2D(Frame vpd);
float getAspectRatio(Frame f);
void sendOrthoToShader(Frame f);
void setViewPort(int X, int Y, int vpWidth, int vpHeight, Frame wFrame);



// example1.cpp - to be moved...
void drawFern(int iterations);
//MyPicture* drawQuadrants(int X, int Y, int width, int height, int numRecursions);
void drawGingerBreadMan(int iterations);
void drawSierpinski(void);
MyPicture* readRandomPicture(void);
void drawQuadrants(int X, int Y, int width, int height, int numRecursions,MyPicture pics[], int iterationNumber);




#endif // UTILS_H