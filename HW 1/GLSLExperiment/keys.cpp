
#include "Angel.h"
#include "utils.h"

int currentIteration = 0;

#define MAX_ITERATIONS 9
MyPicture pics[MAX_ITERATIONS + 1]; // + 1 for simplified array indexing + laziness

void keyboard1( unsigned char key, int x, int y )
{
	// Init step
	if (currentIteration == 0){
		currentIteration = 1;
		pics [currentIteration] = *readRandomPicture();
	}

	// Start by clearing the window
	glClear( GL_COLOR_BUFFER_BIT );

    switch ( key ) {
	
	case 'm':
		currentIteration = 1;
		pics [currentIteration] = *readRandomPicture();
		drawQuadrants(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, currentIteration, pics, 1);
		break;
	case 'p':
		// Pick a new random pic for this iteration
		currentIteration++;
		if (currentIteration > MAX_ITERATIONS) currentIteration = MAX_ITERATIONS;
		pics[currentIteration] = *readRandomPicture();
	
		drawQuadrants(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, currentIteration, pics, 1);
		break;
	case 'l':
		currentIteration--;
		if (currentIteration < 1) currentIteration = 1;
			
		drawQuadrants(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, currentIteration, pics, 1);
		break;
	case 'g':
		drawGingerBreadMan(ONE_MILLION);
		break;
	case 's':
		drawSierpinski();
		break;
	
	// Cases that cause ferns to grow
	default:
	case 'f':
		drawFern(ONE_MILLION); 
		break;
	
		
	// Cases that fall through and intentionally close the program
	case 'q':
	case 'w':
	case 'e':
	case 033:			// 033 is Escape key octal value
        exit(1);		// quit program
        break;

	}
	


	glutSwapBuffers();
}
