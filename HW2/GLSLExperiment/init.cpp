#include "Angel.h"
#include "utils.h"
#include <time.h>


// Good shit
GLuint program;

// Specifically for vertex shader and scaling
GLint ProjLoc;
GLint colorLoc;


/** Basic inits for the general case */
void genericInit( int argc, char **argv, char* window){
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );                // Window size: 640 wide X 480 high
	glutInitWindowPosition(100,5);               // Top left corner at (100, 50)
    glutCreateWindow( window );            // Create Window

    glewInit();		// init glew

	
	// General Stuff
	srand(time(NULL)); // Set random number seed
}

// Only used to set up buffers
void initGPUBuffers1( void )
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


void shaderSetup1( void )
{
	// Load shaders and use the resulting shader program
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(0) );


	glClearColor( 1.0, 1.0, 1.0, 1.0 );        // sets white as color used to clear screen

	// Setting world window shit
	ProjLoc = glGetUniformLocation(program, "Proj");
}
