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


void drawQuadrants(int X, int Y, int width, int height, int numRecursions, MyPicture pics[], int iterationNumber){
	// A Picture
	MyPicture *pic = &pics[iterationNumber];
	
	// Send transformation over to shader
	// **** MOVE THIS TO DRAW PICTURE
	sendOrthoToShader(pic->f);
	
	// Top Left = red
	setViewPort(X, Y + height/2, width/2, height/2, pic->f);
	setLineColor(RED_VEC);
	drawPicture(pic);

	// Bottom left = blue
	setViewPort(X, Y, width/2, height/2, pic->f);
	setLineColor(BLUE_VEC);
	drawPicture(pic);

	// Top right = green
	setViewPort(X + width/2, Y + height/2 ,width/2, height/2,pic->f);
	setLineColor(GREEN_VEC);
	drawPicture(pic);
	
	// If final iteration, place the thing in the corner in red
	if (numRecursions == 1){
		setViewPort(X + width/2, Y, width/2, height/2, pic->f);
		setLineColor(RED_VEC);
		drawPicture(pic);
		return; // Done!
	}// else
	// Next iteration!
	drawQuadrants(X + width/2, Y, width/2, height/2, numRecursions-1, pics, iterationNumber + 1);
}


/**  // OLD VERSION
MyPicture* drawQuadrants(int X, int Y, int width, int height, int numRecursions){
	// A Picture
	MyPicture* pic =  readRandomPicture();
	
	// Send transformation over to shader
	// **** MOVE THIS TO DRAW PICTURE
	sendOrthoToShader(pic->f);
	
	// Top Left = red
	setViewPort(X, Y + height/2, width/2, height/2, pic->f);
	setLineColor(RED_VEC);
	drawPicture(pic);

	// Bottom left = blue
	setViewPort(X, Y, width/2, height/2, pic->f);
	setLineColor(BLUE_VEC);
	drawPicture(pic);

	// Top right = green
	setViewPort(X + width/2, Y + height/2 ,width/2, height/2,pic->f);
	setLineColor(GREEN_VEC);
	drawPicture(pic);
	
	// If final iteration, place the thing in the corner in red
	if (numRecursions == 1){
		setViewPort(X + width/2, Y, width/2, height/2, pic->f);
		setLineColor(RED_VEC);
		drawPicture(pic);
		return pic; // Done!
	}// else
	// Next iteration!
	drawQuadrants(X + width/2, Y, width/2, height/2, numRecursions-1);
	return pic;
}

*/




/*%%%%%%%%%%%%%%%%%%%%%%%%%*//*%%%%%%%%%%%%%%%%%%%%%%%%%*/
/** These two functions have the potential to be made into a "framework"
class that gets extended and such with two functions to fill in
- init variables
- calculate points

*/
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
	setViewPort(0, 0 , WINDOW_WIDTH, WINDOW_HEIGHT, fernPic->f);
	drawMyPicture(fernPic, GL_POINTS);
	free(fernPic);
}

/** Take the number of iterations and draw the fern for that many points */
void drawGingerBreadMan(int iterations){
	// Going to make a picture out of it!
	MyPicture* gingerPic = generateEmptyPicture(1);
	gingerPic->pl = generateEmptyPolyline(iterations);
	MyPoint* pt = gingerPic->pl->pt; // array of points in MyPolyline
	gingerPic->f = newFrame(0,0,0,0);// Initially...

	// Initial point
	pt->x = 0;
	pt->y = 0;

	//Constants
	const int M = 40;
	const int L = 3;

	// Generate the points
	int iter, chosenIndex;
	for (iter = 1; iter < iterations; iter++){
		pt[iter].x =  M*(1 + 2L) - pt[iter - 1].y + abs(pt[iter-1].x - L*M);
		pt[iter].y =  pt[iter - 1].x;


		// Check for edges of world space
		if (pt[iter].x > gingerPic->f.R) gingerPic->f.R = pt[iter].x;
		if (pt[iter].x < gingerPic->f.L) gingerPic->f.L = pt[iter].x;
		if (pt[iter].y > gingerPic->f.T) gingerPic->f.T = pt[iter].y;
		if (pt[iter].y < gingerPic->f.T) gingerPic->f.B = pt[iter].y;

	}
	 
	setLineColor(BLUE_VEC);
	sendOrthoToShader(gingerPic->f);
	setViewPort(0, 0 , WINDOW_WIDTH, WINDOW_HEIGHT, gingerPic->f);
	drawMyPicture(gingerPic, GL_POINTS);
	free(gingerPic);
}

void drawSierpinski(void ){
	const int NumPoints = 100000;
	vec2 points[NumPoints];
	// Specifiy the vertices for a triangle
	vec2 vertices[3] = {vec2( -1.0, -1.0 ), vec2( 0.0, 1.0 ), vec2( 1.0, -1.0 )};
	// An arbitrary initial point inside the triangle
	points[0] = point2(0.25, 0.50);
	// compute and store N-1 new points
	for ( int i = 1; i < NumPoints; ++i ) {
		int j = rand() % 3; // pick a vertex at random
		// Compute the point halfway between the selected vertex
		// and the previous point
		points[i] = ( points[i - 1] + vertices[j] ) / 2.0;
	}

	setLineColor(RED_VEC);
	setGLViewport(DEFAULT_WORLD_FRAME);
	sendOrthoToShader(newFrame(-1,1, -1,1  ));
	// Send off data and draw it
	glBufferData( GL_ARRAY_BUFFER, sizeof(points), points , GL_STATIC_DRAW );
	glDrawArrays( GL_POINTS, 0, NumPoints);    // draw
}



/*%%%%%%%%%%%%%%%%%%%%%%%%%*/
void display( void )
{
	// Start by clearing the window
	glClear( GL_COLOR_BUFFER_BIT );

	//drawQuadrants(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, 3);
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
