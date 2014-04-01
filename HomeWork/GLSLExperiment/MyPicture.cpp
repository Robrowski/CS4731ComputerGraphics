#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"



// Takes a picture and draws it for the given line type
void drawMyPicture(MyPicture *pic, GLint lineType){
	int lineNum;
	for (lineNum = 0; lineNum < pic->numLines; lineNum++){
		drawMyPolyline( pic->pl[lineNum], lineType);
	}
}

// Draws the given picture, assumes lines
void drawPicture(MyPicture *pic){
	sendOrthoToShader(pic->f);
	drawMyPicture( pic,GL_LINE_STRIP);
}