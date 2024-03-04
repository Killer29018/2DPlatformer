#version 430

in vec2 v_Texcoords;

out vec4 f_Colour;

uniform ivec2 u_Index;
uniform ivec2 u_TextureSize;
uniform sampler2DArray u_TextureMap;

void main()
{
    int index = u_Index.x + u_Index.y * u_TextureSize.x;
    f_Colour = texture(u_TextureMap, vec3(v_Texcoords, index));
}
