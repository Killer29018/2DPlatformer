#version 430

in vec2 v_Texcoords;

out vec4 f_Colour;

uniform ivec2 u_TilemapSize;
uniform ivec2 u_TileIndex;

uniform float u_TimeElapsed;

uniform ivec2 u_TileSize;

uniform sampler2DArray u_Texture;

void main()
{
    vec2 texcoord_2d = v_Texcoords;
    texcoord_2d = mod(texcoord_2d * u_TileSize, 1.);

    int tileIndex = u_TileIndex.y * u_TilemapSize.x + u_TileIndex.x;

    vec3 texcoord = vec3(texcoord_2d.xy, tileIndex);

    float mixValue = clamp(cos(2 * u_TimeElapsed), 0.1, 1.0);

    vec4 textureColour = texture(u_Texture, texcoord);

    if (textureColour.a == 0)
        discard;

    textureColour.a = 0.5;

    f_Colour.a = 0.5;

    f_Colour = mix(textureColour, vec4(0.8), mixValue);
}
