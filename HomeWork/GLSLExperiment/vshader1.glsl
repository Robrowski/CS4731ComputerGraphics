#version 150

// Tranformations
uniform mat4 CTM;
uniform mat4 camera;

// Basic COlor
uniform vec4 fragmentShaderLineColor;

// Lighting
uniform int useLighting;
// light and material properties
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;


// In to this (vertex Shader)
in  vec4 vPosition;
in  vec3 vNormal;

// Out to Fragment Shader
out vec2 texCoord;
out vec4 interpolatedColor; 


void main() 
{
  gl_Position = CTM*vPosition;
  texCoord = vPosition.xz;

  interpolatedColor = fragmentShaderLineColor;
  
   
  if (useLighting == 1){
	// Transform vertex position into eye coordinates
	vec3 pos = (CTM * vPosition).xyz; /// My CTM include projection...
	vec3 L = normalize( LightPosition.xyz - pos );
	vec3 E = normalize( -pos );
	vec3 H = normalize( L + E ); // halfway Vector
	// Transform vertex normal into eye coordinates
	vec3 N = normalize( CTM*vec4(vNormal, 0.0) ).xyz; /// My CTM includes projection...
  
  // Compute terms in the illumination equation
	vec4 ambient = AmbientProduct;
	float cos_theta = max( dot(L, N), 0.0 );
	vec4 diffuse = cos_theta * DiffuseProduct;
	float cos_phi = pow( max(dot(N, H), 0.0), Shininess );
	vec4 specular = cos_phi * SpecularProduct;
	if( dot(L, N) < 0.0 ) specular = vec4(0.0, 0.0, 0.0, 1.0); 
		
	interpolatedColor = ambient + diffuse + specular;
	interpolatedColor.a = 1.0;
  }
  
  
} 
