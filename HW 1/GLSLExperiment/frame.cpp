/** This file contains utilies for functions utilizing the "Frame" struct. Inncluded are
aspect ratio calculations and viewport setting helpers. 


Eventually frames will be abstracted into a class and be more useful! 

*/

#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"


// Make a new Frame
//TODO: make a constructor to take origin X, origin y, width, height format 
// and abstract it away
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


// Sends the given frame to the vertex shader
void sendOrthoToShader(Frame f){
	// Should this be GL_TRUE or false?
	glUniformMatrix4fv(ProjLoc,1,GL_TRUE, Frame_Ortho2D(f));
}

// Calculate the aspect ratio based on the given frame
float getAspectRatio(Frame f){
	return (f.R-f.L)/(f.T-f.B);
}

// Sets the view port AND handles aspect ratio!
void setViewPort(int X, int Y, int vpWidth, int vpHeight, Frame wFrame){
	
	// Check aspect ratio stuff, see lecture 4 
	// http://web.cs.wpi.edu/~emmanuel/courses/cs4731/D14/slides/lecture04.pdf 
	int w,h;
	float aspectRatio = getAspectRatio(wFrame);
	if (aspectRatio > vpWidth/vpHeight){
		w = vpWidth;
		h = vpWidth/aspectRatio;
	} else if (aspectRatio < vpWidth/vpHeight){
		w = vpHeight*aspectRatio;
		h = vpHeight;
	} else {
		w = vpWidth;
		h = vpHeight;
	}

	glViewport(X , Y , w, h);
}
