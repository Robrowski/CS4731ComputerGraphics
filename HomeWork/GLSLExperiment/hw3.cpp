// HW 3. 
// Robert Dabrowski   rpdabrowski@wpi.edu

#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"
#include <stdlib.h>
using namespace std;
//----------------------------------------------------------------------------
// Camera Position variables
GLfloat delU = 0;
GLfloat delV = 0;
GLfloat delN = 0;
GLfloat pitch = 0;
GLfloat roll = 0;
GLfloat yaw = 0;

// The degrees each mesh should have turned
GLfloat meshYRotate = 0;

// Modes
bool extentMode = false;
bool sinusoidMode = false;
GLfloat sinusoidModeAngle = 0;

// Pre-loaded pictures and static transforms
PLYPicture pics[9];       // 9 pictures pre-loaded
mat4 staticTransforms[9]; // static transforms for each layer
mat4 staticScales[9]; // because scale last
GLfloat links[9];  // coordinate of top of link

// Location of CTM in vertex shader
GLuint ctmMatrix;

// Regular Constants
#define DEG_TO_RAD          M_PI/180

// Tuning Constants
#define MESH_Y_ROT_INC 0.5f
#define SINUSOID_INC   0.1f
#define SINUSOID_AMP   0.25f
#define ROTATION_INCREMENT    2 // degrees
#define CAM_ROT_INC                (-1)*ROTATION_INCREMENT// degrees
#define SLIDE_INC .01


// Helper to draw a pic by number - handles relative transformations
// Pushes current transformation and assumes previous + camera are on stack
void drawAPic(GLint n){
	drawPLYPicture3(&pics[n]); // someday swap this out to just swap VBO's
	mat4 sinusoidTrans = Translate(0,sin(sinusoidModeAngle)*SINUSOID_AMP ,0);

	pushMatrix( peekMatrix()*staticTransforms[n]);
	// stuff in nextMat doesn't get pushed.. but could
	mat4 nextMat = peekMatrix()*RotateY(meshYRotate*(n*.4 + 0.1))*staticScales[n]*sinusoidTrans;
	glUniformMatrix4fv( ctmMatrix, 1, GL_TRUE, nextMat);

	setColor(n);
	glDrawArrays( GL_TRIANGLES, 0, pics[n].numPointsInPicture ); 

	// Drawing Connecting links
	MyPoint pts[4];
	pts[0] = MyPoint(0,0,0,1);
	pts[1] = MyPoint(0,.3,0,1);
	pts[2] = MyPoint(0,.3,0,1);
	pts[3] = MyPoint(links[n],.3,0,1);
	
	// Draw links, but not for the top one.
	if (n > 0){
		// Prepare buffer
		glBufferData( GL_ARRAY_BUFFER, sizeof(pts) ,  pts, GL_STATIC_DRAW );
	
		// Have to do this every time...
		GLuint vPosition = glGetAttribLocation( program, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,   BUFFER_OFFSET(0) );
	
		glUniformMatrix4fv( ctmMatrix, 1, GL_TRUE, peekMatrix()*Translate(0,sin(sinusoidModeAngle)*SINUSOID_AMP*.02 ,0));
		glDrawArrays(GL_LINES,0,4);
	}

	// Handy place to draw extents
	if (extentMode){
		glUniformMatrix4fv( ctmMatrix, 1, GL_TRUE, peekMatrix()*staticScales[n]*sinusoidTrans);
		MyPoint points[24];
		MyPoint max = pics[n].max;
		MyPoint min = pics[n].min;

		// Lazy way of taking max/mins to define a rectangular prism
		points[0 ] = min;
		points[1 ] = MyPoint(  min.x,  max.y,  min.z, 1.0f); 
		points[2 ] = MyPoint(  max.x,  max.y,  min.z, 1.0f); 
		points[3 ] = MyPoint(  max.x,  min.y,  min.z, 1.0f); 
		points[4 ] = MyPoint(  max.x,  min.y,  max.z, 1.0f); 
		points[5 ] = max;
		points[6 ] = MyPoint(  min.x,  max.y,  max.z, 1.0f); 
		points[7 ] = MyPoint(  min.x,  min.y,  max.z, 1.0f); 
		points[8 ] = min; 
		points[9 ] = MyPoint(  max.x,  min.y,  min.z, 1.0f); 
		points[10] = MyPoint(  max.x,  max.y,  min.z, 1.0f); 
		points[11] = max;
		points[12] = MyPoint(  min.x,  max.y,  max.z, 1.0f); 
		points[13] = MyPoint(  min.x,  max.y,  min.z, 1.0f); 
		points[14] = MyPoint(  min.x,  max.y,  max.z, 1.0f);
		points[15] = MyPoint(  min.x,  min.y,  max.z, 1.0f); 
		points[16] = MyPoint(  max.x,  min.y,  max.z, 1.0f);
		points[17] = MyPoint(  max.x,  min.y,  min.z, 1.0f);

		// Prepare buffer
		glBufferData( GL_ARRAY_BUFFER, sizeof(points) ,  points, GL_STATIC_DRAW );
	
		// Have to do this every time...
		GLuint vPosition = glGetAttribLocation( program, "vPosition" );
		glEnableVertexAttribArray( vPosition );
		glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,   BUFFER_OFFSET(0) );

		glDrawArrays( GL_LINE_LOOP, 0, 18 );
	}
}



// Pitch, rotate on u
	// roll, rotate on n
	// yaw, rotate on v
	// slide on u or v or n
mat4 MyLookAt(void )
{
	// Starting points and vector
	vec4 at =  vec4(0,0,0 ,1);
	vec4 eye = vec4(0,0,.3 ,1);	  
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


	/**********************************************************************
	MATRIX STACK HERE Fun part of pushing and popping as we go through matrix stack
	***********************************************************************/
	resetMatrixStack();
	// Might as well put camera on the stack	
	// Prepratory transformation matrics
	mat4 cam_ctm = Translate(delU, delV, delN)*RotateZ(roll)*RotateY(yaw)*RotateX(pitch); // Alternate method that is meh 
//	pushMatrix(cam_ctm); // i dont like this camera as much 
	pushMatrix(MyLookAt( ));
	

	// Draw each mesh using the matrix stack	
	drawAPic(0 );// layer 1
	pushMatrix( peekMatrix()*RotateY(meshYRotate*1.05));
		drawAPic(8 );// layer 2
		popMatrix();
		drawAPic(1); // other half of layer 2
		pushMatrix( peekMatrix()*RotateY(-meshYRotate*1.42));
			drawAPic(2 );// layer 3
			popMatrix();
			drawAPic(3 );// other half of layer 3
			pushMatrix( peekMatrix()*RotateY(meshYRotate));
				drawAPic(7 );// layer 4
				popMatrix();
				drawAPic(4 );// other half of layer 4
				pushMatrix( peekMatrix()*RotateY(-meshYRotate));
					drawAPic(5 );// layer 5
					popMatrix();
					drawAPic(6 );// other half of layer 5
	

	glDisable( GL_DEPTH_TEST ); 
	glutSwapBuffers();
}



// keyboard handler
void keyboard3( unsigned char key, int x, int y )
{
    switch ( key ) {
	default:
		printf("Not Implemented! \n");
		break;

	case 's':
	case 'S':
		sinusoidMode = !sinusoidMode;
		break;
	
	case 'e':
	case 'E':
		extentMode = !extentMode;
		break;
	
	// Slider camera
	case 'N': delN += SLIDE_INC*5; break;
	case 'n': delN -= SLIDE_INC*5; printf("delN: %f\n",delN); break;
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

	// Quit commands
	case 'q':
	case 033:
        exit( EXIT_SUCCESS );
        break;
    }
}


// Idly rotate the meshes and handle sinusoidal movement
void idleTransformations3(void){
	meshYRotate += MESH_Y_ROT_INC;
	
	if (sinusoidMode){
		sinusoidModeAngle += SINUSOID_INC;
	}
	glutPostRedisplay(); 
}


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

// Read PLY files and set some static tranformation stuff for later
void initPLYPictures(void){
	int i;
	for (i = 0; i < 9 ; i++){
		pics[i] = *readPLYFile(plyToUse[i]); 	
		// Ideally, load the PLY pictures to VBO's right now
		mat4 scaleToFitWindow = Scale(0.8/max(pics[i].max.x,pics[i].max.y, pics[i].max.z, -pics[i].min.x, -pics[i].min.y, -pics[i].min.z)); 
		staticScales[i] =  scaleToFitWindow*Scale(.2); // Maybe have them bigger?
	}

	// Top layer
	staticTransforms[0] = Translate(0,0.8,0); // moved to top
	links[0] = 0;

	// 2nd layer - all have parent of 0
	float rotationRadius = .65;
	staticTransforms[1] =    Translate(rotationRadius*.5,-.3,0);
	staticScales[1] *= Scale(.75); // foot
	links[1] = -rotationRadius*.5;
	staticTransforms[8] =    Translate(-rotationRadius*.5,-.3,0);
	staticScales[8] *= Scale(1.5); // stratocaster
	links[8] = rotationRadius;


	// 3rd layer parent = 1
	rotationRadius *= .5;
	staticTransforms[2] = Translate(rotationRadius,-.3,0); 
	staticScales[2] *= Scale(.65); // tennishoe	
	links[2] = -rotationRadius;
	staticTransforms[3] = Translate(-rotationRadius, -.3,0);
	staticScales[3] *= Scale(.5); // ant
	links[3] = rotationRadius;

	// 4th layer - parent = 3
	rotationRadius *= .4;
	staticTransforms[7] = Translate(rotationRadius,-.3,0);
	staticScales[7] *= Scale(.4); // urn
	links[7] = -rotationRadius;
	staticTransforms[4] = Translate(-rotationRadius,-.3,0);
	staticScales[4] *= Scale(.4); // beethoven
	links[4] = rotationRadius;

	// 5th layer - parent = 4
	//rotationRadius *= .5;
	staticTransforms[5] = Translate(rotationRadius,-.3,0);
	staticScales[5] *= Scale(.6); // sandal
	links[5] = -rotationRadius;
	staticTransforms[6] = Translate(-rotationRadius,-.3,0);
	staticScales[6] *= Scale(.7);// hammerhead
	links[6] = rotationRadius;
}


//----------------------------------------------------------------------------
int HW3( int argc, char **argv )
{
	genericInit(argc, argv, "Homework 3: Robert Dabrowski");

    shaderSetupTwo();
	initPLYPictures();
	initMatrixStack(20); // extra big
	ctmMatrix = glGetUniformLocationARB(program, "CTM");
	
    glutDisplayFunc( display3 );
    glutKeyboardFunc( keyboard3 );
	glutIdleFunc(idleTransformations3);

    glutMainLoop();
	
	return 0;
}