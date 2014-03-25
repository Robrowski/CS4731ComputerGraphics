#include "Angel.h"
#include "utils.h"
#include <time.h>

/** Basic inits for the general case */
void genericInit( int argc, char **argv ){
	glutInit( &argc, argv );                       // intialize GLUT  
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB ); // single framebuffer, colors in RGB
    glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );                // Window size: 640 wide X 480 high
	glutInitWindowPosition(100,150);               // Top left corner at (100, 150)
    glutCreateWindow( "Grundles" );            // Create Window

    glewInit();										// init glew
	srand(time(NULL));
}