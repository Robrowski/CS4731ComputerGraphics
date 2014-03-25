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
#define WINDOW_WIDTH  640.0*2
#define WINDOW_HEIGHT 480.0*2

// CHECK THESE - defaults for reading in GRS files
#define DEFAULT_L 0.0
#define DEFAULT_T WINDOW_HEIGHT
#define DEFAULT_R 0.0
#define DEFAULT_B WINDOW_WIDTH


// Vertex constants
#define NUM_VERTICES_TRI  3
#define NUM_VERTICES_PENT 5


// Colors
#define RED_VEC   vec(1.0, 0.0, 0.0, 1.0);
#define GREEN_VEC vec(1.0, 0.0, 0.0, 1.0);
#define BLUE_VEC  vec(1.0, 0.0, 0.0, 1.0);


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
Frame newFrame(float L,	float R, float B,float T);
void setGLViewport(Frame toSet);
mat4 Frame_Ortho2D(Frame vpd);




#endif // UTILS_H