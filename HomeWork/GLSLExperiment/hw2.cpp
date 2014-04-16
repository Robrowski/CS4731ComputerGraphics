//// HW 2. 
//// Robert Dabrowski   rpdabrowski@wpi.edu
//
//#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
//#include "utils.h"
//#include <stdlib.h>
//
////----------------------------------------------------------------------------
//int width = WINDOW_WIDTH;
//int height = WINDOW_HEIGHT;
//
//// remember to prototype
//void generateGeometry( void );
//void display( void );
//void keyboard( unsigned char key, int x, int y );
//
//
//typedef  vec4  point4;
//
//using namespace std;
//
//mat4 CTM;
//mat4 ReshapeMat;
//mat4 nextTransform;
//
//PLYPicture* pic;
//GLint numPoints;
//
//GLfloat currentShear = 0;
//char colorMode = GL_TRUE;
//char idleMode = GL_TRUE;
//GLfloat speedMultiplier = 1.0;
//#define TRANSLATION_INCREMENT 0.01f*speedMultiplier
//#define ROTATION_INCREMENT    0.5f*speedMultiplier
//#define SCALE_INCREMENT       0.99f*speedMultiplier
//
//float twister = 0.0f;
//
//// Make a shear matrix that sheers by h
//mat4 shear(GLfloat h){
//	mat4 sheared = Angel::identity();
//
//	sheared[0][1] = h;
//
//	return sheared;
//}
//
//
////----------------------------------------------------------------------------
//// this is where the drawing should happen
//// ASSUME GPU BUFFERS ARE ALREADY LOADED
//void display( void )
//{
//	// PROTIP1: You can access the depth buffer value and screen location at each fragment
//	// in the fragment shader, go wild
//
//    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );     // clear the window
//
//	// PROTIP4: Do not set the near and far plane too far appart!
//	// depth buffers do not have unlimited resolution
//	// surfaces will start to fight as they come nearer to each other
//	// if the planes are too far appart (quantization errors :(   )
//
//	// PROTIP5: If you must have extreme distances, use multiple render passes
//	// which divide the entire scene into adjacent viewing volumes
//	// render far away volumes first
//	// also scale your objects appropriatly, dont use scales at the upper or lower bounds
//	// of floating point precision
//
//	// The center of the mesh
//	vec4 at  = vec4( (pic->max.x + pic->min.x)/2,(pic->max.y + pic->min.y)/2,(pic->max.z + pic->min.z)/2 ,1.0);
//	//vec4 eye = vec4( (pic->max.x + pic->min.x)/2,(pic->max.y + pic->min.y)/2, 2*pic->max.z,1);
//	//at = vec4(0,0,0,1);
//	vec4 eye = vec4(0,0,1,1);	
//	vec4 up  = vec4(0,  1, 0,0);
//	mat4 camera = Angel::LookAt(eye    ,at,up ); //*Scale(1/pic->max.x);
//	// Page 213 from text book
//
//
//	// Setting up camera
//	mat4 moveToOrigin  = Translate((pic->max.x + pic->min.x)/-2,(pic->max.y + pic->min.y)/-2,(pic->max.z + pic->min.z)/-2 );
//	mat4 scaled =Scale(0.8/max(pic->max.x,pic->max.y, pic->max.z, -pic->min.x, -pic->min.y, -pic->min.z));
//
//	//camera = scaled*moveToOrigin;
//
//	// set up projection matricies
//	GLuint ctmMatrix = glGetUniformLocationARB(program, "CTM");
//	glUniformMatrix4fv( ctmMatrix, 1, GL_TRUE, scaled*camera*CTM*ReshapeMat);
//
//	// Send twist values
//	glUniform1f(glGetUniformLocation(program,"twist"), twister);
//	
//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
//	// draw functions should enable then disable the features 
//	// that are specifit the themselves
//	// the depth is disabled after the draw 
//	// in case you need to draw overlays
//	glEnable( GL_DEPTH_TEST );
//    glDrawArrays( GL_TRIANGLES, 0, numPoints ); // = Num triangles * 3
//	glDisable( GL_DEPTH_TEST ); 
//
//	glutSwapBuffers();
//}
//
//
//// Set the nextTransformation to be the identity matrix
//void stopCTM(void){
//	nextTransform = Angel::identity();
//}
//
//// Initializes the current transformation matrix and others
//void initCTM(void){
//	CTM = Angel::identity();
//	stopCTM();
//	ReshapeMat = Angel::identity();
//	currentShear = 0;
//}
//
//// If a key is repeated, the motion is stopped. Else, set motion
//// Mat = a motion because it will be the "nextTransformation"
//// cmd is the command key
//char lastCommand = 'q';
//char showRoom = GL_FALSE;
//void toggleTransform(mat4 mat, char cmd){
//	if (cmd == lastCommand){
//		stopCTM();
//		lastCommand = 'q'; //default
//	} else {
//		nextTransform = mat;
//		lastCommand = cmd;
//	}
//}
//
//// A macro for compressing a case statement
//#define COMMAND_CASE(mat,key) case key: toggleTransform(mat,key); break;
//
//
//// keyboard handler
//void keyboard( unsigned char key, int x, int y )
//{
//	// Always turn this off when a key is pressed
//	showRoom = GL_FALSE;
//	
//    switch ( key ) {
//	// Commands that I made up
//	case 'I':
//		printf("Idle mode on\n");
//		idleMode = GL_TRUE;
//		break;
//	case 'i':
//		printf("Idle mode off\n");
//		idleMode = GL_FALSE;
//		break;
//	case 'U':
//		printf("Speed increased:\n   Will not take effect until next command.\n");
//		speedMultiplier += 1;
//		break;
//	case 'u':
//		printf("Speed decreased:\n   Will not take effect until next command.\n");
//		speedMultiplier -= 1;
//		if (speedMultiplier < 1){
//			speedMultiplier = 0.5;
//			printf("Minimum speed = .5\n");
//		}
//		break;
//	case 'b':
//		printf("CTM: ");
//		printm(CTM);
//		printf("\n reshape: ");
//		printm(ReshapeMat);
//		break;
//	case 'S':
//	case 's':
//		printf("Stopping the Object\n");
//		nextTransform = Angel::identity();
//		break;
//	default:
//		printf("Not Implemented! \n");
//		break;
//
//	// Because I wanted to do scale too
//	case 'L': 
//		ReshapeMat = ReshapeMat*Scale(1/SCALE_INCREMENT);
//		break;
//	case 'l': 
//		ReshapeMat = ReshapeMat*Scale(  SCALE_INCREMENT);
//		break;
//
//	// My BETTER rotational commands
//	COMMAND_CASE( RotateX( ROTATION_INCREMENT),'D');
//	COMMAND_CASE( RotateX(-ROTATION_INCREMENT),'d');
//	COMMAND_CASE( RotateY( ROTATION_INCREMENT),'E');
//	COMMAND_CASE( RotateY(-ROTATION_INCREMENT),'e');
//	COMMAND_CASE( RotateZ( ROTATION_INCREMENT),'F');
//	COMMAND_CASE( RotateZ(-ROTATION_INCREMENT),'f');
//
//	// Translational commands
//	// ALL translational commands STOP the previous translation
//	COMMAND_CASE( Translate(  TRANSLATION_INCREMENT,0.0,0.0),'X');// Translate in positive X
//	COMMAND_CASE( Translate( -TRANSLATION_INCREMENT,0.0,0.0),'x');// Translate in negative X		
//	COMMAND_CASE( Translate( 0.0, TRANSLATION_INCREMENT,0.0),'Y'); // Translate in positive Y
//	COMMAND_CASE( Translate( 0.0,-TRANSLATION_INCREMENT,0.0),'y');// Translate in negative Y
//	COMMAND_CASE( Translate( 0.0,0.0, TRANSLATION_INCREMENT),'Z'); // Translate in positive Z
//	COMMAND_CASE( Translate( 0.0,0.0,-TRANSLATION_INCREMENT),'z');// Translate in negative Z
//
//	// Rotational Commands
//	case 'R':
//		showRoom = !showRoom;
//		idleMode = GL_TRUE;
//		break;
//
//	// Color Commands	//c = toggle between random or red
//	case 'c':
//		colorMode = !colorMode;
//		if (colorMode){
//			glBufferSubData( GL_ARRAY_BUFFER,  numPoints*sizeof(MyPoint), sizeof(color4)*numPoints, randomColors(numPoints) );
//		 } else{
//			glBufferSubData( GL_ARRAY_BUFFER,  numPoints*sizeof(MyPoint), sizeof(color4)*numPoints, redArray(numPoints) );
//		 }
//		break;
//
//	// Shearing
//	case 'h': // h = increment +X shearing
//		ReshapeMat = ReshapeMat*shear( 0.5);
//		currentShear += 0.5;
//		break;
//	case 'H': // H = decrement
//		if (currentShear <= 0)	{
//			currentShear = 0;
//		} else {
//			ReshapeMat = ReshapeMat*shear(-0.5);
//			currentShear -= 0.5;
//		}
//		break;
//
//	// Twisting
//	case 't': // t = increment +y twist
//		twister += 0.1;
//		break;
//	case 'T': // T = decrement
//		twister -= 0.1;
//		if (twister < 0) twister = 0;
//		break;
//
//	// Changing wireframes
//	case 'N':
//		initCTM();
//		pic = readPLYFile(nextFile());
//		numPoints = pic->numPointsInPicture;
//		drawPLYPicture(pic);
//		break;
//	case 'P':
//		initCTM();
//		pic = readPLYFile(prevFile());
//		numPoints = pic->numPointsInPicture;
//		drawPLYPicture(pic);
//		break;
//	case '[':
//		pic = generatePLYCube();
//		numPoints = pic->numPointsInPicture;
//		drawPLYPicture(pic);
//		break;
//
//	// Reset
//	case 'W':
//		CTM = Angel::identity();
//		stopCTM();
//		break;
//
//	// Quit commandsq
//	case 'q':
//	case 033:
//        exit( EXIT_SUCCESS );
//        break;
//    }
//
//	// Idle mode logic
//	if (idleMode != GL_TRUE){
//		CTM = CTM*nextTransform;
//		stopCTM();
//	}
//	display();
//}
//
//
//char rotateDirection = -1;
//float rotationProgress = 0;
//// Multiply the current transform matrix by the next transform to do fun stuff!
//void idleTransformations(void){
//	if (showRoom){	
//		nextTransform = RotateY( rotateDirection*ROTATION_INCREMENT);
//		rotationProgress += ROTATION_INCREMENT;
//
//		// After 360 degrees, load next
//		if (rotationProgress >= 360){
//			// Load next picture
//			initCTM();
//			pic = readPLYFile(nextFile());
//			numPoints = pic->numPointsInPicture;
//			drawPLYPicture(pic);
//
//			// Reset tracking variables
//			rotateDirection = rotateDirection*-1;
//			rotationProgress = 0;
//		}
//	}
//	
//	// Standard continuation of transformations	
//	if (idleMode == GL_TRUE){
//		CTM = CTM*nextTransform;
//		display();
//		glutPostRedisplay(); //?
//	}
//}
//
//
////----------------------------------------------------------------------------
//int HW2( int argc, char **argv )
//{
//	genericInit(argc, argv, "Homework2: Robert Dabrowski");
//	initCTM();
//
//	shaderSetupTwo();
//
//	pic = readPLYFile(nextFile());
//	numPoints = pic->numPointsInPicture;
//	drawPLYPicture(pic);
//
//	// assign handlers
//    glutDisplayFunc( display );
//    glutKeyboardFunc( keyboard );
//	glutIdleFunc(idleTransformations);
//
//    glutMainLoop();
//    return 0;
//}
//
//
//
//
//
//
//
//
//
//
//
///** PLY Pictures to use
//
//big_porsche.ply
//footbones.ply
//tennis_shoe.ply
//ant.ply
//beethoven.ply
//hammerhead.ply
//sandal.ply
//stratocaster.ply
//urn2.ply
//
//*/