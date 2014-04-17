// HW 2. 
// Robert Dabrowski   rpdabrowski@wpi.edu

#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"
#include <stdlib.h>

//----------------------------------------------------------------------------
int width = WINDOW_WIDTH;
int height = WINDOW_HEIGHT;




GLfloat delU = 0;
GLfloat delV = 0;
GLfloat delN = 0;
GLfloat pitch = 0;
GLfloat roll = 0;
GLfloat yaw = 0;



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


int whichPic = 0;

mat4 CTM, nextTransform;


GLfloat speedMultiplier = 1.0;
#define DEG_TO_RAD M_PI/180
#define TRANSLATION_INCREMENT 0.01f*speedMultiplier
#define ROTATION_INCREMENT    2 
#define CAM_ROT_INC                (-1)*ROTATION_INCREMENT//*DEG_TO_RAD
#define SCALE_INCREMENT       0.99f*speedMultiplier
#define SLIDE_INC .01


void drawAPic(GLint n){
	drawPLYPicture3(&pics[n]);
	setColor(n);
	glDrawArrays( GL_TRIANGLES, 0, pics[n].numPointsInPicture ); // = Num triangles * 3
}



// Pitch, rotate on u
	// roll, rotate on n
	// yaw, rotate on v
	// slide on u or v or n
mat4 MyLookAt(void )
{
	// Starting points and vector
	vec4 at =  vec4(0,0,0 ,1);
	vec4 eye = vec4(0,0,1 ,1);	  
	vec4 up = vec4(0,1,0,0);

	// Math stolen from Angel::LookAt
	vec4 n1 = eye - at;
	n1.w = 0;
    vec4 n = normalize(n1);
    vec4 u = normalize(vec4(cross(up,n), 0));
    vec4 v = normalize(vec4(cross(n,u), 0));
    vec4 t = vec4(0.0, 0.0, 0.0, 1.0);

	// Handle roll
	GLfloat r = -roll*DEG_TO_RAD;
	vec4 rot_n = n;
	vec4 rot_u = u*cos(r) - v*sin(r);
	vec4 rot_v = u*sin(r) + v*cos(r);
	n = rot_n;
	u = rot_u;
	v = rot_v;

	// pitch
	GLfloat p = -pitch*DEG_TO_RAD;
	rot_n =  n*cos(p) - v*sin(p);
	rot_v = n*sin(p) + v*cos(p);
	n = rot_n;
	u = rot_u;
	v = rot_v;

	// yaw
	GLfloat y = -yaw*DEG_TO_RAD;
	rot_n = n*cos(y) + u*sin(y);
	rot_u = -n*sin(y) + u*cos(y);
	n = rot_n;
	u = rot_u;
	v = rot_v;

	// Handle sliding
	GLfloat ex = eye.x + delU*u.x + delV*v.x + delN*n.x;
	GLfloat ey = eye.y + delU*u.y + delV*v.y + delN*n.y;
	GLfloat ez = eye.z + delU*u.z + delV*v.z + delN*n.z;
	vec4 eyeSlide = vec4( ex, ey,ez,1);


    mat4 c = mat4(u, v, n, t);
    return c * Translate( -eyeSlide );
}



//----------------------------------------------------------------------------
// this is where the drawing should happen
// ASSUME GPU BUFFERS ARE ALREADY LOADED
void display3( void )
{
	// set up projection matricies
	GLuint ctmMatrix = glGetUniformLocationARB(program, "CTM");
	glEnable( GL_DEPTH_TEST );
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	// PROTIP4: Do not set the near and far plane too far appart!
	// depth buffers do not have unlimited resolution
	// surfaces will start to fight as they come nearer to each other
	// if the planes are too far appart (quantization errors :(   )

	// PROTIP5: If you must have extreme distances, use multiple render passes
	// which divide the entire scene into adjacent viewing volumes
	// render far away volumes first
	// also scale your objects appropriatly, dont use scales at the upper or lower bounds
	// of floating point precision

	mat4 cam_ctm = Translate(delU, delV, delN)*RotateZ(roll)*RotateY(yaw)*RotateX(pitch); // Alternate method that is meh
	printf("Matrix shit");
	printm(cam_ctm);
	mat4 camera = MyLookAt( ); 
	printf("Lookat");
	printm(camera);
	cam_ctm = camera;
	
	// CTM will be fore moving individual parts?!?!
	glUniformMatrix4fv( ctmMatrix, 1, GL_TRUE, cam_ctm*staticTransforms[whichPic]*staticScales[whichPic]); 
	drawAPic(whichPic );

	glUniformMatrix4fv( ctmMatrix, 1, GL_TRUE, cam_ctm*staticTransforms[whichPic+1]*staticTransforms[whichPic]*staticScales[whichPic+1]);
	drawAPic(whichPic + 1);



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
	nextTransform = Angel::identity();
    switch ( key ) {
	// Commands that I made up


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
	
	// Slider camera
	case 'N': delN += SLIDE_INC; break;
	case 'n': delN -= SLIDE_INC; break;
	case 'V': delV += SLIDE_INC; break;
	case 'v': delV -= SLIDE_INC; break;
	case 'U': delU += SLIDE_INC; break;
	case 'u': delU -= SLIDE_INC; break;
	
	// Camera Rotations
	case 'j': yaw -= CAM_ROT_INC; break;
	case 'J': yaw += CAM_ROT_INC; break;
	case 'k': roll -= CAM_ROT_INC; break;
	case 'K': roll += CAM_ROT_INC; break;
	case 'l': pitch -= CAM_ROT_INC; break;
	case 'L': pitch += CAM_ROT_INC; break;

	// Reset camera
	case 'r':
		yaw = 0; roll = 0; pitch = 0;
		delN = 0; delU = 0; delV = 0;
		break;




	// Quit commandsq
	case 'q':
	case 033:
        exit( EXIT_SUCCESS );
        break;
    }

	
	CTM = nextTransform*CTM;

	display3();
}


// Multiply the current transform matrix by the next transform to do fun stuff!
void idleTransformations3(void){

	
	// Standard continuation of transformations	
	//if (idleMode == GL_TRUE){
		CTM = CTM*nextTransform;
		display3();
		glutPostRedisplay(); //?
//	}
}






void initPLYPictures(void){
	int i;
	for (i = 0; i < 9 ; i++){
		pics[i] = *readPLYFile(plyToUse[i]); 	
		mat4 scaleToFitWindow = Scale(0.8/max(pics[i].max.x,pics[i].max.y, pics[i].max.z, -pics[i].min.x, -pics[i].min.y, -pics[i].min.z)); 
		staticScales[i] =  scaleToFitWindow*Scale(.3); // Maybe have them bigger?
	}

	// Top layer
	staticTransforms[0] = Translate(0,0.8,0); // moved to top

	// 2nd layer - all have parent of 0
	//  rotate, then translate in a vector down and out
	mat4 layer2Translate = Translate(.4,-.4,0);
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
	genericInit(argc, argv, "Homework 3: Robert Dabrowski");
	initCTM();

    shaderSetupTwo();
	initPLYPictures();

	//// assign handlers
    glutDisplayFunc( display3 );
    glutKeyboardFunc( keyboard3 );
	//glutIdleFunc(idleTransformations3);

    glutMainLoop();
	
	return 0;
}





