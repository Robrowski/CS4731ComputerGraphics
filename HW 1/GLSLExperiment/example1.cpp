// Starter program for HW 1. 
// Program draws a triangle. Study the program first
// The function generateGeometry can be modified to draw many 2D drawings (e.g. 2D Sierpinski Gasket)
// Generated using randomly selected vertices and bisection

#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"
#include <stdlib.h>


typedef vec2 point2;

// remember to prototype
void generateGeometry( void );
void initGPUBuffers( void );
void shaderSetup( void );
void display( void );
void keyboard( unsigned char key, int x, int y );


using namespace std;

// Good shit
GLuint program;

// Specifically for vertex shader and scaling
mat4 ortho;
GLint ProjLoc;
vec4 gl_FragColor;


// Only used to set up buffers
void initGPUBuffers( void )
{
	// Create a vertex array object
	GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
}


void shaderSetup( void )
{
	// Load shaders and use the resulting shader program
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

    glClearColor( 1.0, 1.0, 1.0, 1.0 );        // sets white as color used to clear screen

	// Setting world window shit
	ProjLoc = glGetUniformLocation(program, "Proj");
}


void drawPicture(MyPicture *pic){
	unsigned int lineNum;
	for (lineNum = 0; lineNum < pic->numLines; lineNum++){
		// Because of what the pointer is in the MyPolyline Struct, can't actually get the size 
		// of it because the compiler doesn't know that its a pointer to an array. The compiler
		// thinks it is just a pointer to one polyline. 
		glBufferData( GL_ARRAY_BUFFER, (pic->pl[lineNum].NumPoints)*sizeof(MyPoint), pic->pl[lineNum].pt , GL_STATIC_DRAW );
	
		// All drawing happens in display function
		glDrawArrays( GL_LINE_STRIP, 0, pic->pl[lineNum].NumPoints );    // draw the points
	}
}


MyPicture* readRandomPicture(void){
	char* file;
	
	switch( rand() % 8){
	case 0:
		file = "usa.dat";
		break;
	case 1:
		file = "rex.dat";
		break;
	case 2:
		file = "scene.dat";
		break;
	case 3:
		file = "vinci.dat";
		break;
	case 4:
		file = "knight.dat";
		break;
	case 5:
		file = "dragon.dat";
		break;
	case 6:
		file = "house.dat";
		break;
	case 7:
		file = "birdhead.dat";
		break;
	case 8:
		file = "dino.dat";
		break;
	}

	return parseGRS(file);
}



void drawQuadrants(float X, float Y, float width, float height, int numRecursions){
	if (numRecursions == 0) return; // Done!	
	
	// A Picture
	MyPicture* pic =  readRandomPicture();
	
	// Send transformation over to shader
	ortho = Frame_Ortho2D(pic->f);
	glUniformMatrix4fv(ProjLoc,1,GL_TRUE,ortho);

	// Check aspect ratio stuff, see lecture 4 
	// http://web.cs.wpi.edu/~emmanuel/courses/cs4731/D14/slides/lecture04.pdf 
	float w,h;
	float aspectRatio = getAspectRatio(pic->f);
	if (aspectRatio > width/height){
		w = width;
		h = width/aspectRatio;
	} else if (aspectRatio < width/height){
		w = height*aspectRatio;
		h = height;
	} else {
		w = width;
		h = height;
	}


	// Top Left = red
	glViewport(X, Y + h/2, w/2, h/2);
	drawPicture(pic);
	//gl_FragColor;


	// Bottom left = blue
	glViewport(X, Y, w/2, h/2);
	drawPicture(pic);


	// Top right = green
	glViewport(X + w/2, Y + h/2 ,w/2, h/2);
	drawPicture(pic);
	
	// Next iteration!
	drawQuadrants(X + width/2, Y, width/2, height/2, numRecursions-1);
}




void display( void )
{
	// Start by clearing the window
	glClear( GL_COLOR_BUFFER_BIT );

	drawQuadrants(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, 10);

	// THE FINAL STEP
    glFlush();			// force output to graphics hardware
}





/** Main function for loading polyline files and
*   drawing them.
*
*/
int main( int argc, char **argv )
{
	// main function: program starts here
	genericInit(argc, argv);

	// Defined above
    // generateGeometry(points );                           // Call function that generates points to draw
    initGPUBuffers( );							   // Create GPU buffers
    shaderSetup( );                                // Connect this .cpp file to shader file
	

	// Color?!!?
	gl_FragColor = glGetUniformLocation(program, "gl_FragColor");
	

	// Calbacks
    glutDisplayFunc( display );                    // Register display callback function
    glutKeyboardFunc( keyboard );                  // Register keyboard callback function

	// Can add minimalist menus here
	// add mouse handler
	// add resize window functionality (should probably try to preserve aspect ratio)

	// enter the drawing loop
    glutMainLoop();
    return 0;
}
