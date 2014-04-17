#version 150


uniform vec4 fragmentShaderLineColor;
//in  vec4  interpolatedColor; //deprecated
out vec4  fColor;

void main() 
{ 
	fColor = fragmentShaderLineColor;
    //fColor = interpolatedColor; //deprecated
} 

