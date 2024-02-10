#version 430

in vec2 v_Texcoords;

out vec4 f_Colour;

uniform int u_Collision;

void main()
{
    if (u_Collision <= 0)
    {
        f_Colour = vec4(v_Texcoords, 0.0, 1.0);
    }
    else
    {
        f_Colour = vec4(1.0, 0.0, 0.0, 1.0);
    }
}
