#version 150

out vec4  fColor;
in vec4 newColor;

void
main()
{
    fColor = newColor;
}
