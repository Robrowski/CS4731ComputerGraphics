#version 150

uniform mat4 CTM;
uniform mat4 camera;
out vec2 texCoord;


in  vec4 vPosition;
in  vec3 vNormal;


out vec4 interpolatedColor; //deprecated
uniform vec4 fragmentShaderLineColor;

uniform int useLighting;



void main() 
{


  gl_Position = CTM*vPosition;
  texCoord = vPosition.xz;

  interpolatedColor = fragmentShaderLineColor;
  
  
  
  
  if (useLighting == 1){
  
  
  
  
  
  }
  
  
} 
