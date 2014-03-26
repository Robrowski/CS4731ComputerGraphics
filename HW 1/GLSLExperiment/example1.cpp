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
GLint ProjLoc;
GLint colorLoc;


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


void drawMyPicture(MyPicture *pic, int lineType){
	int lineNum;
	for (lineNum = 0; lineNum < pic->numLines; lineNum++){
		// Because of what the pointer is in the MyPolyline Struct, can't actually get the size 
		// of it because the compiler doesn't know that its a pointer to an array. The compiler
		// thinks it is just a pointer to one polyline. 
		glBufferData( GL_ARRAY_BUFFER, (pic->pl[lineNum].NumPoints)*sizeof(MyPoint), pic->pl[lineNum].pt , GL_STATIC_DRAW );
	
		// All drawing happens in display function
		glDrawArrays( lineType, 0, pic->pl[lineNum].NumPoints );    // draw the points
	}
}


void drawPicture(MyPicture *pic){
	drawMyPicture( pic,GL_LINE_STRIP);
}






MyPicture* readRandomPicture(void){
	char* file;
	
	switch( rand() % 9){
	case 0:
		file = "usa.dat";
		break;
	case 1:
		file = "birdhead.dat";
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
		file = "rex.dat";
		break;
	case 8:
		file = "dino.dat";
		break;
	}

	return parseGRS(file);
}


void setLineColor(vec4 toSet){
	glUniform4fv( glGetUniformLocation(program, "fragmentShaderLineColor"),1, toSet );
}



void drawQuadrants(int X, int Y, int width, int height, int numRecursions){
	// A Picture
	MyPicture* pic =  readRandomPicture();
	
	// Send transformation over to shader
	// **** MOVE THIS TO DRAW PICTURE
	sendOrthoToShader(pic->f);

	// Check aspect ratio stuff, see lecture 4 
	// http://web.cs.wpi.edu/~emmanuel/courses/cs4731/D14/slides/lecture04.pdf 
	int w,h;
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
	glViewport(X, Y + height/2, w/2, h/2);
	setLineColor(RED_VEC);
	drawPicture(pic);

	// Bottom left = blue
	glViewport(X, Y, w/2, h/2);
	setLineColor(BLUE_VEC);
	drawPicture(pic);


	// Top right = green
	glViewport(X + width/2, Y + height/2 ,w/2, h/2);
	setLineColor(GREEN_VEC);
	drawPicture(pic);
	
	// If final iteration, place the thing in the corner in red
	if (numRecursions == 1){
		glViewport(X + width/2, Y, w/2, h/2);
		setLineColor(RED_VEC);
		drawPicture(pic);
		return; // Done!
	}// else
	// Next iteration!
	drawQuadrants(X + width/2, Y, width/2, height/2, numRecursions-1);
}



float a[] = {	0.0f,	0.2f,	-0.15f,	0.85f};
float b[] = {	0.0f,	0.23f,	0.26f ,	-0.04f };
float c[] = {	0.0f,	-0.26f,	0.28f ,	0.04f };
float d[] = {	0.16f,	0.22f,	0.24f ,	0.85f };
float tx[] = {  0.0f,	0.0f,	0.0f ,	0.0f };
float ty[] = {  0.0f,	1.6f,	0.44f ,	1.6f };


/** Take the number of iterations and draw the fern for that many points */
void drawFern(int iterations){
	// Going to make a picture out of it!
	MyPicture* fernPic = generateEmptyPicture(1);
	fernPic->pl = generateEmptyPolyline(iterations);
	MyPoint* pt = fernPic->pl->pt; // array of points in MyPolyline
	fernPic->f = newFrame(0,0,0,0);// Initially...

	// Initial point
	pt->x = 0.0;
	pt->y = 0.0;

	// Generate the points
	int iter, chosenIndex;
	for (iter = 1; iter < iterations; iter++){
		int rNum = (rand()%100) + 1; // Random number from 1 to 100
		if (rNum <= 85) chosenIndex = 3;      // 85%
		else if (rNum <= 92) chosenIndex = 2; //  7%
		else if (rNum <= 99) chosenIndex = 1; //  7%
		else				 chosenIndex = 0; //  1%

		pt[iter].x =  a[chosenIndex] * pt[iter-1].x + c[chosenIndex] * pt[iter-1].y + tx[chosenIndex];
		pt[iter].y =  b[chosenIndex] * pt[iter-1].x + d[chosenIndex] * pt[iter-1].y + ty[chosenIndex];
	
		// Check for edges of world space
		if (pt[iter].x > fernPic->f.R) fernPic->f.R = pt[iter].x;
		if (pt[iter].x < fernPic->f.L) fernPic->f.L = pt[iter].x;
		if (pt[iter].y > fernPic->f.T) fernPic->f.T = pt[iter].y;
	}
	
	setLineColor(GREEN_VEC);
	sendOrthoToShader(fernPic->f);
	drawMyPicture(fernPic, GL_POINTS);
}






void display( void )
{
	// Start by clearing the window
	glClear( GL_COLOR_BUFFER_BIT );

	//drawQuadrants(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, 10);
	drawFern(ONE_MILLION); 

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
