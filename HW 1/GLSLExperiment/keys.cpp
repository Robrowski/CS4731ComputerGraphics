
#include "Angel.h"
#include "utils.h"

int currentIteration = 1;


void keyboard( unsigned char key, int x, int y )
{
	// Start by clearing the window
	glClear( GL_COLOR_BUFFER_BIT );

    switch ( key ) {
	// Cases that fall through and intentionally close the program
	case 'q':
	case 'w':
	case 'e':
	case 033:			// 033 is Escape key octal value
        exit(1);		// quit program
        break;
	case 'f':
		drawFern(ONE_MILLION); 
		break;
	case 'm':
		drawQuadrants(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, 1);
		break;
	case 'p':
		currentIteration++;
		drawQuadrants(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, currentIteration);
		break;
	case 'l':
		currentIteration--;
		if (currentIteration < 1) currentIteration = 1;
		drawQuadrants(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, currentIteration);
		break;
	}
	

	// THE FINAL STEP
    glFlush();	








}
