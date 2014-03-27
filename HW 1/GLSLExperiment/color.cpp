#include "Angel.h"
#include "utils.h"

// Sends teh given color vector to the fragment shader for colorizing
void setLineColor(vec4 toSet){
	glUniform4fv( glGetUniformLocation(program, "fragmentShaderLineColor"),1, toSet );
}