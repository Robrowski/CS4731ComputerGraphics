// HW 1. 
// Robert Dabrowski   rpdabrowski@wpi.edu


#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"
#include <stdlib.h>

using namespace std;


/*%%%%%%%%%%%%%%%%%%%%%%%%%*/
void display2( void )
{
	// Start by clearing the window
	glClear( GL_COLOR_BUFFER_BIT );


	drawFern(ONE_MILLION); 

	// THE FINAL STEP
    glFlush();			// force output to graphics hardware
}


/** Main function for loading polyline files and
*   drawing them.
*
*/
int HW2( int argc, char **argv )
{
	// main function: program starts here
	genericInit(argc, argv);

	// Calbacks
    glutDisplayFunc( display2 );                    // Register display callback function
    glutKeyboardFunc( keyboard );                  // Register keyboard callback function

	// Can add minimalist menus here
	// add mouse handler
	// add resize window functionality (should probably try to preserve aspect ratio)

	// enter the drawing loop
    glutMainLoop();
    return 0;
}
