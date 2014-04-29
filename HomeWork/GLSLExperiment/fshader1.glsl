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
in vec3 T;

out vec4  fColor;
in vec4 interpolatedColor;


void main() 
{ 


	if (useTexture == 1){  // If ground
		fColor = texture2D( textureGround, texCoord );
	} else if (reflectMode == 1){ // If reflection Mode
		fColor = texture(texMap, R);
	} else if (refractMode == 1){ // refract mode
		fColor = mix(texture(texMap, T),  vec4( 1.0, 1.0, 1.0, 1.0 ), 0.3);
	} else {
		fColor = interpolatedColor;
	}
} 

