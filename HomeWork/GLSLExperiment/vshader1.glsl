#version 150

// Tranformations
uniform mat4 CTM;
uniform mat4 camera;

// Basic COlor
uniform vec4 fragmentShaderLineColor;

// Lighting
uniform int useLighting;
uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform vec4 LightPosition;
uniform float Shininess;

// Reflection/Refraction
uniform int reflectMode;
uniform int refractMode;

// In to this (vertex Shader)
in  vec4 vPosition;
in  vec3 vNormal;

// Out to Fragment Shader
out vec2 texCoord;
out vec4 interpolatedColor; 
out vec3 R;
out vec3 T;

void main() 
{
	gl_Position = CTM*vPosition;
	texCoord = vPosition.xz;
	interpolatedColor = fragmentShaderLineColor;

	// Reflection crap
	if (reflectMode == 1){
		vec3 eyePos = vPosition.xyz;
		vec4 normal = vec4(vNormal.x, vNormal.y, vNormal.z, 0);
		vec4 NN = CTM*normal;
		vec3 N = normalize(NN.xyz);
		
		R = reflect(eyePos, N);
	} else if (refractMode == 1){
		vec4 eyePos = vPosition; 
		vec4 normal = vec4(vNormal.x, vNormal.y, vNormal.z, 0);
		vec4 NN = CTM*normal; 
		vec3 N =normalize(NN.xyz); 
		
		T = refract(eyePos.xyz, N, 1); 				
	} else if (useLighting == 1){// Blegh lighting
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
