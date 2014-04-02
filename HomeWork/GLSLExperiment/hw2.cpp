// HW 1. 
// Robert Dabrowski   rpdabrowski@wpi.edu


#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"
//#include <stdlib.h>

//----------------------------------------------------------------------------
int width = WINDOW_WIDTH;
int height = WINDOW_HEIGHT;

// remember to prototype
void generateGeometry( void );
void display( void );
void keyboard( unsigned char key, int x, int y );
void quad( int a, int b, int c, int d );
void colorcube(void);
void drawCube(void);

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;


using namespace std;

const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
GLuint buffer;
mat4 CTM;
mat4 nextTransform;
point4 points[NumVertices];
color4 colors[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
};
// RGBA olors
color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};
// quad generates two triangles for each face and assigns colors
//    to the vertices
int Index = 0;
void quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a]; Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c]; Index++;
    colors[Index] = vertex_colors[d]; points[Index] = vertices[d]; Index++;
}

// generate 12 triangles: 36 vertices and 36 colors
void colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

// This stuff has to be done for each picture
void bufferData(void){
	// Create and initialize a buffer object ONCE VERTEX AND COLOR DATA IS ACCUMULATED
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),  NULL, GL_STATIC_DRAW );

	// ACTUALLY SENDS DATA
	glBufferSubData( GL_ARRAY_BUFFER, 0,              sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );

    // set up vertex arrays
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,   BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,  BUFFER_OFFSET(sizeof(points)) );
}

void generateGeometry( void )
{	
    colorcube();

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // sets the default color to clear screen
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
	
	// Load shaders and use the resulting shader program
    program = InitShader( "vshader1.glsl", "fshader1.glsl" );
    glUseProgram( program );
	 
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
	


	bufferData();
	

}


void drawCube(void)
{
	// change to GL_FILL
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	// draw functions should enable then disable the features 
	// that are specifit the themselves
	// the depth is disabled after the draw 
	// in case you need to draw overlays
	glEnable( GL_DEPTH_TEST );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
	glDisable( GL_DEPTH_TEST ); 
}


void printMat4(mat4 m){
	printf("%4.2f   %4.2f   %4.2f   %4.2f\n", m[0][0],m[0][1],m[0][2],m[0][3]);	
	printf("%4.2f   %4.2f   %4.2f   %4.2f\n", m[1][0],m[1][1],m[1][2],m[1][3]);	
	printf("%4.2f   %4.2f   %4.2f   %4.2f\n", m[2][0],m[2][1],m[2][2],m[2][3]);	
	printf("%4.2f   %4.2f   %4.2f   %4.2f\n\n", m[3][0],m[3][1],m[3][2],m[3][3]);
}


//----------------------------------------------------------------------------
// this is where the drawing should happen
void display( void )
{
	// remember to enable depth buffering when drawing in 3d

	// TIP1: if you get bogged down with many matrix operations
	// or need to perform some complexed fitting or solving
	// it may be time to include some package like diffpac or lapack
	// http://www.netlib.org/lapack/#_lapack_for_windows
	// http://www.diffpack.com/

	// TIP2: if you feel that GLSL is too restrictive OpenCL or CUDA can be
	// used to generate images or other information, they support interface to OpenGL
	// http://www.khronos.org/registry/cl/

	// TIP3: prototype your operations in Matlab or Mathematica first to verify correct behavior
	// both programs support some sort of runtime interface to C++ programs

	// TIP4: check your graphics specs. you have a limited number of loop iterations, storage, registers, texture space etc.
	
	// TIP5: take a look at the "Assembly" generated from the opengl compilers, it might lead you to some optimizations
	// http://http.developer.nvidia.com/Cg/cgc.html

	// avoid using glTranslatex, glRotatex, push and pop
	// pass your own view matrix to the shader directly
	// refer to the latest OpenGL documentation for implementation details

	// PROTIP1: You can access the depth buffer value and screen location at each fragment
	// in the fragment shader, go wild

	// PROTIP2: Render stuff to texture, then run filters on the texture in a second pass to 
	// produce cool effects
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window

	// PROTIP4: Do not set the near and far plane too far appart!
	// depth buffers do not have unlimited resolution
	// surfaces will start to fight as they come nearer to each other
	// if the planes are too far appart (quantization errors :(   )

	// PROTIP5: If you must have extreme distances, use multiple render passes
	// which divide the entire scene into adjacent viewing volumes
	// render far away volumes first
	// also scale your objects appropriatly, dont use scales at the upper or lower bounds
	// of floating point precision

	// WARNING1: I believe Angel::transpose(...) does not transpose a mat4, but just returns
	// an identical matrix, can anyone verify this?


	// set up projection matricies
	
	GLuint ctmMatrix = glGetUniformLocationARB(program, "CTM");
	glUniformMatrix4fv( ctmMatrix, 1, GL_TRUE, CTM);

	drawCube();


	// use this call to double buffer
	glutSwapBuffers();
	// you can implement your own buffers with textures
}


void initCTM(void){
	// Probably dependent on the picture
	Angel::mat4 perspectiveMat = Angel::Perspective((GLfloat)45.0, (GLfloat)width/(GLfloat)height, (GLfloat)0.1, (GLfloat) 100.0);

	// arbitrary transformations
	Angel::mat4 modelMat = Angel::identity();
	modelMat = modelMat * Angel::Translate(0.0, 0.0, -2.0f) * Angel::RotateY(45.0f) * Angel::RotateX(35.0f);
	printMat4(modelMat);

	nextTransform = Angel::identity();
	CTM = perspectiveMat*modelMat;
}





//----------------------------------------------------------------------------

char idleMode = GL_TRUE;

// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	case 'I':
		idleMode = GL_TRUE;
		break;
	case 'i':
		idleMode = GL_FALSE;
		break;
	
	default:
		printf("Not Implemented! \n");
		break;


	case 'X': // Translate in positive X
		nextTransform = Angel::Translate( 0.1f,0.0,0.0);
		break;
	case 'x': // Translate in negative X
		nextTransform = Angel::Translate( -0.1f,0.0,0.0);
		break;

	case 'Y': // Translate in positive Y
		nextTransform = Angel::Translate( 0.0,0.1f,0.0);
		break;
	case 'y': // Translate in negative Y
		nextTransform = Angel::Translate( 0.0,-0.1f,0.0);
		break;

	case 'Z': // Translate in positive Z
		nextTransform = Angel::Translate( 0.0,0.0,0.1f);
		break;
	case 'z': // Translate in negative Z
		nextTransform = Angel::Translate(  0.0,0.0,-0.1f);
		break;


	/// WTF THIS SHOULDN'T WORK
	case 'S':
	case 's':
		printf("Stopping the Object");
		nextTransform = Angel::identity();
		break;
	case 'q':
   
	
	case 033:
        exit( EXIT_SUCCESS );
        break;
    }
	
	if (idleMode != GL_TRUE){
		CTM = CTM*nextTransform;
	}
	display();
}


// Multiply the current transform matrix by the next transform to do fun stuff!
void idleTransformations(void){
	if (idleMode == GL_TRUE){
		CTM = CTM*nextTransform;
		display();
		glutPostRedisplay(); //?
	}
}


//----------------------------------------------------------------------------
// entry point
int HW2( int argc, char **argv )
{
	
	genericInit(argc, argv, "Color Cube");
	initCTM();

    generateGeometry();

	// assign handlers
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	// should add menus
	// add mouse handler
	// add resize window functionality (should probably try to preserve aspect ratio)

	glutIdleFunc(idleTransformations);

	// enter the drawing loop
	// frame rate can be controlled with 
    glutMainLoop();
    return 0;
}



