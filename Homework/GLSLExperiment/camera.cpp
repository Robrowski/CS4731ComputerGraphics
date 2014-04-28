#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"



// Camera Position variables
MyPoint eye = MyPoint(0,0,.3,1);
vec3 u,v,n;
mat4 NewCamera;


mat4 getCamera(void){
	return NewCamera;
}


void MySetModelViewMatrix(void)
{ // load modelview matrix with camera values
	mat4 m;
	vec3 eVec(eye.x, eye.y, 	eye.z);// eye as vector
	m[0][0] = u.x; m[0][1] = u.y; m[0][2] = 	u.z; m[0][3] = -dot(eVec,u);
	m[1][0] = v.x; m[1][1] = v.y; m[1][2] = 	v.z; m[1][3] = -dot(eVec,v);
	m[2][0] = n.x; m[2][1] = n.y; m[2][2] = n.z; m[2][3] = -dot(eVec,n);
	m[3][0] = 0;   m[3][1] =   0; m[3][2] =   0; m[3][3] = 1.0;
	NewCamera = m;
	printm(NewCamera);
}

void MySlide(float dU, float dV, float dN)
{
	eye.x += dU*u.x + dV*v.x + dN*n.x;
	eye.y += dU*u.y + dV*v.y + dN*n.y;
	eye.z += dU*u.z + dV*v.z + dN*n.z;
	MySetModelViewMatrix( );
}


// roll, rotate on n
void MyRoll(float angle)
{ // roll the camera through angle degrees
	float cs = cos(DEG_TO_RAD * angle);
	float sn = sin(DEG_TO_RAD * angle);
	vec3 X = u; // remember old u
	vec3 Y = v;
	u = vec3(cs*X.x + sn*Y.x, cs*X.y + sn*Y.y, cs*X.z + sn*Y.z);
	v = vec3(-sn*X.x + cs*Y.x, -sn*X.y + cs*Y.y, -sn*X.z + cs*Y.z) ;
	MySetModelViewMatrix( );
} 

// yaw, rotate on v
void MyYaw(float angle)
{ // roll the camera through angle degrees
	float cs = cos(DEG_TO_RAD * angle);
	float sn = sin(DEG_TO_RAD * angle);
	vec3 X = n; 
	vec3 Y = u;
	n = vec3(cs*X.x + sn*Y.x, cs*X.y + sn*Y.y, cs*X.z + sn*Y.z);
	u = vec3(-sn*X.x + cs*Y.x, -sn*X.y + cs*Y.y, -sn*X.z + cs*Y.z) ;
	MySetModelViewMatrix( );
} 

// Pitch, rotate on u
void MyPitch(float angle)
{ // roll the camera through angle degrees
	float cs = cos(DEG_TO_RAD * angle);
	float sn = sin(DEG_TO_RAD * angle);
	vec3 X = v; 
	vec3 Y = n;
	v = vec3(cs*X.x + sn*Y.x, cs*X.y + sn*Y.y, cs*X.z + sn*Y.z);
	n = vec3(-sn*X.x + cs*Y.x, -sn*X.y + cs*Y.y, -sn*X.z + cs*Y.z) ;
	MySetModelViewMatrix( );
} 

void initCamera(void){
	u = vec3(1,0,0);
	v = vec3(0,1,0);
	n = vec3(0,0,1);
	eye = MyPoint(0,-.7,3.3,1);

	MySetModelViewMatrix();
}