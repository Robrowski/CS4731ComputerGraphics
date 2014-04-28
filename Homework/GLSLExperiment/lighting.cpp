#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"


// Light Source

vec4 diffuse0   = vec4(1.0, 0.0, 0.0, 1.0);
vec4 ambient0   = vec4(1.0, 0.0, 0.0, 1.0);
vec4 specular0  = vec4(1.0, 0.0, 0.0, 1.0);
vec4 light0_pos = vec4(1.0, 2.0, 3.0, 1.0);











vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
vec4 LightPosition;
float shininess;


void setLightingStatus(int status){
	sendIntToShader( "useLighting", status );
}











