// HW 1. 
// Robert Dabrowski   rpdabrowski@wpi.edu


#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"
#include <stdlib.h>

using namespace std;

//***************** RECURSIVE FUNCTION!!!
// Specific for HW for drawing in quadrants in iterations
void drawQuadrants(int X, int Y, int width, int height, int numRecursions, MyPicture pics[], int iterationNumber){
	// A Picture
	MyPicture *pic = &pics[iterationNumber];
	
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


/*%%%%%%%%%%%%%%%%%%%%%%%%%*/
void display1( void )
{
	// Start by clearing the window
	glClear( GL_COLOR_BUFFER_BIT );


	drawFern(ONE_MILLION); 


	glutSwapBuffers();
}


/** Main function for loading polyline files and
*   drawing them.
*
*/
int HW1( int argc, char **argv )
{
	// main function: program starts here
	genericInit(argc, argv, "HOMEWORK 1");
	initGPUBuffers1();
	shaderSetup1();


	// Calbacks
    glutDisplayFunc( display1 );                    // Register display callback function
    glutKeyboardFunc( keyboard1 );                  // Register keyboard callback function

	// Can add minimalist menus here
	// add mouse handler
	// add resize window functionality (should probably try to preserve aspect ratio)

	// enter the drawing loop
    glutMainLoop();
    return 0;
}
