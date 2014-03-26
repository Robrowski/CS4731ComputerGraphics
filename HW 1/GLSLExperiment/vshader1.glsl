#version 150

in  vec4 vPosition;
uniform mat4 Proj;

void
main()
{
    gl_Position = Proj*vPosition;
}
