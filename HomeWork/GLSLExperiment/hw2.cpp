// HW 2. 
// Robert Dabrowski   rpdabrowski@wpi.edu

#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"
#include <stdlib.h>

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

typedef  vec4  point4;

using namespace std;


mat4 CTM;
mat4 ReshapeMat;
mat4 nextTransform;

PLYPicture* pic;
GLint numPoints;


char colorMode = GL_TRUE;
char idleMode = GL_TRUE;
GLfloat speedMultiplier = 1.0;
#define TRANSLATION_INCREMENT 0.01f*speedMultiplier
#define ROTATION_INCREMENT    0.5f*speedMultiplier
#define SCALE_INCREMENT       0.99f*speedMultiplier



// Make a shear matrix that sheers by h
mat4 shear(GLfloat h){
	mat4 sheared = Angel::identity();

	sheared[0][1] = h;

	return sheared;
}

// Make a twist matrix that twists by t
mat4 twist(GLfloat t){
	mat4 twisted = Angel::RotateY(10);

	twisted[0][1] = .3*t;
	twisted[2][1] = .3*t;
	
	printm(twisted);
	return twisted;
}




//----------------------------------------------------------------------------
// this is where the drawing should happen
// ASSUME GPU BUFFERS ARE ALREADY LOADED
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



	// set up projection matricies
	GLuint ctmMatrix = glGetUniformLocationARB(program, "CTM");
	glUniformMatrix4fv( ctmMatrix, 1, GL_TRUE, CTM*ReshapeMat);


	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	// draw functions should enable then disable the features 
	// that are specifit the themselves
	// the depth is disabled after the draw 
	// in case you need to draw overlays
	glEnable( GL_DEPTH_TEST );
    glDrawArrays( GL_TRIANGLES, 0, numPoints ); // = Num triangles * 3
	glDisable( GL_DEPTH_TEST ); 



	// use this call to double buffer
	glutSwapBuffers();
	// you can implement your own buffers with textures
}


// Set the nextTransformation to be the identity matrix
void stopCTM(void){
	nextTransform = Angel::identity();
}

// Initializes the current transformation matrix and others
void initCTM(void){
	CTM = Angel::identity();
	stopCTM();
	ReshapeMat = Angel::identity();
}


// If a key is repeated, the motion is stopped. Else, set motion
// Mat = a motion because it will be the "nextTransformation"
// cmd is the command key
char lastCommand = 'q';
void toggleTransform(mat4 mat, char cmd){
	if (cmd == lastCommand){
		stopCTM();
		lastCommand = 'q'; //default
	} else {
		nextTransform = mat;
		lastCommand = cmd;
	}
}

// A macro for compressing a case statement
#define COMMAND_CASE(mat,key) case key: toggleTransform(mat,key); break;


// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	// Commands that I made up
	case 'I':
		printf("Idle mode on\n");
		idleMode = GL_TRUE;
		break;
	case 'i':
		printf("Idle mode off\n");
		idleMode = GL_FALSE;
		break;
	case 'U':
		printf("Speed increased:\n   Will not take effect until next command.\n");
		speedMultiplier += 1;
		break;
	case 'u':
		printf("Speed decreased:\n   Will not take effect until next command.\n");
		speedMultiplier -= 1;
		if (speedMultiplier < 1){
			speedMultiplier = 0.5;
			printf("Minimum speed = .5\n");
		}
		break;
	case 'S':
	case 's':
		printf("Stopping the Object\n");
		nextTransform = Angel::identity();
		break;
	default:
		printf("Not Implemented! \n");
		break;

	// Because I wanted to do scale too
	case 'L': 
		ReshapeMat = ReshapeMat*Scale(1/SCALE_INCREMENT);
		break;
	case 'l': 
		ReshapeMat = ReshapeMat*Scale(  SCALE_INCREMENT);
		break;

	// My BETTER rotational commands
	COMMAND_CASE( RotateX( ROTATION_INCREMENT),'D');
	COMMAND_CASE( RotateX(-ROTATION_INCREMENT),'d');
	COMMAND_CASE( RotateY( ROTATION_INCREMENT),'E');
	COMMAND_CASE( RotateY(-ROTATION_INCREMENT),'e');
	COMMAND_CASE( RotateZ( ROTATION_INCREMENT),'F');
	COMMAND_CASE( RotateZ(-ROTATION_INCREMENT),'f');

	// Translational commands
	// ALL translational commands STOP the previous translation
	COMMAND_CASE( Translate(  TRANSLATION_INCREMENT,0.0,0.0),'X');// Translate in positive X
	COMMAND_CASE( Translate( -TRANSLATION_INCREMENT,0.0,0.0),'x');// Translate in negative X		
	COMMAND_CASE( Translate( 0.0, TRANSLATION_INCREMENT,0.0),'Y'); // Translate in positive Y
	COMMAND_CASE( Translate( 0.0,-TRANSLATION_INCREMENT,0.0),'y');// Translate in negative Y
	COMMAND_CASE( Translate( 0.0,0.0, TRANSLATION_INCREMENT),'Z'); // Translate in positive Z
	COMMAND_CASE( Translate( 0.0,0.0,-TRANSLATION_INCREMENT),'z');// Translate in negative Z


	// Rotational Commands
	// R

	// Color Commands
	//c = toggle between random or red
	case 'c':
		colorMode = !colorMode;
		if (colorMode){
			glBufferSubData( GL_ARRAY_BUFFER,  numPoints*sizeof(MyPoint), sizeof(color4)*numPoints, randomColors(numPoints) );
		 } else{
			glBufferSubData( GL_ARRAY_BUFFER,  numPoints*sizeof(MyPoint), sizeof(color4)*numPoints, redArray(numPoints) );
		 }
		break;

	
	// Shearing
	case 'h': // h = increment +X shearing
		ReshapeMat = ReshapeMat*shear( 0.5);
		break;
	case 'H': // H = decrement 
		ReshapeMat = ReshapeMat*shear(-0.5);
		break;

		// Twisting
	case 't': // t = increment +y twist
		ReshapeMat = ReshapeMat*twist(1);
		break;
	case 'T': // T = decrement
		ReshapeMat = ReshapeMat*twist(-1);
		break;



	// Changing wireframes
	case 'N':
		pic = readPLYFile(nextFile());
		numPoints = pic->numPointsInPicture;
		drawPLYPicture(pic);
		break;
	case 'P':
		pic = readPLYFile(prevFile());
		numPoints = pic->numPointsInPicture;
		drawPLYPicture(pic);
		break;
		

	// Reset
	case 'W':
		CTM =  Angel::identity();
		stopCTM();
		break;

	// Quit commands
	case 'q':
	case 033:
        exit( EXIT_SUCCESS );
        break;
    }
	

	// Idle mode logic
	if (idleMode != GL_TRUE){
		CTM = CTM*nextTransform;
		stopCTM();
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

	shaderSetupTwo();


  //  generateGeometry();
//	pic = generatePLYCube();
	pic = readPLYFile(nextFile());
	numPoints = pic->numPointsInPicture;
	drawPLYPicture(pic);


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



