#include "Angel.h"  // Angel.h is homegrown include file, which also includes glew and freeglut
#include "utils.h"


// These products account for material and light source RGBness
vec4 AmbientProduct  = vec4(0.2, 0.2, 0.2, 1.0);
vec4 DiffuseProduct  = vec4(1.0, 0.8, 0.0, 1.0);
vec4 SpecularProduct = vec4(1.0, 1.0, 1.0, 1.0);
vec4 LightPosition   = vec4(1.0, 2.0, 3.0, 1.0);
GLfloat shininess = 100.0f; // 100 default


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

#define RAND_F (GLfloat) 1.0*rand()/RAND_MAX
#define RAND_F_VEC4 vec4(RAND_F,RAND_F,RAND_F, 1.0)

void randomizeLighting(void){

	AmbientProduct  = RAND_F_VEC4;
	DiffuseProduct  = RAND_F_VEC4;
	SpecularProduct = RAND_F_VEC4;
	LightPosition   = RAND_F_VEC4;	 
	shininess = (GLfloat) rand();
}


// Now for shadows
mat4 shadowProjection;
void setShadowStatus(int status){
	sendIntToShader( "isShadow", status );
}

void initShadows(void){
	shadowProjection = Angel::identity();
	shadowProjection[3][1] = -1.0/LightPosition.y;
	//shadowProjection[3][3] = 0;
	printm(shadowProjection);
	int sign = 1;
	shadowProjection = Translate(LightPosition.x *sign , LightPosition.y*sign, LightPosition.z*sign)*shadowProjection*Translate(LightPosition.x*-sign, LightPosition.y*-sign, LightPosition.z*-sign);
	printm(shadowProjection);
}

mat4 getShadowProjection(void){
	return shadowProjection;
}







