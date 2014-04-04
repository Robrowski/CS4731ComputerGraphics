#version 150

uniform mat4 Proj;
uniform mat4 model_matrix;
uniform mat4 CTM;
uniform float twist;

in  vec4 vPosition;
in  vec4 vColor;
out vec4 interpolatedColor;

void main() 
{
  // PROTIP #3
  // can extend to gl_Position = projection * camera * models * vertex
  // you may wish to view camera motion as a transform on the projection (projection * camera) * models ...
  // this could be thought of as moving the observer in space
  // or you may view camera motion as transforming all objects in the world while the observer remains still
  // projection * (camera * models) * ...
  // notice mathematically there is no difference
  // however in rare circumstances the order of the transform may affect the numerical stability
  // of the overall projection
  //mat4 rot = Rotate(twist);
 
 float angles = radians( twist*vPosition.y);
 
 float c = cos( angles );
 float s = sin( angles );
 mat4 rot = mat4( c,   0.0, s, 0.0,
				  0.0, 1.0, 0.0, 0.0,
				 -s, 0.0, c, 0.0,
			      0.0, 0.0, 0.0, 1.0 );


  gl_Position = CTM*rot*vPosition;
  interpolatedColor = vColor;
  
  // Old
 //  gl_Position = Proj*model_matrix*vPosition;
 // gl_Position = Proj*vPosition;
  
} 
