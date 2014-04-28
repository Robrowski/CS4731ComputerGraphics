#version 150

// Texture crap
uniform int useTexture;
in  vec2 texCoord;
uniform sampler2D texture;

out vec4  fColor;
in vec4 interpolatedColor;


void main() 
{ 
	fColor = interpolatedColor;

	if (useTexture == 1){
		fColor = texture2D( texture, texCoord );
	}
} 

