#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"



vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
vec4 LightPosition;
float shininess;


void setLightingStatus(int status){
	sendIntToShader( "useLighting", status );
}











