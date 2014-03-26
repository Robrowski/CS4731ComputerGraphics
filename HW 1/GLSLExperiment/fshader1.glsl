#version 150

out vec4  fColor; // works


uniform vec4 fragmentShaderLineColor;

void
main()
{
    fColor = fragmentShaderLineColor;
}
