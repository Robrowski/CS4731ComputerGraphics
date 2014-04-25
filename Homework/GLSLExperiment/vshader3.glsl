#version 150

in vec4 vPosition;
in vec4 poop;
out vec2 texCoord;

uniform mat4 Projection;

void main()
{
	//texCoord = vec2(poop.x, poop.y);
	gl_Position = Projection * vPosition;
}