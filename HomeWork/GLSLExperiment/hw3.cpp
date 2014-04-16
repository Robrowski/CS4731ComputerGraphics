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


typedef  vec4  point4;

using namespace std;

// Pre-loaded pictures and static transforms
PLYPicture pics[9];       // 9 pictures pre-loaded
mat4 staticTransforms[9]; // static transforms for each layer
mat4 staticScales[9]; // because scale last

char* plyToUse[9] = { "PLYFiles/big_porsche.ply",
	"PLYFiles/footbones.ply",
	"PLYFiles/tennis_shoe.ply",
	"PLYFiles/ant.ply",
	"PLYFiles/beethoven.ply",
	"PLYFiles/sandal.ply",
	"PLYFiles/hammerhead.ply",
	"PLYFiles/urn2.ply",
	"PLYFiles/stratocaster.ply"
};




mat4 CTM;
mat4 ReshapeMat;
mat4 nextTransform;

PLYPicture* pic;
GLint numPoints;

GLfloat currentShear = 0;
char colorMode = GL_TRUE;
char idleMode = GL_TRUE;
GLfloat speedMultiplier = 1.0;
#define TRANSLATION_INCREMENT 0.01f*speedMultiplier
#define ROTATION_INCREMENT    0.5f*speedMultiplier
#define SCALE_INCREMENT       0.99f*speedMultiplier



//----------------------------------------------------------------------------
// this is where the drawing should happen
// ASSUME GPU BUFFERS ARE ALREADY LOADED
void display3( void )
{
	// PROTIP1: You can access the depth buffer value and screen location at each fragment
	// in the fragment shader, go wild

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



	// Lookin at ---- THIS ISN'T EVEN USED!?!?
	vec4 at = vec4(0,0,0,1);
	vec4 eye = vec4(0,0,1,1);	
	vec4 up  = vec4(0,  1, 0,0);     
	mat4 camera = Angel::LookAt(eye    ,at,up ); 	// Page 213 from text book


	// set up projection matricies
	GLuint ctmMatrix = glGetUniformLocationARB(program, "CTM");
	glUniformMatrix4fv( ctmMatrix, 1, GL_TRUE, CTM*staticTransforms[0]*staticScales[0]);

	
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	// TODO:  SEND COLOR                       setColor(index)



	// TODO: have to resent CTM once for each PLY, + draw arrays once per PLY



	// draw functions should enable then disable the features 
	// that are specifit the themselves
	// the depth is disabled after the draw 
	// in case you need to draw overlays
	glEnable( GL_DEPTH_TEST );
    glDrawArrays( GL_TRIANGLES, 0, numPoints ); // = Num triangles * 3
	glDisable( GL_DEPTH_TEST ); 

	glutSwapBuffers();
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
	currentShear = 0;
}

// If a key is repeated, the motion is stopped. Else, set motion
// Mat = a motion because it will be the "nextTransformation"
// cmd is the command key
char lastCommand = 'q';
char showRoom = GL_FALSE;
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
void keyboard3( unsigned char key, int x, int y )
{
	// Always turn this off when a key is pressed
	showRoom = GL_FALSE;
	
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
	case 'b':
		printf("CTM: ");
		printm(CTM);
	
		break;
	case 'S':
	case 's':
		printf("Stopping the Object\n");
		nextTransform = Angel::identity();
		break;
	default:
		printf("Not Implemented! \n");
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



	// Changing wireframes
	case 'N':
		initCTM();
		pic = readPLYFile(nextFile());
		numPoints = pic->numPointsInPicture;
		drawPLYPicture(pic);
		break;
	case 'P':
		initCTM();
		pic = readPLYFile(prevFile());
		numPoints = pic->numPointsInPicture;
		drawPLYPicture(pic);
		break;
	case '[':
		pic = generatePLYCube();
		numPoints = pic->numPointsInPicture;
		drawPLYPicture(pic);
		break;

	// Reset
	case 'W':
		CTM = Angel::identity();
		stopCTM();
		break;

	// Quit commandsq
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
	display3();
}


// Multiply the current transform matrix by the next transform to do fun stuff!
void idleTransformations3(void){

	
	// Standard continuation of transformations	
	if (idleMode == GL_TRUE){
		CTM = CTM*nextTransform;
		display3();
		glutPostRedisplay(); //?
	}
}






void initPLYPictures(void){
	int i;
	for (i = 0; i < 9 ; i++){
		pics[i] = *readPLYFile(plyToUse[i]); 	
		// Call "Draw" function here to load each VBO
	//	drawPLYPicture3(&pics[i],i);
		mat4 scaleToFitWindow = Scale(0.8/max(pics[i].max.x,pics[i].max.y, pics[i].max.z, -pics[i].min.x, -pics[i].min.y, -pics[i].min.z)); 
		staticScales[i] =  scaleToFitWindow*Scale(.3); // Maybe have them bigger?
	}

	// Top layer
	staticTransforms[0] = Translate(0,0.8,0); // no movement!




	// 2nd layer - all have parent of 0
	//  rotate, then translate in a vector down and out
	mat4 layer2Translate = Translate(1,-1,0);
	staticTransforms[1] =   RotateY(0) * layer2Translate;
	staticTransforms[2] = RotateY(120) * layer2Translate;
	staticTransforms[3] = RotateY(240) * layer2Translate;

	// 3rd layer - each of these goes straight down
	mat4 straightDown = Translate(0,-1,0);
	staticTransforms[4] = straightDown; // parent = 1
	staticTransforms[5] = straightDown; // parent = 2
	staticTransforms[6] = straightDown; //parent = 3

	// 4th layer - straight down again
	staticTransforms[7] = straightDown;
	staticTransforms[8] = straightDown;
}




//----------------------------------------------------------------------------
int HW3( int argc, char **argv )
{
	genericInit(argc, argv, "Homework2: Robert Dabrowski");
	initCTM();

	shaderSetupTwo();
//  shaderSetup3();

	initPLYPictures();
	

	numPoints = pics[0].numPointsInPicture;
	drawPLYPicture(&pics[0]);
	pic = &pics[0];

	// assign handlers
    glutDisplayFunc( display3 );
    glutKeyboardFunc( keyboard3 );
	glutIdleFunc(idleTransformations3);

    glutMainLoop();
	return 0;
}





