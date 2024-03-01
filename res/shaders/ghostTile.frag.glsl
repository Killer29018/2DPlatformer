#version 430

in vec2 v_Texcoords;

out vec4 f_Colour;

uniform ivec2 u_TilemapSize;
uniform ivec2 u_TileIndex[3];
uniform int u_TileIndexSize;

uniform float u_TimeElapsed;

uniform ivec2 u_TileSize;

uniform sampler2DArray u_Texture;

vec4 tileFrag()
{
    vec2 texcoord_2d = v_Texcoords;
    texcoord_2d = mod(texcoord_2d * u_TileSize, 1.);

    ivec2 tilePosInBlock = ivec2(floor(v_Texcoords * u_TileSize));
    int indexInBlock = tilePosInBlock.y * u_TileSize.x + tilePosInBlock.x;

    ivec2 index = u_TileIndex[0];

    int tileIndex = index.y * u_TilemapSize.x + index.x;

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
