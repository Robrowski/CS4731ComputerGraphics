#include "Angel.h"
#include "utils.h"


// untested
void sendMat4ToShader(char* variable, mat4 matrix){
	glUniformMatrix4fv( glGetUniformLocationARB(program, variable), 1, GL_TRUE, matrix);
}



// untested
void sendVec4ToShader(char* variable, vec4 toSet){
	glUniform4fv( glGetUniformLocation(program, variable),1, toSet );
}



void sendIntToShader(char* variable, int toSet){
	glUniform1i( glGetUniformLocation(program, variable), toSet );
}

void sendFloatToShader(char* variable, float toSet){
	glUniform1f( glGetUniformLocation(program, variable), toSet );
}



