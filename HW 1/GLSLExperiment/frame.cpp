#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"





// Make a new Frame
Frame newFrame(float L,	float R, float B,float T){
	Frame newVPD;
	newVPD.L = L;
	newVPD.R = R;
	newVPD.T = T;
	newVPD.B = B;
	return newVPD;
}

// Adapter from VPDims to view port
void setGLViewport(Frame toSet){
	glViewport(toSet.L,toSet.B, toSet.R, toSet.T);
}

// Adapter from neVPD to mat4
mat4 Frame_Ortho2D(Frame vpd){
	return Ortho2D(vpd.L, vpd.R, vpd.B, vpd.T);
}