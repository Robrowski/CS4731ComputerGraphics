#version 150

// Texture crap
uniform int reflectMode;
uniform int refractMode;
uniform int useTexture;
in  vec2 texCoord;
uniform sampler2D textureGround;

// Cube shit
uniform samplerCube texMap;
in vec3 R;

out vec4  fColor;
in vec4 interpolatedColor;


void main() 
{ 
	fColor = interpolatedColor;

	if (useTexture == 1){
		fColor = texture2D( textureGround, texCoord );
	}
	
	if (reflectMode == 1){
		fColor = texture(texMap, R);
	}
	
} 

