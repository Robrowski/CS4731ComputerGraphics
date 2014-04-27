#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"
#include <stdlib.h>

mat4 *stack;
GLint stackSize = 0;
GLint stackTop = -1; // index of top of stack

/** Takes a stack size and initializes the matrix stack  */
void initMatrixStack(GLint newSize){
	stack = (mat4*) calloc(newSize, sizeof(mat4));

	stackSize = newSize;
	stackTop = -1;

	if (newSize == 0){
		stackTop = -1; // safety precaution
	}
}

void resetMatrixStack(void){
	stackTop = -1;
}


void pushMatrix(mat4 m){
	if (stackTop  < stackSize - 1){
		stackTop++;
		stack[stackTop] = m;
	}
	glUniformMatrix4fv( glGetUniformLocationARB(program, "CTM"), 1, GL_TRUE, m);

}

// Because it is stupid to pop then push immediately after
mat4 peekMatrix(void){
	if (stackTop != -1){
		return stack[stackTop];
	}
	return Angel::identity(); 
}


mat4 popMatrix(void){
	mat4 topMat = peekMatrix();
	stackTop--;
	return topMat;
}
