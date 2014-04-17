#version 150

//uniform mat4 Proj;  //deprecated
//uniform mat4 model_matrix; //deprecated
uniform mat4 CTM;
//uniform float twist; //deprecated

in  vec4 vPosition;
//in  vec4 vColor; //deprecated
//out vec4 interpolatedColor; //deprecated

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
 
// float angles = radians( twist*vPosition.y); //deprecated
 
 // float c = cos( angles ); //deprecated
 // float s = sin( angles ); //deprecated
 // mat4 rot = mat4( c,   0.0, s, 0.0,
				  // 0.0, 1.0, 0.0, 0.0,
				 // -s, 0.0, c, 0.0,
			      // 0.0, 0.0, 0.0, 1.0 );

  gl_Position = CTM*vPosition;
  
  //gl_Position = CTM*rot*vPosition; //deprecated
//  interpolatedColor = vColor; //deprecated
  
  // Old //deprecated
 //  gl_Position = Proj*model_matrix*vPosition; //deprecated
 // gl_Position = Proj*vPosition; //deprecated
  
} 
