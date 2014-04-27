#version 150

uniform mat4 CTM;
out vec2 texCoord;


in  vec4 vPosition;
//in  vec4 vColor; //deprecated
//out vec4 interpolatedColor; //deprecated

void main() 
{


  gl_Position = CTM*vPosition;
  texCoord = vPosition.xz;

  
} 
