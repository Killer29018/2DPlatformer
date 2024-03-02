#version 430

in vec2 v_Texcoords;

out vec4 f_Colour;

uniform ivec2 u_TilemapSize;
uniform ivec2 u_TileTextureMapping[3];
uniform int u_TileMappingSize;

uniform float u_TimeElapsed;

uniform ivec2 u_BlockSize;

uniform sampler2DArray u_Texture;

vec4 tileFrag()
{
    ivec2 index = u_TileTextureMapping[0];

    int tileIndex = index.y * u_TilemapSize.x + index.x;

    vec2 texcoord_2d = mod(v_Texcoords * u_BlockSize, 1.);
    vec3 texcoord = vec3(texcoord_2d.xy, tileIndex);

    return texture(u_Texture, texcoord);
}

void main()
{
    vec4 textureColour = tileFrag();

    if (textureColour.a == 0)
        discard;

    textureColour.a = 0.5;

    f_Colour.a = 0.5;

    float mixValue = clamp(cos(2 * u_TimeElapsed), 0.1, 1.0);
    f_Colour = mix(textureColour, vec4(0.8), mixValue);
}
