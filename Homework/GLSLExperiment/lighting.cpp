#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"


// These products account for material and light source RGBness
vec4 AmbientProduct  = vec4(0.2, 0.2, 0.2, 1.0);
vec4 DiffuseProduct  = vec4(1.0, 0.8, 0.0, 1.0);
vec4 SpecularProduct = vec4(1.0, 1.0, 1.0, 1.0);
vec4 LightPosition   = vec4(1.0, 2.0, 3.0, 1.0);
float shininess = 100.0f; // 100 default


void changeShininess(GLfloat toSet){
	shininess = toSet;
}

void setLightingStatus(int status){
	sendIntToShader( "useLighting", status );
}


void sendLightingConstants(void){
	sendVec4ToShader("AmbientProduct",AmbientProduct);
	sendVec4ToShader("DiffuseProduct",DiffuseProduct);
	sendVec4ToShader("SpecularProduct",SpecularProduct);
	sendVec4ToShader("LightPosition",LightPosition);

	sendFloatToShader("Shininess", shininess);
}








