#ifndef UTILS_H
#define UTILS_H

#include "Angel.h"
typedef vec4 MyPoint;


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
} ;
typedef struct MyPolyline MyPolyline;

// A series of polines
typedef struct {
	int numLines;
	MyPolyline* pl;
	Frame f;
} MyPicture;

typedef struct {
	GLint numVertices;
	MyPolyline* points; // holds points and number of points
	vec3* triangles; // holds 3 numbers representing indecies of vertices
	//vec4* colors; // could hold color data
} PLYPicture;



// Program locations
extern GLuint program;
extern GLint ProjLoc;
extern GLint colorLoc;

// Default world window
#define WINDOW_WIDTH  512 //640
#define WINDOW_HEIGHT 512
#define FIFTY_MILLION 50*1000*1000
#define ONE_MILLION      1000*1000

// CHECK THESE - defaults for reading in GRS files
#define DEFAULT_L 0.0
#define DEFAULT_T 640.0
#define DEFAULT_R 0.0
#define DEFAULT_B 480.0


// File reading usefulness
#define BUFFER_SIZE 100 // 100 characters
#define READ_NEXT_LINE fgets( buf, BUFFER_SIZE,f)


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

// init.cpp
void genericInit( int argc, char **argv, char* window);
void initGPUBuffers1( void );
void shaderSetup1( void );

// MyPolyline.cpp
MyPolyline* generateEmptyPolyline(int size);
MyPicture* generateEmptyPicture(int numPolyLines);
void copyPolyline(MyPolyline* src, MyPolyline* dst);
void printPolyline(MyPolyline* toPrint);
void drawMyPolyline(MyPolyline ln, GLint lineType);

// MyPicture.cpp
void drawPicture(MyPicture *pic);
void drawMyPicture(MyPicture *pic, GLint lineType);

// geometries.cpp
MyPolyline* generateTriangle(void);
MyPolyline* generatePentagon(void);


// keys.cpp
void keyboard1( unsigned char key, int x, int y );

// color.cpp
void setLineColor(vec4 toSet);


// frame.cpp
#define DEFAULT_WORLD_FRAME newFrame(0,  WINDOW_WIDTH , 0,WINDOW_HEIGHT )
Frame newFrame(float L,	float R, float B,float T);
void setGLViewport(Frame toSet);
mat4 Frame_Ortho2D(Frame vpd);
float getAspectRatio(Frame f);
void sendOrthoToShader(Frame f);
void setViewPort(int X, int Y, int vpWidth, int vpHeight, Frame wFrame);


// GRS.cpp
MyPicture* readRandomPicture(void);
MyPicture* parseGRS( char *file );

// PLYreader.cpp


// PLYPicture.cpp
PLYPicture* generateEmptyPLYPicture(GLint numVertices, GLint numTriangles);


// draw.cpp
void drawFern(int iterations);
void drawGingerBreadMan(int iterations);
void drawSierpinski(void);


// hw1.cpp
void drawQuadrants(int X, int Y, int width, int height, int numRecursions,MyPicture pics[], int iterationNumber);






#endif // UTILS_H