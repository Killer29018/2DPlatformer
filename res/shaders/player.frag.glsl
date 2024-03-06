#version 430

in vec2 v_Texcoords;

out vec4 f_Colour;

uniform int u_FlipTextureX;
uniform ivec2 u_Index;
uniform ivec2 u_TextureSize;
uniform sampler2DArray u_TextureMap;

void main()
{
    vec2 texCoord = v_Texcoords;

    if (u_FlipTextureX == 1)
        texCoord.x = 1 - texCoord.x;

    int index = u_Index.x + u_Index.y * u_TextureSize.x;
    f_Colour = texture(u_TextureMap, vec3(texCoord, index));

    if (f_Colour.a < 0.1)
        discard;
}
