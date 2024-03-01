#version 430

in vec2 v_Texcoords;

out vec4 f_Colour;

uniform ivec2 u_TilemapSize;
uniform ivec2 u_TileIndex[3];
uniform int u_TileIndexSize;

uniform ivec2 u_TileSize;

uniform int u_ShowBlock;
uniform int u_BlockID;

uniform sampler2DArray u_Texture;

int hash(int x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

void main()
{
    vec2 texcoord_2d = v_Texcoords;
    texcoord_2d = mod(texcoord_2d * u_TileSize, 1.);

    ivec2 tilePosInBlock = ivec2(floor(v_Texcoords * u_TileSize));
    int indexInBlock = tilePosInBlock.y * u_TileSize.x + tilePosInBlock.x;

    int subIndex = hash(indexInBlock) % u_TileIndexSize;

    ivec2 index = u_TileIndex[subIndex];

    int tileIndex = index.y * u_TilemapSize.x + index.x;

    vec3 texcoord = vec3(texcoord_2d.xy, tileIndex);

    if (u_ShowBlock == 1)
    {
        float a = 1;
        float b = ((u_BlockID >>  0) & 0xFF) / 255.;
        float g = ((u_BlockID >>  8) & 0xFF) / 255.;
        float r = ((u_BlockID >> 16) & 0xFF) / 255.;
        f_Colour = vec4(r, g, b, a);
    }
    else
    {
        f_Colour = texture(u_Texture, texcoord);

        if (f_Colour.a == 0)
            discard;
    }

}
