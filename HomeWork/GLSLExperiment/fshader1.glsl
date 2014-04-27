#version 150

// Texture crap
uniform int useTexture;
in  vec2 texCoord;
uniform sampler2D texture;

uniform vec4 fragmentShaderLineColor;
out vec4  fColor;

void main() 
{ 
	fColor = fragmentShaderLineColor;

	if (useTexture == 1){
		fColor = texture2D( texture, texCoord );
	}
} 

