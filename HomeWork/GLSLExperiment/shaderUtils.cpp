#include "Angel.h"
#include "utils.h"


// untested
void sendMat4ToShader(char* variable, mat4 matrix){
	GLuint loc =  glGetUniformLocationARB(program, variable);
	glUniformMatrix4fv( loc, 1, GL_FALSE, matrix);
}


