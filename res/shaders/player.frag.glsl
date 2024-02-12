#version 430

in vec2 v_Texcoords;

out vec4 f_Colour;

uniform sampler2D u_Texture;

void main()
{
    f_Colour = texture(u_Texture, v_Texcoords);
}
