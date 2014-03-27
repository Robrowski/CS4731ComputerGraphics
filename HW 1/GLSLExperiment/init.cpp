#include "Angel.h"
#include "utils.h"
#include <time.h>


// Good shit
GLuint program;

// Specifically for vertex shader and scaling
GLint ProjLoc;
GLint colorLoc;


/** Basic inits for the general case */
void genericInit( int argc, char **argv ){
	glutInit( &argc, argv );                       // intialize GLUT  
    glutInitDisplayMode( GLUT_SINGLE | GLUT_RGB ); // single framebuffer, colors in RGB
    glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );                // Window size: 640 wide X 480 high
	glutInitWindowPosition(100,5);               // Top left corner at (100, 50)
    glutCreateWindow( "CS 4731: HW1     Robert Dabrowski" );            // Create Window

    glewInit();		// init glew

	initGPUBuffers();
	shaderSetup();
	
	// General Stuff
	srand(time(NULL)); // Set random number seed
}

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
