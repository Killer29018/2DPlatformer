#version 430

in vec2 v_Texcoords;

out vec4 f_Colour;

uniform int u_Collision;

uniform ivec2 u_TilemapSize;
// uniform ivec2 u_TilemapPosition;
uniform int u_TileIndex;

uniform ivec2 u_TileSize;

uniform sampler2D u_Texture;

void main()
{
    vec2 texCoords = v_Texcoords;
    texCoords = mod(texCoords * u_TileSize, 1.);

    vec2 tileSize = 1. / vec2(u_TilemapSize);

    vec2 tilePosition = vec2(mod(u_TileIndex, u_TilemapSize.x), u_TileIndex / u_TilemapSize.x);

    texCoords = (texCoords * tileSize) + tileSize * tilePosition;

    if (u_Collision <= 0)
    {
        // f_Colour = vec4(texCoords, 0.0, 1.0);
        f_Colour = texture(u_Texture, texCoords);
    }
    else
    {
        f_Colour = vec4(1.0, 0.0, 0.0, 1.0);
    }
}
